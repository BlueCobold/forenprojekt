#include "ScrollView.hpp"

ScrollView::ScrollView()
{
}

ScrollView::ScrollView(const sf::Vector2u& levelsize, const sf::Vector2u& viewsize)
	: m_viewSize(viewsize), m_levelSize(levelsize)
{
}

void ScrollView::adjustView(const sf::Vector2f& scrollvec, sf::RenderTarget& window)
{
	sf::View view = window.getView();
	sf::Vector2f center = scrollvec;
	
	if(m_levelSize.x < m_viewSize.x)
		center.x = m_viewSize.x / 2.0f;
	else
		if(center.x < (m_viewSize.x / 2))
			center.x = m_viewSize.x / 2.0f;
		else if(center.x > m_levelSize.x - m_viewSize.x / 2.0f)
			center.x = m_levelSize.x - m_viewSize.x / 2.0f;

	if(m_levelSize.y < m_viewSize.y)
		center.y = m_viewSize.y / 2.0f;
	else
		if(center.y < (m_viewSize.y / 2))
			center.y = m_viewSize.y / 2.0f;
		else if(center.y > m_levelSize.y - m_viewSize.y / 2.0f)
			center.y = m_levelSize.y - m_viewSize.y / 2.0f;

    m_viewCenter = center;
	view.setCenter(center);
	window.setView(view);
}

void ScrollView::setViewSize(const sf::Vector2u& v)
{
	m_viewSize = v;
}

void ScrollView::setLevelSize(const sf::Vector2u& v)
{
	m_levelSize = v;
}

sf::Vector2f ScrollView::toGlobalCoords(const sf::Vector2u& v)
{
    sf::Vector2f screenCoords;

    screenCoords.x = m_viewCenter.x - m_viewSize.x / 2 + v.x;
    screenCoords.y = m_viewCenter.y - m_viewSize.y / 2 + v.y;
    
    return screenCoords;
}

unsigned int ScrollView::getGlobalRightCorner()
{
    return m_viewCenter.x + m_viewSize.x / 2;
}
unsigned int ScrollView::getGlobalTopCorner()
{
    return m_viewCenter.y - m_viewSize.y / 2;
}
unsigned int ScrollView::getGloablLeftCorner()
{
    return m_viewCenter.x - m_viewSize.x / 2;
}
unsigned int ScrollView::getGlobalBottomCorner()
{
    return m_viewCenter.y + m_viewSize.y / 2;
}