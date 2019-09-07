#include "SFML_Credits.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

SFML_Credits::SFML_Credits(int windowWidth, int windowHeight)
{
	LoadBackgroundImage("Media/Textures/Credits.jpg");

	//Set volume settings
	m_audio.openFromFile("Media/Audio/Daybreak.ogg");
	m_audio.setLoop(true);
	m_audio.setVolume(75);

	//Cast background values to floats and scale appropriately
	float scaleX = static_cast<float>(windowWidth) / static_cast<float>(m_backgroundTexture.getSize().x);
	float scaleY = static_cast<float>(windowHeight) / static_cast<float>(m_backgroundTexture.getSize().y);
	m_backgroundSprite.setScale(scaleX, scaleY);

	LoadNames("Media/MenuInfo/Credits_Names.txt");

	m_textFont.loadFromFile("Media/Fonts/DK_Nightbird.ttf");

	ParseTextFile("Media/MenuInfo/Credits_Fixed.txt");

	PositionMenuItems();

	//Load the texture file and set it to the sprite
	m_logo.loadFromFile("Media/Textures/Company_Logo.png");
	m_logoSprite.setTexture(m_logo);

	//Set the origin, texture rectangle and position of the sprite
	sf::Vector2u logoTextureSize = m_logo.getSize();
	m_logoSprite.setOrigin(logoTextureSize.x / 2, logoTextureSize.y / 2);
	m_logoSprite.setTextureRect(sf::IntRect(0, 0, logoTextureSize.x, logoTextureSize.y));
	m_logoSprite.setPosition(m_backgroundTexture.getSize().x * 0.1f, m_backgroundTexture.getSize().y / 2 * 0.1f);
}


SFML_Credits::~SFML_Credits()
{
}

int SFML_Credits::update(sf::Time elapsedTime)
{
	if (m_audio.getStatus() != sf::SoundSource::Status::Playing)
		m_audio.play();

	if (sf::Keyboard::isKeyPressed((sf::Keyboard::Escape)))
		m_audio.stop();

	for (int i = 0; i < m_textVector.size(); i++) //Move each text item up
	{
		m_textVector[i].move(0, -50 * elapsedTime.asSeconds());
	}

	//Change appstate to main menu
	float lastElementPosition = m_textVector.back().getPosition().y;
	float lastElementHeight = m_textVector.back().getLocalBounds().height;

	if (lastElementPosition + lastElementHeight < 0)
		return 0;

	return -1;
}

void SFML_Credits::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_backgroundSprite);

	for (int i = 0; i < m_textVector.size(); i++)
		target.draw(m_textVector[i]);

	target.draw(m_logoSprite);
}

void SFML_Credits::LoadBackgroundImage(const std::string& filepath)
{
	//Load the texture file and set it to the sprite
	m_backgroundTexture.loadFromFile(filepath);
	
	m_backgroundSprite.setTexture(m_backgroundTexture);

	//Set the origin, texture rectangle, and position of the sprite
	sf::Vector2u textureSize = m_backgroundTexture.getSize();
	m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	m_backgroundSprite.setPosition(0, 0);

	//Set the rectangle texture which is centered at 0, 0
	m_backgroundRect.left = 0;
	m_backgroundRect.top = 0;
	m_backgroundRect.width = textureSize.x;
	m_backgroundRect.height = textureSize.y;
}


void SFML_Credits::LoadNames(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
		return;

	std::string line;

	//initializes pair and pushes into array
	while (std::getline(file, line))
	{
		std::pair<std::string, bool> namePair;
		namePair.first = line;
		namePair.second = false;

		m_nameArray.push_back(namePair);
	}

	file.close();
}

