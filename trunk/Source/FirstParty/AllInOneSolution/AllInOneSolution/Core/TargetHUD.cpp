#include "TargetHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont) :
HUDElement(resourceManager,position,rotation,bitmapFont),
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
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    m_label.setPosition(screenCenter.x - screenSize.x / 2 + m_position.x + m_targetTexture.getTextureRect().width, screenCenter.y - screenSize.y / 2 + m_position.y);

    m_targetTexture.setPosition(screenCenter.x - screenSize.x / 2 + m_position.x , screenCenter.y - screenSize.y / 2 + m_position.y);

    m_label.setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
}

void TargetHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   HUDElement::draw(params);
}