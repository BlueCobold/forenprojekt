#include "RotatingSquaresTransition.hpp"
#include "../../Utility.hpp"

RotatingSquaresTransition::RotatingSquaresTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int columns,
    const int rows,
    const float duration) :
    m_columns(columns),
    m_rows(rows),
    Transition(sourceTexture, targetTexture, duration)
{
    if(targetTexture != nullptr)
    {
        m_rectWidth = targetTexture->getSize().x / columns;
        if(static_cast<unsigned int>(m_rectWidth * columns) < targetTexture->getSize().x)
            m_rectWidth++;

        m_rectHeigth = targetTexture->getSize().y / rows;
        if(static_cast<unsigned int>(m_rectHeigth * rows) < targetTexture->getSize().y)
            m_rectHeigth++;

        int spriteCount = columns * rows;

        int column = 0;
        int row = 0;

        for(int i = 0; i < spriteCount; ++i)
        {
            column = i % columns;
            row = i / columns;
            auto sprite = new sf::Sprite(*targetTexture, sf::IntRect(column * m_rectWidth, row * m_rectHeigth, m_rectWidth, m_rectHeigth));
            sprite->setOrigin(m_rectWidth / 2.f,m_rectHeigth / 2.f);
            sprite->setPosition(column * m_rectWidth + m_rectWidth / 2.f, row * m_rectHeigth + m_rectHeigth / 2.f);
            m_targetSprites.push_back(std::unique_ptr<sf::Sprite>(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "RotatingSquaresTransition"));

    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void RotatingSquaresTransition::update()
{
    Transition::update();

    float scale = getProgress();

    /*int spriteCount = 0;

    int column = 0;
    int row = 0;

    float centreX = 0;
    float centreY = 0;

    float sizeX = scale * m_rectWidth / 2.f;
    float sizeY = scale * m_rectHeigth / 2.f;*/

    sf::Uint8 alpha = static_cast<sf::Uint8>(255 * scale);

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {

        /*column = spriteCount % m_columns;
        row = spriteCount / m_columns;

        centreX = column * m_rectWidth + m_rectWidth / 2.f;
        centreY = row * m_rectHeigth + m_rectHeigth / 2.f;

        auto rect = (*it)->getTextureRect();

        rect.left = static_cast<int>(centreX - sizeX);
        rect.top =  static_cast<int>(centreY - sizeY);
        rect.width = static_cast<int>(sizeX * 2);
        rect.height = static_cast<int>(sizeY * 2);

        (*it)->setPosition(centreX ,centreY );
        (*it)->setOrigin(sizeX, sizeY);

        (*it)->setTextureRect(rect);

        spriteCount++;*/

        (*it)->setColor(sf::Color(255, 255, 255, alpha));
        (*it)->setRotation(270.f + 90.f * scale);
    }
}

void RotatingSquaresTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}