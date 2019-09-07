#include "SFML_SpriteObject.h"

SFML_SpriteObject::SFML_SpriteObject()
: m_initialised(false)
{
	//Ensure the pointer has not been used to store any allocated memory
	m_defaultTexture = NULL;
}


SFML_SpriteObject::~SFML_SpriteObject()
{
	//delete any allocated memory that the pointer has stored unless that pointer contains NULL
	if (m_defaultTexture != NULL)
		delete m_defaultTexture;
}

bool SFML_SpriteObject::setDefaultTexture(std::string textureFileName)
{
	//Load a texture from a file and set it as the default texture
	if (m_defaultTexture != NULL)
		delete m_defaultTexture;

	m_defaultTexture = new sf::Texture();

	//Failure to load the file texture
	if (!m_defaultTexture->loadFromFile(textureFileName))
	{
		delete m_defaultTexture;
		m_defaultTexture = NULL;
		return m_initialised = false;
	}

	//If texture file loads successfully
	m_spriteTexture = m_defaultTexture;

	//Set the default rectangle and set the default origin to the center
	m_spriteOriginPoint = sf::Vector2f(m_defaultTexture->getSize().x / 2, m_defaultTexture->getSize().y / 2);
	m_textureRect = sf::IntRect(0, 0, m_defaultTexture->getSize().x, m_defaultTexture->getSize().y);

	return m_initialised = true;

}

bool SFML_SpriteObject::setDefaultTexture(std::string textureFileName, std::string spriteInfoFileName)
{
	//Load a texture from a file and set it as the default texture
	if (m_defaultTexture != NULL)
		delete m_defaultTexture;

	m_defaultTexture = new sf::Texture();

	//Failure to load the file texture
	if (!m_defaultTexture->loadFromFile(textureFileName))
	{
		delete m_defaultTexture;
		m_defaultTexture = NULL;
		return m_initialised = false;
	}

	//If texture file loads successfully
	m_spriteTexture = m_defaultTexture;

	//Opens sprite info file
	std::ifstream file(spriteInfoFileName);

	if (file.is_open())
	{
		//Read a line
		std::string line;
		std::getline(file, line);

		if (file.good())
			parseSpriteFile(line);
	}

	m_spriteOriginPoint = sf::Vector2f(m_intXcenter - m_intLeft, m_intYcenter - m_intTop);
	m_textureRect = sf::IntRect(m_intLeft, m_intTop, m_intWidth, m_intHeight);

	file.close();

	m_initialised = true;

	return m_initialised;
}

void SFML_SpriteObject::parseSpriteFile(const std::string& line)
{
	std::stringstream stringStream(line);

	//Separate line into 6 individual strings
	stringStream >> m_intLeft;
	stringStream >> m_intTop;
	stringStream >> m_intWidth;
	stringStream >> m_intHeight;
	stringStream >> m_intYcenter;
	stringStream >> m_intYcenter;
}

void SFML_SpriteObject::setTintColour(const sf::Color& tintColour)
{
	m_sprite.setColor(tintColour);
}

sf::Vector2f SFML_SpriteObject::getObjectWorldPosition()
{
	//returns the position of the sprites origin in the world space
	//Locally this is point (0, 0)

	return getTransform().transformPoint(0, 0);
}

sf::Vector2f SFML_SpriteObject::getObjectScreenPosition(sf::Transform cameraScreenTransform)
{
	//returns the position of the sprites origin in the screen space
	sf::Transform WorldViewProjTransform = cameraScreenTransform * getTransform();
	
	return WorldViewProjTransform.transformPoint(0, 0);
}

sf::FloatRect SFML_SpriteObject::getLocalBounds() const
{
	//Returns texture rectangle but centered at the sprites origin
	//Cast vector positions to a float then set as point on rectangle
	float left = static_cast<float> (-m_spriteOriginPoint.x);
	float top = static_cast<float> (-m_spriteOriginPoint.y);
	float width = static_cast<float> (m_textureRect.width);
	float height = static_cast<float> (m_textureRect.height);

	return sf::FloatRect(left, top, width, height);
}

sf::FloatRect SFML_SpriteObject::getGlobalBounds() const
{
	//Calculates the texture rectangle in regards to the world space

	return getTransform().transformRect(getLocalBounds());
}

void SFML_SpriteObject::update(float elapsedTime)
{
	if (!m_initialised)
		return;

	//Assign the texture to the sprite
	if (m_spriteTexture != NULL)
		m_sprite.setTexture(*m_spriteTexture);

	//Set the sprites texture rectangle
	m_sprite.setTextureRect(m_textureRect);

	//Set the sprites origin
	m_sprite.setOrigin(m_spriteOriginPoint);
}

void SFML_SpriteObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}
