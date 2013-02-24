#include "TargetHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
HorizontalReference hReference, VerticalReference vReference) :
HUDElement(resourceManager,position,rotation,bitmapFont, hReference, vReference),
m_remainingTarget(0),
m_totalTarget(0)
{
    setText(utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
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

    switch(m_aligend)
    {
        case LeftAligend:
            m_targetTexture.setPosition(getPosition().x - m_targetTexture.getTextureRect().width , getPosition().y);
        break;

        case CenterAligend:
            m_targetTexture.setPosition(getPosition().x - m_targetTexture.getTextureRect().width - getLength() / 2, getPosition().y);
        break;

        case RightAligend:
            m_targetTexture.setPosition(getPosition().x - m_targetTexture.getTextureRect().width - getLength() , getPosition().y);
        break;

        default:
        break;
    };

    setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
}

void TargetHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   HUDElement::draw(params);
}