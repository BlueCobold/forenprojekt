
#include "Animation.hpp"
#include "CloneHandler.hpp"
#include "provider/ValueProvider.hpp"
#include "../Utility.hpp" // toDegree, toPixel
#include "../rendering/GLExt.hpp"
#include "../rendering/Shader.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include <climits>

bool Animation::_renderStencilEffects = true;
std::list<Animation*> Animation::_stencilAnimations;

Animation::Animation(const unsigned int frames,
                     const int frameWidth, const int frameHeight,
                     const bool applyRotation,
                     const sf::Vector2f& origin,
                     const sf::Vector2f& drawOffset,
                     const bool scaleToScreenSize,
                     const bool horizontal) :
    m_applyRotation(applyRotation),
    m_stopOnAlphaZero(false),
    m_frames(frames),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight),
    m_horizontal(horizontal),
    m_externalRotation(0.f),
    m_drawOffset(drawOffset),
    m_blending(Blending::RegularAlpha),
    m_targetBuffer(UINT_MAX),
    m_isViewAligned(false),
    m_shader(nullptr),
    m_cloneHandler(nullptr),
    m_scaleToScreenSize(scaleToScreenSize)
{
    m_sprite.setOrigin(origin);
}

Animation::~Animation()
{
    _stencilAnimations.remove(this);
}

void Animation::update()
{
    if(isStopped())
        return;

    if(!_renderStencilEffects && m_stencil.mode == StencilInfo::Test)
        return;

    if(m_frameProvider == nullptr)
        m_frame = 0;
    else
        m_frame = static_cast<int>(m_frameProvider->getValue()) % m_frames;
    m_sprite.setTextureRect(getTextureRect());

    float rotation = m_externalRotation;
    if(m_rotationProvider != nullptr)
        rotation += m_rotationProvider->getValue();
    m_sprite.setRotation(rotation);

    updatePosition();

    float scaleX = 1, scaleY = 1;
    if(m_xScaleProvider != nullptr)
        scaleX = m_xScaleProvider->getValue();
    if(m_yScaleProvider != nullptr)
        scaleY = m_yScaleProvider->getValue();
    if(m_xScaleProvider != nullptr || m_yScaleProvider != nullptr)
    m_sprite.setScale(scaleX, scaleY);

    if(m_origins.size() != 0)
        m_sprite.setOrigin(static_cast<float>(m_origins[m_frame].x), static_cast<float>(m_origins[m_frame].y));

    sf::Color color(255, 255, 255);
    if(m_colorProviders[Red] != nullptr)
        color.r = static_cast<sf::Uint8>(255*m_colorProviders[Red]->getValue());
    if(m_colorProviders[Green] != nullptr)
        color.g = static_cast<sf::Uint8>(255*m_colorProviders[Green]->getValue());
    if(m_colorProviders[Blue] != nullptr)
        color.b = static_cast<sf::Uint8>(255*m_colorProviders[Blue]->getValue());
    if(m_colorProviders[Alpha] != nullptr)
    {
        float alpha = m_colorProviders[Alpha]->getValue();
        if(m_stopOnAlphaZero && alpha < 0)
        {
            stop();
            return;
        }
        color.a = static_cast<sf::Uint8>(255*alpha);
    }
    if(m_blending == Blending::PreMultipliedAlpha)
    {
        color.r = color.r * color.a / 255;
        color.g = color.b * color.a / 255;
        color.b = color.g * color.a / 255;
    }
    m_sprite.setColor(color);
}

void Animation::reset()
{
    Stoppable::continueNow();

    if(m_frameProvider == nullptr)
        m_frameProvider->reset();

    if(m_xPositionProvider != nullptr)
        m_xPositionProvider->reset();
    if(m_yPositionProvider != nullptr)
        m_yPositionProvider->reset();

    if(m_rotationProvider != nullptr)
        m_rotationProvider->reset();

    if(m_xScaleProvider != nullptr)
        m_xScaleProvider->reset();
    if(m_yScaleProvider != nullptr)
        m_yScaleProvider->reset();

    for(int i = 0; i < 4; i++)
        if(m_colorProviders[i] != nullptr)
            m_colorProviders[i]->reset();
}

void Animation::setPosition(const float x, const float y)
{
    m_externalPosition = sf::Vector2f(x, y);
    m_sprite.setPosition(m_externalPosition + m_dynamicPosition + m_drawOffset);
}

