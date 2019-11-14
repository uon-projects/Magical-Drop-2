//In The "Header" File Go All The Declarations As Well As All The Implementations of The Class' Methods...
#include <SFML/Graphics.hpp>
#include <string>
#include <list>


namespace zeoFlow
{
	class ZeoFlow_SFML
	{
		std::list<sf::Texture> zfTextures;

	public:
		sf::Sprite loadSpriteFromTexture(std::string, std::string, std::string);

	public:
		sf::Sprite formatSpriteForBackground(sf::Sprite);

	public:
		sf::Sprite spriteInMiddle(sf::Sprite, double, double);

	};

	sf::Sprite ZeoFlow_SFML::loadSpriteFromTexture(std::string assetPath, std::string assetName, std::string assetExtension)
	{
		sf::Texture zfTexture;
		if (!zfTexture.loadFromFile(assetPath + assetName + "." + assetExtension)) {
				system("pause");
		}
		zfTextures.push_front(zfTexture);
		sf::Sprite zfSprite(zfTextures.front());
		return zfSprite;
	}

	sf::Sprite ZeoFlow_SFML::formatSpriteForBackground(sf::Sprite assetSprite)
	{
		float BackgroundCurrentScaleX = 1.0f,BackgroundCurrentScaleY = 1.0f;
		float BackgroundWidth,BackgroundHeight;
		sf::Vector2f BackgroundLocation(0,0);
		assetSprite.setScale(sf::Vector2f(BackgroundCurrentScaleX, BackgroundCurrentScaleY));
		BackgroundWidth = assetSprite.getGlobalBounds().width;
		BackgroundHeight = assetSprite.getGlobalBounds().height;
		assetSprite.setPosition(BackgroundLocation.x, BackgroundLocation.y);
		return assetSprite;
	}

	sf::Sprite ZeoFlow_SFML::spriteInMiddle(sf::Sprite assetSprite, double windowWidth, double windowHeight)
	{
		double windowHeightHalf = windowHeight / 2;
		double windowWidthHalf = windowWidth / 2;
		double spriteHeightHalf = assetSprite.getGlobalBounds().width / 2;
		double spriteWidthHalf = assetSprite.getGlobalBounds().width / 2;
		assetSprite.setPosition(windowWidthHalf - spriteWidthHalf, windowHeightHalf - spriteHeightHalf);
		return assetSprite;
	}
}