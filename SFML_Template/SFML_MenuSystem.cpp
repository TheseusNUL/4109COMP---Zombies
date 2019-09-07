#include "SFML_MenuSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_MenuSystem::SFML_MenuSystem(std::string menufilename, float spacing)
	: m_textSpacing(spacing)
{
	//Call parse function to get files
	ParseTextFile(menufilename);


	SetAlignment();
}


SFML_MenuSystem::~SFML_MenuSystem()
{
	m_menuText.clear();
}

int SFML_MenuSystem::update(sf::Vector2i mousePosition)
{
	//sf::Transform transform = getTransform();

	for (unsigned int i = 0; i < m_menuText.size(); i++)
	{
		sf::FloatRect rect = m_menuText[i].getGlobalBounds();

		if (m_menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			m_menuText[i].setFillColor(m_highlightTextColour);
			m_menuText[i].setOutlineColor(m_highlightOutlineColour);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				return i;
			}
		}
		else
		{
			m_menuText[i].setFillColor(m_normalTextColour);
			m_menuText[i].setOutlineColor(m_normalOutlineColour);
		}
	}

	return -1;
}

void SFML_MenuSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_menuText.size(); i++)
	{
		target.draw(m_menuText[i]);
	}
}

void SFML_MenuSystem::SetCharacterSize(int size)
{
	for (int i = 0; i < m_menuText.size(); i++)
		m_menuText[i].setCharacterSize(size);

	SetAlignment();
}

void SFML_MenuSystem::SetPosition(float x, float y)
{
	m_menuItemBorder.x = x;
	m_menuItemBorder.y = y;

	SetAlignment();
}

void SFML_MenuSystem::SetBoundingArea()
{
	//Set the maximum values of the bounding area
	float maxHeight = 0.0f;
	float maxWidth = 0.0f;
	m_boundingArea.left = m_menuText[0].getPosition().x;
	m_boundingArea.top = m_menuText[0].getPosition().y;

	for (int i = 0; i < m_menuText.size(); i++)
	{
		sf::FloatRect textbox = m_menuText[i].getLocalBounds();
		//height
		maxHeight += textbox.height * m_textSpacing;
		//width
		if (maxWidth < textbox.width)
			maxWidth = textbox.width;

		// Set boundary position to minimum position of all menu items
		if (m_boundingArea.left > m_menuText[i].getPosition().x)
			m_boundingArea.left = m_menuText[i].getPosition().x;
	}
	//maxHeight -= m_menuText.back().getLocalBounds().height * m_textSpacing;

	m_boundingArea.width = maxWidth;
	m_boundingArea.height = maxHeight;
}

bool SFML_MenuSystem::ParseTextFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
		return false;

	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("fontPath") != std::string::npos)
		{
			int equalsPos = line.find("=") + 2;
			m_gameFont.loadFromFile(line.substr(equalsPos, line.length() - equalsPos));
		}

		if (line.find("colourFill") != std::string::npos)
		{
			ParseRGBAValues(line, m_normalTextColour);
		}

		if (line.find("colourOutline") != std::string::npos)
		{
			ParseRGBAValues(line, m_normalOutlineColour);
		}

		if (line.find("colourHiFill") != std::string::npos)
		{
			ParseRGBAValues(line, m_highlightTextColour);
		}

		if (line.find("colourHiOutline") != std::string::npos)
		{
			ParseRGBAValues(line, m_highlightOutlineColour);
		}

		if (line.find("alignment") != std::string::npos)
		{
			int equalsPos = line.find("=") + 2;
			std::string value = line.substr(equalsPos, line.length() - equalsPos);

			if (!std::strcmp(value.c_str(), "LEFT_JUSTIFIED"))
				m_menuAlignment = E_LEFT_JUSTIFIED;
			else if (!std::strcmp(value.c_str(), "CENTRE_JUSTIFIED"))
				m_menuAlignment = E_CENTRE_JUSTIFIED;
			else if (!std::strcmp(value.c_str(), "RIGHT_JUSTIFIED"))
				m_menuAlignment = E_RIGHT_JUSTIFIED;
		}

		if (line.find("itemText") != std::string::npos)
		{
			int equalsPos = line.find("=") + 2;
			std::string value = line.substr(equalsPos, line.length() - equalsPos);

			sf::Text item;
			item.setString(value);
			item.setFont(m_gameFont);
			item.setStyle(sf::Text::Bold);
			item.setFillColor(m_normalTextColour);
			item.setOutlineColor(m_normalOutlineColour);
			item.setOutlineThickness(2.0f);
			m_menuText.push_back(item);
		}

		if (line.find("border") != std::string::npos)
		{
			ParseVector2(line, m_menuItemBorder);
		}
	}
}

void SFML_MenuSystem::ParseRGBAValues(const std::string& line, sf::Color& colour)
{
	std::stringstream stringStream(line);

	std::string dump, strRed, strGreen, strBlue, stringAlpha;

	stringStream >> dump;
	stringStream >> strRed;
	stringStream >> strGreen;
	stringStream >> strBlue;
	stringStream >> stringAlpha;

	colour.r = std::stoi(strRed);
	colour.g = std::stoi(strGreen);
	colour.b = std::stoi(strBlue);
	colour.a = std::stoi(stringAlpha);
}

void SFML_MenuSystem::ParseVector2(const std::string& line, sf::Vector2f& vector)
{
	std::stringstream stringStream(line);

	std::string dump, x, y;

	stringStream >> dump;
	stringStream >> x;
	stringStream >> y;

	vector.x = std::stof(x);
	vector.y = std::stof(y);
}

void SFML_MenuSystem::SetAlignment()
{
	// Aligning the menu items depends on m_boundingArea being set up correctly
	// To guarantee m_boundingArea is correct, call the function first
	SetBoundingArea();

	float xPosition;

	for (int i = 0; i < m_menuText.size(); i++)
	{
		sf::FloatRect textbox = m_menuText[i].getLocalBounds();

		switch (m_menuAlignment)
		{
		case E_LEFT_JUSTIFIED:
			xPosition = m_menuItemBorder.x;
			break;

		case E_CENTRE_JUSTIFIED:
			xPosition = (m_menuItemBorder.x + (m_boundingArea.width / 2)) - (textbox.width / 2);
			break;

		case E_RIGHT_JUSTIFIED:
			xPosition = m_menuItemBorder.x + m_boundingArea.width - textbox.width;
			break;
		}

		m_menuText[i].setPosition(xPosition, m_menuText[i].getPosition().y);
	}

	// Position menu items vertically
	PositionMenuItems(m_textSpacing);
}

void SFML_MenuSystem::PositionMenuItems(float spacing)
{
	//Iterate through menu items and position on screen
	sf::Vector2f textpos;
	float previousHeight = m_menuItemBorder.y;

	for (int i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].setPosition(m_menuText[i].getPosition().x, previousHeight);

		previousHeight += m_menuText[i].getLocalBounds().height * spacing;
	}

	SetBoundingArea();
}
