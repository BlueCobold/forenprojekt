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
    if (targetTexture != nullptr)
    {
        m_rectWidth = targetTexture->getSize().x / rows;
        m_rectHeigth = targetTexture->getSize().y / columns;

        int spriteCount = columns * rows;

        int column = 0;
        int row = 0;

        for(int i = 0; i < spriteCount; ++i)
        {
            column = i / rows;
            row = i % columns;
            auto sprite = new sf::Sprite(*targetTexture, sf::IntRect(row * m_rectWidth + m_rectWidth / 2, column * m_rectHeigth + m_rectHeigth / 2, 0, 0));
            sprite->setOrigin(30, 30);
            sprite->setPosition(row * m_rectWidth + m_rectWidth / 2.f -30, column * m_rectHeigth + m_rectHeigth / 2.f-30);
            m_targetSprites.push_back(std::unique_ptr<sf::Sprite>(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "RotatingSquaresTransition"));

    if (sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void RotatingSquaresTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int spriteCount = 0;

    int column = 0;
    int row = 0;

    float centreX = 0;
    float centreY = 0;

    float sizeX = scale * m_rectWidth / 2.f;
    float sizeY = scale * m_rectHeigth / 2.f;

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {

        column = spriteCount / m_rows;
        row = spriteCount % m_columns;

        centreX = row * m_rectWidth + m_rectWidth / 2.f;
        centreY = column * m_rectHeigth + m_rectHeigth / 2.f;

        auto rect = (*it)->getTextureRect();

        rect.left = static_cast<int>(centreX - sizeX);
        rect.top =  static_cast<int>(centreY - sizeY);
        rect.width = static_cast<int>(sizeX * 2);
        rect.height = static_cast<int>(sizeY * 2);

        (*it)->setPosition(centreX ,centreY );
        (*it)->setOrigin(sizeX, sizeY);

        (*it)->setTextureRect(rect);
        (*it)->setRotation(270.f + 90.f * scale);

        spriteCount++;
    }
}

void RotatingSquaresTransition::draw(const DrawParameter& param)
{
    if (getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if (getTargetTexture() != nullptr)
    {
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
        {
            param.getTarget().draw(**it);
        }
    }
}