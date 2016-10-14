#include "TargetHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager,
                     const sf::Vector2f& position,
                     const sf::Vector2f& offset,
                     const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont),
    m_totalTarget(0),
    m_remainingTarget(0),
    m_resourceManager(resourceManager)
{
    setAlignment(LineLabel::Right);
    setText(utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
    m_targetTexture.setTexture(*m_resourceManager.getTexture("targetmini"));
}

void TargetHUD::setTargets(int totalTarget,int remainingTarget)
{
    m_totalTarget = totalTarget;
    m_remainingTarget = remainingTarget;
}

void TargetHUD::updated(const DrawParameter& params)
{
    LabelHUD::updated(params);

    setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));

    m_targetTexture.setPosition(getCurrentPosition().x
        - getLabelWidth() - m_targetTexture.getTextureRect().width,
        getCurrentPosition().y);
}

void TargetHUD::doDraw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   LabelHUD::doDraw(params);
}
