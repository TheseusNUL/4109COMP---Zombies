#pragma once
#include <SFML/Graphics.hpp>
class SFML_MenuSystem : public sf::Drawable, public sf::Transformable
{
public:
	SFML_MenuSystem(std::string menufilename, float spacing);
	~SFML_MenuSystem();

	int update(sf::Vector2i mousePosition);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void SetCharacterSize(int size);
	
	void SetPosition(float x, float y);

	float GetHalfWidth() const { return m_boundingArea.width / 2; }
	float GetHalfHeight() const { return m_boundingArea.height / 2; }

private:


	bool ParseTextFile(const std::string& filePath);

	void ParseRGBAValues(const std::string& line, sf::Color& colour);
	void ParseVector2(const std::string& line, sf::Vector2f& vector);

	void SetAlignment();
	void PositionMenuItems(float spacing);
	void SetBoundingArea();

	sf::Font		m_gameFont;

	sf::Color		m_highlightTextColour;
	sf::Color		m_normalTextColour;
	sf::Color		m_normalOutlineColour;
	sf::Color		m_highlightOutlineColour;

	sf::Vector2f	m_menuItemBorder;

	sf::FloatRect	m_boundingArea;

	float			m_textSpacing;

	int				m_highlightIndex;

	enum MENU_ALIGNMENT
	{
		E_LEFT_JUSTIFIED,
		E_CENTRE_JUSTIFIED,
		E_RIGHT_JUSTIFIED
	};

	MENU_ALIGNMENT	m_menuAlignment;

protected:
	std::vector<sf::Text> m_menuText;
};

