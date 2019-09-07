#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_MenuSystem.h"

class SFML_GameMenu : public SFML_MenuSystem
{
public:
	SFML_GameMenu(std::string menufilename, int windowWidth, int windowHeight);
	~SFML_GameMenu();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void LoadBackgroundImage(const std::string& filepath);

	enum MENU_ALIGNMENT
	{
		E_LEFT_JUSTIFIED,
		E_CENTRE_JUSTIFIED,
		E_RIGHT_JUSTIFIED
	};

	MENU_ALIGNMENT	m_menuAlignment;

	sf::Texture		m_logo;
	sf::Sprite		m_logoSprite;

	sf::Texture		m_backgroundTexture;
	sf::Sprite		m_backgroundSprite;
	sf::FloatRect	m_backgroundRect;
};