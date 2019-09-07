#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_PlayableSpriteObject.h"

class SFML_HUD : public sf::Drawable
{
public:
	SFML_HUD(SFML_PlayableSpriteObject& playerObject, int windowWidth, int windowHeight);
	~SFML_HUD();

	void update(int windowHeight);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture								m_hudTexture;
	sf::Sprite								m_hudSprite;

	std::vector<sf::Texture>				m_WeaponTextures;
	sf::Sprite								m_WeaponSprite;

	sf::Texture								m_soldierTexture;
	sf::Sprite								m_soldierSprite;

	sf::Vector2u							m_spriteTextureSize;

	SFML_PlayableSpriteObject&				m_player;

	sf::Texture								m_playerHealthTexture;
	std::vector<sf::Sprite>					m_playerHealthSprite;

	sf::Texture								m_bulletTexture;
	std::vector <sf::Sprite>				m_bulletSprite;
};