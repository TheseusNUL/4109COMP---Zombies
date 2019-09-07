#include "SFML_2DCamera.h"



SFML_2DCamera::SFML_2DCamera(int projectionWindowWidth, int projectionWindowHeight)
:m_cameraPosition(0, 0)
,m_cameraWindowSize(projectionWindowWidth, projectionWindowHeight)
,m_cameraZoomLevel(1)
{

}

SFML_2DCamera::~SFML_2DCamera()
{
}

void SFML_2DCamera::setPosition(sf::Vector2f target)
{
	//Set the camera position to the input
	m_cameraPosition = target;
}

sf::FloatRect SFML_2DCamera::getCameraWindow(float parallaxFactor)
{
	//parallaxFactor should be larger than 0
	//If parallaxFactor is less than 1, the window will be smaller and appears to scroll faster otherwise the window will be bigger and appears to scroll slower.

	sf::FloatRect window;
	
	//Calculating the top-left coordinate of the visible area
	window.left = m_cameraPosition.x - m_cameraWindowSize.x / 2 * m_cameraZoomLevel * parallaxFactor;
	window.top = m_cameraPosition.y - m_cameraWindowSize.y / 2 * m_cameraZoomLevel * parallaxFactor;

	//Assign the width and height to x and y attribute respectively
	window.width = m_cameraWindowSize.x * m_cameraZoomLevel * parallaxFactor;
	window.height = m_cameraWindowSize.y * m_cameraZoomLevel * parallaxFactor;

	//Returns the rectangle of the camera
	return window;
}

void SFML_2DCamera::cameraZoomLevelUp(float elapsedTime)
{
	//Increases the cameras zoom level by 10%
	float cameraZoomSpeed = 60.0f;
	float cameraZoomChange = m_cameraZoomLevel * 0.1f * elapsedTime * cameraZoomSpeed;
	m_cameraZoomLevel -= cameraZoomChange;
}

void SFML_2DCamera::cameraZoomLevelDown(float elapsedTime)
{
	//Decreases the cameras zoom by 10%
	float cameraZoomSpeed = 60.0f;
	float cameraZoomChange = m_cameraZoomLevel * 0.1f * elapsedTime * cameraZoomSpeed;;
	m_cameraZoomLevel += cameraZoomChange;
}

sf::Transform SFML_2DCamera::getViewTransform() const
{
	/* Transform the coordinate from WORLD space to CAMERA space centered at the middle of the visible window
		For example, if the camera is at (10, 20), and object at (10, 20) will be directly in the center of the screen*/

	sf::Transform transform = sf::Transform::Identity;
	transform.translate(m_cameraPosition);
	
	return transform.getInverse();
}

sf::Transform SFML_2DCamera::getProjTransform() const
{
	/* Transform the coordinate from CAMERA space to SCREEN space. Not that SCREEN space origin point is at the top left
		corner, so it is effectively shifting the coordinate by half the screen dimension */
	sf::Transform transform = sf::Transform::Identity;
	transform.translate(m_cameraWindowSize.x / 2, m_cameraWindowSize.y / 2);
	transform.scale(1.0f / m_cameraZoomLevel, 1.0f / m_cameraZoomLevel);

	return transform;
}
