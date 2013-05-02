#include "BallHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

BallHUD::BallHUD(ResourceManager& resourceManager, 
                    const sf::Vector2f& position, 
                    const float rotation, 
                    BitmapFont* bitmapFont,
                    const float hReference,
                    const float vReference) :
    LabelHUD(position, rotation, bitmapFont, hReference, vReference),
    m_remainingBall(0),
    m_resourceManager(resourceManager)
{
    m_label.setAlignment(LineLabel::Right);
    m_label.setText(" " + utility::toString<int>(m_remainingBall));
    m_ballTexture.setTexture(*m_resourceManager.getTexture("ballmini"));
}

void BallHUD::setBalls(int remainingBall)
{
    m_remainingBall = remainingBall;
}

void BallHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_label.setText(" " + utility::toString<int>(m_remainingBall));

    m_ballTexture.setPosition(m_label.getPosition().x
        - m_label.getWidth() - m_ballTexture.getTextureRect().width,
        m_label.getPosition().y);
}

void BallHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_ballTexture);
   LabelHUD::draw(params);
}
