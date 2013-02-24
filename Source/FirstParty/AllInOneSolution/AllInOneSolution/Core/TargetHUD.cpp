#include "TargetHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
HorizontalReference hReference, VerticalReference vReference) :
HUDElement(resourceManager,position,rotation,bitmapFont, hReference, vReference),
m_remainingTarget(0),
m_totalTarget(0)
{
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
    HUDElement::update(params);

    m_targetTexture.setPosition(m_label.getPosition().x -22 ,m_label.getPosition().y);

    m_label.setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
}

void TargetHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   HUDElement::draw(params);
}