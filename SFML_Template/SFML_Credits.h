#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class SFML_Credits : public sf::Drawable
{
public:
	SFML_Credits(int windowWidth, int windowHeight);
	~SFML_Credits();

	int update(sf::Time elapsedTime);

	void PositionMenuItems();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void LoadBackgroundImage(const std::string& filepath);

	void LoadNames(const std::string& filePath);

	bool ParseTextFile(const std::string& filePath);

	void ParseRGBValues(const std::string& line, sf::Color& colour);


	sf::Texture					m_backgroundTexture;
	sf::Sprite					m_backgroundSprite;
	sf::FloatRect				m_backgroundRect;

	sf::Texture					m_logo;
	sf::Sprite					m_logoSprite;

	std::vector<sf::Text>		m_textVector;

	sf::Font					m_textFont;

	sf::Color					m_rollFillColour;
	sf::Color					m_rollOutlineColour;
	sf::Color					m_nameFillColour;
	sf::Color					m_nameOutlineColour;
	sf::Color					m_ackFillColour;
	sf::Color					m_ackOutlineColour;

	std::vector<std::pair<std::string, bool>> m_nameArray;

	sf::Music					m_audio;
};

