
#include "BackgroundParser.hpp"

std::unique_ptr<Background> BackgroundParser::parse(const tinyxml2::XMLElement& xml, const sf::Vector2u& size, unsigned int defaultTargetBuffer)
{
    std::unique_ptr<Background> background(new Background(size));

    for(auto parallax = xml.FirstChildElement("parallax"); parallax != nullptr;
        parallax = parallax->NextSiblingElement("parallax"))
    {
        sf::Vector2f layerSize;
        parallax->QueryFloatAttribute("width", &layerSize.x);
        parallax->QueryFloatAttribute("height", &layerSize.y);
        std::unique_ptr<ParallaxLayer> layer(new ParallaxLayer(layerSize));
            
        auto context = ProviderContext(nullptr, layer.get(), layer.get(), layer.get(), m_context.cloneHandler)
                                            .withFunctions(m_context.functions);
        AnimationParser loader(context, m_resourceManager, defaultTargetBuffer);
        auto animations = loader.parseMultiple(*parallax);
        for(auto ani = begin(animations); ani != end(animations); ++ani)
            layer->bindAnimation(std::move(*ani));

        background->bindLayer(std::move(layer));
    }

    return background;
}
