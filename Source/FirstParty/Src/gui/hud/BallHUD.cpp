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
    m_ball.setTexture(*m_resourceManager.getTexture("guiMisc"));
    m_ball.setTextureRect(sf::IntRect(0, 0, 22, 22));
}

void BallHUD::setBalls(int remainingBall)
{
    m_remainingBall = remainingBall;
}

void BallHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_label.setText(" " + utility::toString<int>(m_remainingBall));

    m_ball.setPosition(m_label.getPosition().x
        - m_label.getWidth() - m_ball.getTextureRect().width,
        m_label.getPosition().y);
}

void BallHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_ball);
   LabelHUD::draw(params);
}