void Animation::updatePosition()
{
    m_dynamicPosition = sf::Vector2f(0, 0);
    if(m_xPositionProvider != nullptr)
        m_dynamicPosition.x += m_xPositionProvider->getValue();
    if(m_yPositionProvider != nullptr)
        m_dynamicPosition.y += m_yPositionProvider->getValue();

    auto offset = sf::Vector2f();
    if(m_offsets.size() != 0)
        offset = sf::Vector2f(m_offsets[m_frame]);
    m_sprite.setPosition(m_externalPosition + m_dynamicPosition + m_drawOffset + offset);
}

void Animation::setRotation(const float radians)
{
    if(m_applyRotation)
        m_externalRotation = utility::toDegree<float, float>(radians);
}

void Animation::setBlending(const Blending::Mode mode)
{
    m_blending = mode;
}

void Animation::bindFrameProvider(std::unique_ptr<ValueProvider> frames)
{
    m_frameProvider = std::move(frames);
}

void Animation::bindTexture(const sf::Texture& texture, const sf::Vector2f& sourceOffset, bool prepareOnUsage)
{
    m_sourceOffset = sourceOffset;
    m_sprite.setTexture(texture);
    m_prepareTextureOnUse = prepareOnUsage;
}

const sf::IntRect Animation::getTextureRect() const
{
    if(m_sizes.size() != 0)
        return m_sizes[m_frame];
    else
        if(m_horizontal)
            return sf::IntRect(static_cast<int>(m_frame * m_frameWidth + m_sourceOffset.x),
                static_cast<int>(m_sourceOffset.y), m_frameWidth, m_frameHeight);
        else
            return sf::IntRect(static_cast<int>(m_sourceOffset.x),
                static_cast<int>(m_frame * m_frameHeight + m_sourceOffset.y), m_frameWidth, m_frameHeight);
}

void Animation::draw(const DrawParameter& param)
{
    if(!_renderStencilEffects && m_stencil.mode == StencilInfo::Test)
        return;

    if(isStopped())
        return;

    auto targetBuffer = &param.getTarget(m_targetBuffer != UINT_MAX ? m_targetBuffer : 0);

    if(m_scaleToScreenSize)
    {
        auto spriteSize = sf::Vector2i(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height);
        auto scaleFactor = sf::Vector2f(targetBuffer->getView().getSize().x / spriteSize.x * m_sprite.getScale().x,
                                        targetBuffer->getView().getSize().y / spriteSize.y * m_sprite.getScale().y);
        m_sprite.setScale(scaleFactor);
    }

    if(m_isViewAligned)
    {
        auto topLeft = targetBuffer->getView().getCenter() - 0.5f * targetBuffer->getView().getSize();
        setPosition(topLeft.x, topLeft.y);

        if(m_frameWidth < 0 || m_frameHeight < 0)
            m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(m_sourceOffset), sf::Vector2i(targetBuffer->getView().getSize())));
    }
    else if(!param.getScreenRect().intersects(m_sprite.getGlobalBounds()))
        return;

    if(Animation::_renderStencilEffects)
        m_stencil.enable();

    if(m_prepareTextureOnUse)
        param.prepareTexture(m_sprite.getTexture());

    if(m_shader)
        m_shader->prepare(param);

    targetBuffer->draw(m_sprite, Blending::toSfmlMode(m_blending));

    if(Animation::_renderStencilEffects)
        m_stencil.disable();

    if(m_shader)
        m_shader->unbind();
}

void Animation::bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
{
    if(x)
        m_xPositionProvider = std::move(x);
    if(y)
        m_yPositionProvider = std::move(y);
}

void Animation::bindRotationController(std::unique_ptr<ValueProvider> provider)
{
    if(provider)
        m_rotationProvider = std::move(provider);
}

void Animation::bindScaleController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
{
    if(x)
        m_xScaleProvider = std::move(x);
    if(y)
        m_yScaleProvider = std::move(y);
}

void Animation::bindColorController(std::unique_ptr<ValueProvider> red,
    std::unique_ptr<ValueProvider> green,
    std::unique_ptr<ValueProvider> blue,
    std::unique_ptr<ValueProvider> alpha)
{
    if(red)
        m_colorProviders[Red] = std::move(red);
    if(green)
        m_colorProviders[Green] = std::move(green);
    if(blue)
        m_colorProviders[Blue] = std::move(blue);
    if(alpha)
        m_colorProviders[Alpha] = std::move(alpha);
}

void Animation::setLayout(
    const std::vector<sf::Vector2i>& srcPositions,
    const std::vector<sf::Vector2i>& srcOffsets,
    const std::vector<sf::Vector2i>& sizes,
    const std::vector<sf::Vector2i>& origins)
{
    if(m_frames > srcPositions.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), "srcPositions"));

    if(m_frames > srcOffsets.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), "srcOffsets"));

    if(m_frames > sizes.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), "sizes"));

    if(m_frames > origins.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), "origins"));

    for(unsigned i = 0; i < m_frames; i++)
        m_sizes.push_back(sf::IntRect(srcPositions[i].x, srcPositions[i].y, sizes[i].x, sizes[i].y));
    m_origins = origins;
    m_offsets = srcOffsets;
}

