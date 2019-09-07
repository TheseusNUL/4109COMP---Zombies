#include "SFML_GameWorldLayer.h"

SFML_GameWorldLayer::SFML_GameWorldLayer(std::string textureFileName, float parallaxFactor, int viewportWidth, int viewportHeight)
:m_layerTexture()
,m_layerVertices(sf::Quads, 4)
,m_parallaxFactor(parallaxFactor)
,m_offset(0, 0)
,m_scrollSpeed(100, 0)
,m_followCamera(true)

{
	if (m_layerTexture.loadFromFile(textureFileName));
	{
		//Set Texture to repeat
		m_layerTexture.setRepeated(true);

		//Set the vertices's of the quad to the following
		// 0 -------- 1
		// |		  |
		// |		  |
		// |          |
		// 3 -------- 2

		m_layerVertices[0].position = sf::Vector2f(0.0f, 0.0f);
		m_layerVertices[1].position = sf::Vector2f(viewportWidth, 0.0f);
		m_layerVertices[2].position = sf::Vector2f(viewportWidth, viewportHeight);
		m_layerVertices[3].position = sf::Vector2f(0.0f, viewportHeight);
	}
}

SFML_GameWorldLayer::~SFML_GameWorldLayer()
{
}

void SFML_GameWorldLayer::followCamera(SFML_2DCamera* camera)
{
	//Find out the visible area rectangle of the camera
	sf::FloatRect cameraWindows = camera->getCameraWindow(m_parallaxFactor);

	//Calculate the left, right, top and bottom values
	int left = cameraWindows.left + m_offset.x;
	int right = cameraWindows.left + cameraWindows.width + m_offset.x;
	int top = cameraWindows.top + m_offset.y;
	int bottom = cameraWindows.top + cameraWindows.height + m_offset.y;

	//Set the texture coordinates using those values
	m_layerVertices[0].texCoords = sf::Vector2f(left, top);
	m_layerVertices[1].texCoords = sf::Vector2f(right, top);
	m_layerVertices[2].texCoords = sf::Vector2f(right, bottom);
	m_layerVertices[3].texCoords = sf::Vector2f(left, bottom);
}

void SFML_GameWorldLayer::setParallaxFactor(float parallaxFactor)
{
	m_parallaxFactor = parallaxFactor;
}

void SFML_GameWorldLayer::setScrollSpeed(sf::Vector2f scrollSpeed)
{
	m_scrollSpeed = scrollSpeed;
}

float SFML_GameWorldLayer::getParallaxFactor()
{
	return m_parallaxFactor;
}

void SFML_GameWorldLayer::update(float elapsedTime)
{
	m_offset.x = m_offset.x + elapsedTime * m_scrollSpeed.x;
	m_offset.y = m_offset.y + elapsedTime * m_scrollSpeed.y;
}

void SFML_GameWorldLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//The draw() member function set m_layerTexture as the texture before drawing the texture quad onto the window

	//Assign the m_layerText to render state
	states.texture = &m_layerTexture;

	//Draw the quad using the render state
	target.draw(m_layerVertices, states);
}
