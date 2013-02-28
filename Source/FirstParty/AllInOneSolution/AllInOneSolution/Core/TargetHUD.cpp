#include "TargetHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

TargetHUD::TargetHUD(ResourceManager& resourceManager, 
				     const sf::Vector2f& position, 
					 const float rotation, 
					 std::string bitmapFont,
					 float hReference, float vReference) :
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
    LineLabel temp;

    temp.setBitmapFont(*m_bitmapfont);
    temp.setText(" " + utility::toString<int>(m_totalTarget) + "/" + utility::toString<int>(m_totalTarget));

    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();
    
	sf::Vector2f topLeftViewStart = screenCenter - screenSize * 0.5f;
	sf::Vector2f bottomRightViewEnd = screenCenter + screenSize * 0.5f;
	sf::Vector2f delta = bottomRightViewEnd - topLeftViewStart;

	sf::Vector2f position;
    sf::Vector2f positionTarget;

    switch (m_aligned)
    {
        case AlignedRight:
            position.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x - m_label.getLength();
	        position.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;
            temp.setPosition(position);
            positionTarget.x = temp.getPosition().x - 22 + m_label.getLength() - temp.getLength();
            positionTarget.y = temp.getPosition().y;
        break;

        case AlignedCenter:
            position.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x - m_label.getLength() / 2.f;
	        position.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;
            temp.setPosition(position);
            positionTarget.x = temp.getPosition().x - 22 + (m_label.getLength() - temp.getLength()) / 2.f;
            positionTarget.y = temp.getPosition().y;
        break;

        case AlignedLeft:
            position.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x;
	        position.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;
            temp.setPosition(position);
            positionTarget.x = temp.getPosition().x - 22;
            positionTarget.y = temp.getPosition().y;
        break;

        default:
            position.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x;
	        position.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;
            temp.setPosition(position);
            positionTarget.x = temp.getPosition().x - 22;
            positionTarget.y = temp.getPosition().y;
        break;
    }

    m_label.setPosition(position);

    m_targetTexture.setPosition(positionTarget);

    m_label.setText(" " + utility::toString<int>(m_remainingTarget) + "/" + utility::toString<int>(m_totalTarget));
}

void TargetHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_targetTexture);
   HUDElement::draw(params);
}