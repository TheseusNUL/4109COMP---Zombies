#include "SFML_TitleScreen.h"

SFML_TitleScreen::SFML_TitleScreen(std::string texturefilename, int windowWidth, int windowHeight)
{
	//Load image from texturefilename to m_titleScreenTexture
	m_titleScreenTexture.loadFromFile(texturefilename);

	//Assign the texture to m_titleScreenSprite
	m_titleScreenSprite.setTexture(m_titleScreenTexture);

	//Set the texture rectangle to cover the entire texture area
	sf::Vector2u bkTextureSize = m_titleScreenTexture.getSize();
	m_titleScreenSprite.setTextureRect(sf::IntRect(0, 0, bkTextureSize.x, bkTextureSize.y));

	//Set sprite position to top left corner
	m_titleScreenSprite.setPosition(0, 0);

	//Set the message string attributes
	m_messageFont.loadFromFile("Media/Fonts/DK_Nightbird.ttf");
	m_messageString.setString("Press Any Key to Continue");
	m_messageString.setFillColor(sf::Color(64, 64, 64));
	m_messageString.setOutlineColor(sf::Color(0, 0, 0));
	m_messageString.setOutlineThickness(1.0f);
	m_messageString.setFont(m_messageFont);
	m_messageString.setCharacterSize(100);

	//Set message string position
	sf::FloatRect textbox = m_messageString.getLocalBounds();
	m_messageString.setOrigin(textbox.width / 2, textbox.height / 2);
	m_messageString.setPosition(windowWidth * 0.5f, windowHeight * 0.9);
	

	//Set the parameters to control the flashing message sprite
	m_timeSinceLastFlash = sf::Time::Zero;
	m_flashingTime = sf::seconds(0.2f);

	//Cast background values to floats and scale appropriately
	float scaleX = static_cast<float>(windowWidth) / static_cast<float>(m_titleScreenTexture.getSize().x);
	float scaleY = static_cast<float>(windowHeight) / static_cast<float>(m_titleScreenTexture.getSize().y);
	m_titleScreenSprite.setScale(scaleX, scaleY);
}


SFML_TitleScreen::~SFML_TitleScreen()
{
}

void SFML_TitleScreen::update(sf::Time elapsedTime)
{
	//Change the tint of the message sprite from red to white every 0.2 seconds
	m_timeSinceLastFlash += elapsedTime;

	if (m_timeSinceLastFlash > m_flashingTime)
	{
		m_flashOn = !m_flashOn;
		m_timeSinceLastFlash -= m_flashingTime;

		if (m_flashOn)
		{
			m_messageString.setFillColor(sf::Color(128, 128, 128));
			m_messageString.setOutlineColor(sf::Color(96, 96, 96));
		}
		else
		{
			m_messageString.setFillColor(sf::Color(64, 64, 64));
			m_messageString.setOutlineColor(sf::Color(0, 0, 0));
		}
				

	}
}

void SFML_TitleScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Draw m_titleScreenSprite and m_messageString in that order
	target.draw(m_titleScreenSprite);
	target.draw(m_messageString);
}
