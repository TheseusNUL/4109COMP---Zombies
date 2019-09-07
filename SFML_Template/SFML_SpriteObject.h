#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class SFML_SpriteObject : public sf::Drawable, public sf::Transformable
{
public:
	SFML_SpriteObject();
	virtual ~SFML_SpriteObject();

	//Member function to load and set the default textures
	bool			setDefaultTexture(std::string textureFileName);
	bool			setDefaultTexture(std::string textureFileName, std::string spriteInfoFileName);

	void			parseSpriteFile(const std::string& line);

	//Member function to set tint colour
	void			setTintColour(const sf::Color& tintColour);

	//Member function to get object's world position
	sf::Vector2f	getObjectWorldPosition();

	//Member function to get the object's screen position
	sf::Vector2f	getObjectScreenPosition(sf::Transform cameraScreenTransform);

	//Member function to get object's global and local bounding
	sf::FloatRect	getLocalBounds() const;
	sf::FloatRect	getGlobalBounds() const;

	virtual void	update(float elapsedTime);

private:
	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite		m_sprite;

	sf::Texture*	m_defaultTexture;
	
	int m_intLeft, m_intTop, m_intWidth, m_intHeight, m_intXcenter, m_intYcenter;

protected:

	bool			m_initialised;

	sf::IntRect		m_textureRect;

	sf::Vector2f	m_spriteOriginPoint;
	sf::Texture*	m_spriteTexture;
};