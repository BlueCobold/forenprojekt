#include "BallHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

BallHUD::BallHUD(ResourceManager& resourceManager,
                 const sf::Vector2f& position,
                 const sf::Vector2f& offset,
                 const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont),
    m_remainingBall(0),
    m_resourceManager(resourceManager)
{
    setAlignment(LineLabel::Right);
    setText(" " + utility::toString<int>(m_remainingBall));
    m_ball.setTexture(*m_resourceManager.getTexture("guiMisc"));
    m_ball.setTextureRect(sf::IntRect(0, 0, 22, 22));
}

void BallHUD::setBalls(int remainingBall)
{
    m_remainingBall = remainingBall;
}

void BallHUD::updated(const DrawParameter& params)
{
    setText(" " + utility::toString<int>(m_remainingBall));
    LabelHUD::updated(params);

    m_ball.setPosition(getCurrentPosition().x
                       - getLabelWidth() - m_ball.getTextureRect().width,
                       getCurrentPosition().y);
}

void BallHUD::doDraw(const DrawParameter& params)
{
   params.getTarget().draw(m_ball);
   LabelHUD::doDraw(params);
}
