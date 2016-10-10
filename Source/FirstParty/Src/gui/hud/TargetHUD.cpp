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
    m_label.setAlignment(LineLabel::Right);
    m_label.setText(utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
    m_targetTexture.setTexture(*m_resourceManager.getTexture("targetmini"));
}

void TargetHUD::setTargets(int totalTarget,int remainingTarget)
{
    m_totalTarget = totalTarget;
    m_remainingTarget = remainingTarget;
}

void TargetHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_label.setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));

    m_targetTexture.setPosition(getCurrentPosition().x
        - m_label.getWidth() - m_targetTexture.getTextureRect().width,
        getCurrentPosition().y);
}

void TargetHUD::doDraw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   LabelHUD::doDraw(params);
}
