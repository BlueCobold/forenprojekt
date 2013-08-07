#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Stoppable.hpp"
#include "provider/ValueProvider.hpp"
#include "../rendering/Drawable.hpp"

#include <memory> // unique_ptr

#include "VariableHolder.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation : public Drawable, public VariableHolder, public Stoppable
{
public:

    Animation(std::unique_ptr<ValueProvider> provider,
        const unsigned int frames,
        const unsigned int frameWidth,
        const unsigned int frameHeight,
        const bool applyRotation,
        const sf::Vector2f& origin,
        const sf::Vector2f& drawOffset,
        const bool horizontal = true);
    ~Animation();

    void update();
    void reset();

    void setPosition(const float x, const float y);
    void setRotation(const float radians);
    void setBlending(const sf::BlendMode mode);
    void bindTexture(const sf::Texture& texture, const sf::Vector2f& sourceOffset);
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
        const std::vector<sf::Vector2i>& srcOffsets,
        const std::vector<sf::Vector2i>& sizes,
        const std::vector<sf::Vector2i>& origins);
    
    virtual void draw(const DrawParameter& param) override;

private:

    enum ColorChannels
    {
        Red, Green, Blue, Alpha
    };

    void updatePosition();
    const sf::IntRect Animation::getTextureRect() const;

    std::unique_ptr<ValueProvider> m_xPositionProvider;
    std::unique_ptr<ValueProvider> m_yPositionProvider;
    std::unique_ptr<ValueProvider> m_xScaleProvider;
    std::unique_ptr<ValueProvider> m_yScaleProvider;
    std::unique_ptr<ValueProvider> m_rotationProvider;
    std::unique_ptr<ValueProvider> m_frameProvider;
    std::unique_ptr<ValueProvider> m_colorProviders[4];
    std::vector<sf::Vector2i> m_srcOffsets;
    std::vector<sf::Vector2i> m_sizes;
    std::vector<sf::Vector2i> m_origins;
    bool m_applyRotation;
    unsigned int m_frames;
    unsigned int m_frame;
    unsigned int m_frameWidth;
    unsigned int m_frameHeight;
    bool m_horizontal;
    sf::Sprite m_sprite;
    sf::Vector2f m_sourceOffset;
    sf::Vector2f m_externalPosition;
    sf::Vector2f m_dynamicPosition;
    float m_externalRotation;
    sf::Vector2f m_drawOffset;
    sf::BlendMode m_blending;
};

#endif // ANIMATION_HPP
