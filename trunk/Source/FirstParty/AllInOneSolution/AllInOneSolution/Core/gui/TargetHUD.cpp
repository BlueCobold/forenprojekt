#include "TargetHUD.hpp"
#include "../resources/ResourceManager.hpp"
#include "../Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager, 
                    const sf::Vector2f& position, 
                    const float rotation, 
                    BitmapFont* bitmapFont,
                    const float hReference,
                    const float vReference) :
    LabelHUD(position, rotation, bitmapFont, hReference, vReference),
    m_remainingTarget(0),
    m_totalTarget(0),
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

    m_targetTexture.setPosition(m_label.getPosition().x
        - m_label.getWidth() - m_targetTexture.getTextureRect().width,
        m_label.getPosition().y);
}

void TargetHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   LabelHUD::draw(params);
}