std::unique_ptr<Animation> Animation::clone() const
{
    auto ani = std::unique_ptr<Animation>(new Animation(m_frames, m_frameWidth, m_frameHeight,
        m_applyRotation, m_sprite.getOrigin(), m_drawOffset, m_scaleToScreenSize, m_horizontal));

    if(m_cloneHandler != nullptr)
        m_cloneHandler->registerClone(*this, *ani.get(), *ani.get(), *ani.get());

    if(m_frameProvider)
        ani->bindFrameProvider(m_frameProvider->clone());

    ani->m_stopOnAlphaZero = m_stopOnAlphaZero;
    ani->m_sourceOffset = m_sourceOffset;
    ani->m_sprite = m_sprite;
    ani->m_blending = m_blending;
    ani->m_stencil = m_stencil;
    ani->m_targetBuffer = m_targetBuffer;
    ani->m_sizes = m_sizes;
    ani->m_offsets = m_offsets;
    ani->m_origins = m_origins;
    ani->m_shader = m_shader;
    ani->copyValuesFrom(*this);

    std::array<std::unique_ptr<ValueProvider>, 4> colors;
    for(int i = 0; i < 4; i++)
        if(m_colorProviders[i])
            ani->m_colorProviders[i] = m_colorProviders[i]->clone();

    if(m_xScaleProvider)
        ani->m_xScaleProvider = m_xScaleProvider->clone();
    if(m_yScaleProvider)
        ani->m_yScaleProvider = m_yScaleProvider->clone();

    if(m_rotationProvider)
        ani->m_rotationProvider = m_rotationProvider->clone();

    if(m_xPositionProvider)
        ani->m_xPositionProvider = m_xPositionProvider->clone();
    if(m_yPositionProvider)
        ani->m_yPositionProvider = m_yPositionProvider->clone();

    if(m_cloneHandler != nullptr)
        m_cloneHandler->unregisterClone(*this);

    return ani;
}

void Animation::setStopOnAlphaZero(bool stop)
{
    m_stopOnAlphaZero = stop;
}

void Animation::setBufferId(unsigned int id)
{
    m_targetBuffer = id;
}

unsigned int Animation::getBufferId()
{
    return m_targetBuffer;
}

void Animation::applyRotation(bool apply)
{
    m_applyRotation = apply;
}

void Animation::bindCloneHandler(CloneHandler& handler)
{
    m_cloneHandler = &handler;
}

void Animation::bindShader(Shader& shader)
{
    m_shader = &shader;
}

void Animation::alignToView(bool align)
{
    m_isViewAligned = align;
}

void Animation::enableStencilEffects(bool enable)
{
    _renderStencilEffects = enable;
    if(enable)
    {
        for(auto it = begin(_stencilAnimations); it != end(_stencilAnimations); ++it)
            (*it)->update();
    }
}

bool Animation::usesStencilEffects()
{
    return _renderStencilEffects;
}

void Animation::setStencilInfo(StencilInfo info)
{
    m_stencil = info;
    _stencilAnimations.remove(this);
    _stencilAnimations.push_back(this);
}

void StencilInfo::enable()
{
    if(mode == StencilInfo::Write)
    {
        gl::Enable(gl::STENCIL_TEST);
        gl::StencilFunc(gl::ALWAYS, ref, mask);//0xFF);
        gl::StencilOp(gl::KEEP, gl::REPLACE, gl::REPLACE);
        gl::StencilMask(ref);
        //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        gl::Enable(gl::ALPHA_TEST);
        gl::AlphaFunc(gl::GREATER, 0.0f);
    }
    if(mode == StencilInfo::Test)
    {
        gl::Enable(gl::STENCIL_TEST);
        gl::StencilFunc(gl::LESS, ref, mask);
        gl::StencilOp(gl::KEEP, gl::KEEP, gl::KEEP);
        gl::StencilMask(0x00);
    }
}

void StencilInfo::disable()
{
    if(mode != StencilInfo::None)
    {
        gl::Disable(gl::STENCIL_TEST);
        gl::Disable(gl::ALPHA_TEST);
        gl::ColorMask(gl::TRUE_, gl::TRUE_, gl::TRUE_, gl::TRUE_);
        gl::StencilMask(0x00);
    }
}