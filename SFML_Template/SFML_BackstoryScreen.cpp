#include "SFML_BackstoryScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_BackstoryScreen::SFML_BackstoryScreen(int windowWidth, int windowHeight)
: SFML_MenuSystem("Media/MenuInfo/backstory_menu.txt", 1.0f)
, m_alphaChannel(0)
{
	LoadBackgroundImage("Media/Textures/Backstory_City.jpg");

	SetCharacterSize(23);
	SetPosition(windowWidth / 2 - GetHalfWidth(), windowHeight / 2 - GetHalfHeight());

	for (unsigned char i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].setOutlineThickness(1.0f);
	}
}


SFML_BackstoryScreen::~SFML_BackstoryScreen()
{
}

int SFML_BackstoryScreen::update(sf::Time elapsedTime)
{
	//local variables for algorithm
	float fadeTime = 5.0f;
	float startTime = 1.0f;
	float fadeInTime = startTime + fadeTime; //Number on time line when text should fade in
	float fadeOutTime = 6.0f + fadeInTime; //Number on time line when text should fade out
	float exitToMain = fadeOutTime + fadeTime; //Number on time line when app should switch to main menu

	if (m_accumulatedTime > 0.0f && m_accumulatedTime < startTime)
		m_state = E_WAIT;
	else if (m_accumulatedTime > startTime && m_accumulatedTime < fadeInTime)
		m_state = E_FADEIN;
	else if (m_accumulatedTime > fadeInTime && m_accumulatedTime < fadeOutTime)
		m_state = E_WAIT;
	else if (m_accumulatedTime > fadeOutTime && m_accumulatedTime < exitToMain)
		m_state = E_FADEOUT;
	else if (m_accumulatedTime > exitToMain)
		return 0;
	
	switch (m_state)
	{
	case E_FADEIN:
		//Fade text onto screen
		for (unsigned char i = 0; i < m_menuText.size(); i++)
		{
			if (m_alphaChannel <= 255.0f)
			{
				m_menuText[i].setFillColor(sf::Color(64, 64, 64, static_cast<unsigned char>(m_alphaChannel)));
				m_menuText[i].setOutlineColor(sf::Color(255, 255, 255, static_cast<unsigned char>(m_alphaChannel)));
			}
		}
		m_alphaChannel += 255.0f * (elapsedTime.asSeconds() / fadeTime);
		break;

	case E_FADEOUT:
		//Fade text off screen
		for (unsigned char i = 0; i < m_menuText.size(); i++)
		{
			if (m_alphaChannel >= 0)
			{
				m_menuText[i].setFillColor(sf::Color(64, 64, 64, static_cast<unsigned char>(m_alphaChannel)));
				m_menuText[i].setOutlineColor(sf::Color(255, 255, 255, static_cast<unsigned char>(m_alphaChannel)));
			}
		}

		m_alphaChannel -= 255.0f * (elapsedTime.asSeconds() / fadeTime);
		break;
		
	case E_WAIT:
		//Do nothing
		break;
	}



	m_accumulatedTime += elapsedTime.asSeconds();
	return -1;
}

void SFML_BackstoryScreen::ResetVariables()
{
	m_alphaChannel = 0;
	m_accumulatedTime = 0;
}

void SFML_BackstoryScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_backgroundSprite);

	SFML_MenuSystem::draw(target, states);
}

void SFML_BackstoryScreen::LoadBackgroundImage(const std::string& filepath)
{
	//Load the texture file and set it to the sprite
	m_backgroundTexture.loadFromFile(filepath);

	m_backgroundSprite.setTexture(m_backgroundTexture);

	//Set the origin, texture rectangle, and position of the sprite
	sf::Vector2u textureSize = m_backgroundTexture.getSize();
	m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	m_backgroundSprite.setPosition(0, 0);
}
