#include "SFML_GameMenu.h"

SFML_GameMenu::SFML_GameMenu(std::string menufilename, int windowWidth, int windowHeight)
	: SFML_MenuSystem(menufilename, 1.5f)
{
	LoadBackgroundImage("Media/Textures/Zombie_Game_Menu.jpg");

	//Load the texture file and set it to the sprite
	m_logo.loadFromFile("Media/Textures/Title_Logo_1000px.png");
	m_logoSprite.setTexture(m_logo);
	m_logoSprite.setScale(0.75f, 0.75f);
	
	//Set the origin, texture rectangle and position of the sprite
	sf::Vector2u logoTextureSize = m_logo.getSize();
	m_logoSprite.setOrigin(logoTextureSize.x / 2, logoTextureSize.y / 2);
	m_logoSprite.setTextureRect(sf::IntRect(0, 0, logoTextureSize.x, logoTextureSize.y));
	m_logoSprite.setPosition(0, m_backgroundTexture.getSize().y / 2 * 0.9f); //set the position of the logo 90% down the screen height.

	SetCharacterSize(50);
}

SFML_GameMenu::~SFML_GameMenu()
{
	
}

void SFML_GameMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_backgroundSprite, states);
	target.draw(m_logoSprite, states);

	SFML_MenuSystem::draw(target, states);
}



void SFML_GameMenu::LoadBackgroundImage(const std::string& filepath)
{
	//Load the texture file and set it to the sprite
	m_backgroundTexture.loadFromFile(filepath);
	m_backgroundSprite.setTexture(m_backgroundTexture);

	//Set the origin, texture rectangle, and position of the sprite
	sf::Vector2u textureSize = m_backgroundTexture.getSize();
	m_backgroundSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	m_backgroundSprite.setPosition(0, 0);
	//Note here that we set the origin of the texture center but place it to the top left-corner of the screen. We are going to use the sf::transformable
	//class properly later to shift  this to the center of the screen.

	//Set the rectangle texture which is centered at 0,0
	m_backgroundRect.left = -static_cast<float> (textureSize.x / 2);
	m_backgroundRect.top = -static_cast<float> (textureSize.y / 2);
	m_backgroundRect.width = textureSize.x;
	m_backgroundRect.height = textureSize.y;
}