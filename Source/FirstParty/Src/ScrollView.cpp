#include "ScrollView.hpp"

#include <algorithm>

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
    int width = static_cast<int>(view.getSize().x);
    int height = static_cast<int>(view.getSize().y);
    m_viewSize = sf::Vector2f(view.getSize()) * m_zoomFactor;
    // calculate a point that will result in a perfect integer-position
    // of the upper left corner of the view
    sf::Vector2f center = sf::Vector2f(floorf(scrollvec.x) + (width & 1) / 2.0f,
                                       floorf(scrollvec.y) + (height & 1) / 2.0f);
	
	if(m_levelSize.x < m_viewSize.x)
		center.x = m_viewSize.x / 2.0f;
	else
        center.x = std::max(std::min(center.x, m_levelSize.x - m_viewSize.x / 2.0f),
                            m_viewSize.x / 2.0f);

	if(m_levelSize.y < m_viewSize.y)
		center.y = m_viewSize.y / 2.0f;
	else
        center.y = std::max(std::min(center.y, m_levelSize.y - m_viewSize.y / 2.0f),
                            m_viewSize.y / 2.0f);

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
