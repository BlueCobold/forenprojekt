#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Cloneable.hpp"
#include "VariableHolder.hpp"
#include "Stoppable.hpp"
#include "../rendering/Drawable.hpp"
class ValueProvider;
class Shader;

#include <array>
#include <list>
#include <memory> // unique_ptr
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class CloneHandler;

struct Blending
{
    enum Mode
    {
        RegularAlpha,
        PreMultipliedAlpha,
        Multiply,
        Add
    };
    
    static sf::BlendMode toSfmlMode(const Mode mode) 
    {
        switch (mode)
        {
            case RegularAlpha:
                return sf::BlendAlpha;
            case PreMultipliedAlpha:
                return sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha);
            case Multiply:
                return sf::BlendMultiply;
            case Add:
                return sf::BlendAdd;
        }
        return sf::BlendAlpha;
    }
};

struct StencilInfo
{
    enum StencilMode
    {
        None,
        Write,
        Test
    };
    
    StencilInfo(StencilMode mode = None, int ref = 0, int mask = 0) :
        mode(mode),
        ref(ref),
        mask(mask)
    { }
    
    void enable();
    void disable();
    
    StencilMode mode;
    int ref;
    int mask;
};

class Animation : public Drawable, public VariableHolder, public Stoppable, public Cloneable<Animation>
{
public:

    Animation(
        const unsigned int frames,
        const int frameWidth,
        const int frameHeight,
        const bool applyRotation,
        const sf::Vector2f& origin,
        const sf::Vector2f& drawOffset,
        const bool scaleToScreenSize,
        const bool horizontal = true);
    ~Animation();

    void update();
    void reset();

    void setStencilInfo(StencilInfo info);

    void setPosition(const float x, const float y);
    void setRotation(const float radians);
    void setBlending(const Blending::Mode mode);
    void bindTexture(const sf::Texture& texture, const sf::Vector2f& sourceOffset, bool prepareOnUsage = false);
    void bindFrameProvider(std::unique_ptr<ValueProvider> frames);
    void bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y);
    void bindRotationController(std::unique_ptr<ValueProvider> provider);
    void bindScaleController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y);
    void bindColorController(
        std::unique_ptr<ValueProvider> red,
        std::unique_ptr<ValueProvider> green,
        std::unique_ptr<ValueProvider> blue,
        std::unique_ptr<ValueProvider> alpha);
    void setLayout(
        const std::vector<sf::Vector2i>& srcPositions,
        const std::vector<sf::Vector2i>& srcOffsets,
        const std::vector<sf::Vector2i>& sizes,
        const std::vector<sf::Vector2i>& origins);
    void setStopOnAlphaZero(bool stop);
    void setBufferId(unsigned int id);
    unsigned int getBufferId();
    void applyRotation(bool apply);
    void bindCloneHandler(CloneHandler& handler);
    void alignToView(bool align);
    void bindShader(Shader& shader);

    virtual void draw(const DrawParameter& param) override;

    virtual std::unique_ptr<Animation> clone() const override;

    static void enableStencilEffects(bool enable);
    static bool usesStencilEffects();

private:

    enum ColorChannels
    {
        Red, Green, Blue, Alpha
    };

    static bool _renderStencilEffects;
    static std::list<Animation*> _stencilAnimations;

    void updatePosition();
    const sf::IntRect getTextureRect() const;

    std::unique_ptr<ValueProvider> m_xPositionProvider;
    std::unique_ptr<ValueProvider> m_yPositionProvider;
    std::unique_ptr<ValueProvider> m_xScaleProvider;
    std::unique_ptr<ValueProvider> m_yScaleProvider;
    std::unique_ptr<ValueProvider> m_rotationProvider;
    std::unique_ptr<ValueProvider> m_frameProvider;
    std::array<std::unique_ptr<ValueProvider>, 4> m_colorProviders;
    std::vector<sf::IntRect> m_sizes;
    std::vector<sf::Vector2i> m_offsets;
    std::vector<sf::Vector2i> m_origins;
    bool m_applyRotation;
    bool m_stopOnAlphaZero;
    StencilInfo m_stencil;
    unsigned int m_frames;
    unsigned int m_frame;
    int m_frameWidth;
    int m_frameHeight;
    bool m_horizontal;
    sf::Sprite m_sprite;
    bool m_prepareTextureOnUse;
    sf::Vector2f m_sourceOffset;
    sf::Vector2f m_externalPosition;
    sf::Vector2f m_dynamicPosition;
    float m_externalRotation;
    sf::Vector2f m_drawOffset;
    Blending::Mode m_blending;
    unsigned int m_targetBuffer;
    bool m_isViewAligned;
    Shader* m_shader;
    CloneHandler* m_cloneHandler;
    bool m_scaleToScreenSize;
};

#endif // ANIMATION_HPP
