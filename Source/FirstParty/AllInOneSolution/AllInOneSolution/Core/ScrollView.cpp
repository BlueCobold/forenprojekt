#include "ScrollView.hpp"

ScrollView::ScrollView()
{
}

ScrollView::ScrollView(const sf::Vector2u& levelsize, const sf::Vector2f& viewsize)
	: m_viewSize(viewsize), m_levelSize(levelsize), m_zoomFactor(1.0f)
{
}

void ScrollView::adjustView(const sf::Vector2f& scrollvec, sf::RenderTarget& window)
{
	sf::View view = window.getView();
    m_viewSize = sf::Vector2f(view.getSize()) * m_zoomFactor;
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
    view.setSize(m_viewSize);
	window.setView(view);
}

void ScrollView::setLevelSize(const sf::Vector2f& v)
{
	m_levelSize = v;
}

void ScrollView::setZoomFactor(const float factor)
{
    m_zoomFactor = factor;
}

sf::Vector2f ScrollView::toGlobalCoords(const sf::Vector2f& v)
{
    sf::Vector2f screenCoords;

    screenCoords.x = m_viewCenter.x - m_viewSize.x / 2 + v.x;
    screenCoords.y = m_viewCenter.y - m_viewSize.y / 2 + v.y;
    
    return screenCoords;
}

float ScrollView::getGlobalRightCorner()
{
    return m_viewCenter.x + m_viewSize.x / 2;
}
float ScrollView::getGlobalTopCorner()
{
    return m_viewCenter.y - m_viewSize.y / 2;
}
float ScrollView::getGloablLeftCorner()
{
    return m_viewCenter.x - m_viewSize.x / 2;
}
float ScrollView::getGlobalBottomCorner()
{
    return m_viewCenter.y + m_viewSize.y / 2;
}