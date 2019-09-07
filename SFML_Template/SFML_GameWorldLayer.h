#pragma once

#include <SFML/Graphics.hpp>
#include "SFML_2DCamera.h"


class SFML_GameWorldLayer : public sf::Drawable
{
public:
	SFML_GameWorldLayer(std::string textureFilename, float parallaxFactor, int viewportWidth, int viewportHeight);
	~SFML_GameWorldLayer();

	void		followCamera(SFML_2DCamera* camera);
	void		setParallaxFactor(float parallaxFactor);

	void		setScrollSpeed(sf::Vector2f scrollSpeed);

	float		getParallaxFactor();

	void		update(float elapsedTime);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Member attribute to store texture object
	sf::Texture		m_layerTexture;

	//Member attribute to store the window articles
	sf::VertexArray	m_layerVertices;

	//Member attribute to store the layer's parallax factor
	float			m_parallaxFactor;

	//Member variables to store 2D Vectors
	sf::Vector2f	m_offset;
	sf::Vector2f	m_scrollSpeed;

	bool			m_followCamera;

};

