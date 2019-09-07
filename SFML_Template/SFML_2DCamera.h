#pragma once

#include <SFML/Graphics.hpp>

class SFML_2DCamera
{
public:
	SFML_2DCamera(int projectionWindowWidth, int projectionWindowHeight);
	~SFML_2DCamera();

	//Function to get the visible area of the game world
	sf::FloatRect	getCameraWindow(float parallaxFactor);

	//Function to set the virtual camera's position
	void			setPosition(sf::Vector2f target);

	//Functions to increase and decrease camera zoom level
	void			cameraZoomLevelUp(float elaspedTime);
	void			cameraZoomLevelDown(float elapsedTime);

	sf::Transform	getViewTransform() const;
	sf::Transform	getProjTransform() const;

private:
	//Member attribute to store the visible area width and height
	sf::Vector2f	m_cameraWindowSize;

	//Member attribute to store the cameras position
	sf::Vector2f	m_cameraPosition;

	//Member attribute to store camera zoom level to emulate height
	float			m_cameraZoomLevel;
};