bool SFML_Credits::ParseTextFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
		return false;

	std::string line;
	sf::Text text;
	int storage;

	std::srand(std::time(0)); //use current time as seed for random number generator

	while (std::getline(file, line))
	{

		if (line[0] == '+')
		{
			do//randomize names while name has been used before
			{
				storage = rand() % m_nameArray.size();

			} while (m_nameArray[storage].second == true);

			//Once unique name is found set the string
			text.setString(m_nameArray[storage].first);
			text.setFillColor(m_nameFillColour);
			text.setOutlineColor(m_nameOutlineColour);
			text.setOutlineThickness(1.0f);
			text.setCharacterSize(50);
			m_nameArray[storage].second = true;
			
			m_textVector.push_back(text);
		}

		int equalsPos = line.find("=") + 2;
		 //Sets colour for credits
		if (line.find("colourRoleFill") != std::string::npos)
		{
			ParseRGBValues(line, m_rollFillColour);
		}

		if (line.find("colourRoleOutline") != std::string::npos)
		{
			ParseRGBValues(line, m_rollOutlineColour);
		}

		if (line.find("colourNameFill") != std::string::npos)
		{
			ParseRGBValues(line, m_nameFillColour);
		}

		if (line.find("colourNameOutline") != std::string::npos)
		{
			ParseRGBValues(line, m_nameOutlineColour);
		}


		if (line.find("colourAckFill") != std::string::npos)
		{
			ParseRGBValues(line, m_ackFillColour);
		}


		if (line.find("colourAckOutline") != std::string::npos)
		{
			ParseRGBValues(line, m_ackOutlineColour);
		}

		//Fixed name for roles in company
		if (line.find("role") != std::string::npos)
		{
			std::string value = line.substr(equalsPos, line.length() - equalsPos);
			text.setString(value);
			text.setFillColor(m_rollFillColour);
			text.setOutlineColor(m_rollOutlineColour);
			text.setOutlineThickness(1.0f);
			text.setCharacterSize(80);

			m_textVector.push_back(text);
		}
		
		//Fixed name for Lead Programmer
		if (line.find("name") != std::string::npos)
		{
			std::string value = line.substr(equalsPos, line.length() - equalsPos);
			text.setString(value);
			text.setFillColor(m_nameFillColour);
			text.setOutlineColor(m_nameOutlineColour);
			text.setOutlineThickness(1.0f);
			text.setCharacterSize(50);
		
			m_textVector.push_back(text);
		}

		//Fixed name for acknowledgments
		if (line.find("service") != std::string::npos)
		{
			std::string value = line.substr(equalsPos, line.length() - equalsPos);
			text.setString(value);
			text.setFillColor(m_ackFillColour);
			text.setOutlineColor(m_ackOutlineColour);
			text.setOutlineThickness(1.0f);
			text.setCharacterSize(65);

			m_textVector.push_back(text);
		}
		
		text.setFont(m_textFont);
	}

	return true;
}

void SFML_Credits::ParseRGBValues(const std::string& line, sf::Color& colour)
{
	std::stringstream ss(line);

	std::string dump;
	std::string strRed;
	std::string strGreen;
	std::string strBlue;

	ss >> dump; //For name of variable
	ss >> strRed; //red value
	ss >> strGreen; //green value
	ss >> strBlue; //blue

	colour.r = std::stoi(strRed);
	colour.g = std::stoi(strGreen);
	colour.b = std::stoi(strBlue);
}

void SFML_Credits::PositionMenuItems()
{
	sf::Vector2f textpos;
	sf::Text text;
	float previousHeight = 0.0f;
	float previousPosition = m_backgroundRect.height;

	for (int i = 0; i < m_textVector.size(); i++)
	{
		sf::FloatRect textbox = m_textVector[i].getLocalBounds();

		m_textVector[i].setOrigin(textbox.width / 2, textbox.height / 2);
		textpos.x = m_backgroundRect.left + (m_backgroundRect.width * 0.25);
		textpos.y = previousPosition + previousHeight + 10;

		m_textVector[i].setPosition(textpos);

		if (m_textVector[i].getFillColor() == m_rollFillColour)
			previousHeight = textbox.height + 10;
		else if (m_textVector[i].getFillColor() == m_ackFillColour)
			previousHeight = textbox.height + 7;
		else
			previousHeight = textbox.height + 5;
		
		previousPosition = m_textVector[i].getPosition().y;
	}
}