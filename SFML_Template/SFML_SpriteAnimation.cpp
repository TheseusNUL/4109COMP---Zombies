#include "SFML_SpriteAnimation.h"

SFML_SpriteAnimation::SFML_SpriteAnimation()
:m_currentFrameNumber(0)
,m_canBeInterrupted(true)
,m_looped(true)
{
}


SFML_SpriteAnimation::~SFML_SpriteAnimation()
{
	//clearing vectors
	m_frameCenterList.clear();
	m_frameRectList.clear();
}

void SFML_SpriteAnimation::addFrame(sf::IntRect textureRect, sf::Vector2f centre)
{
	m_frameRectList.push_back(textureRect);
	m_frameCenterList.push_back(centre);
}

bool SFML_SpriteAnimation::getCurrentFrame(sf::IntRect* rect, sf::Vector2f* centre)

{
	if ((m_currentFrameNumber < 0) || (m_currentFrameNumber >= m_frameRectList.size()))
	{
		//Out of bounds position is supplied
		return false;
	}

	//retrieve the rectangle and centre data
	sf::IntRect rectangle = m_frameRectList[m_currentFrameNumber];
	sf::Vector2f icentre = m_frameCenterList[m_currentFrameNumber];

	//Copy information over
	rect->left = rectangle.left;
	rect->top = rectangle.top;
	rect->width = rectangle.width;
	rect->height = rectangle.height;

	centre->x = icentre.x;
	centre->y = icentre.y;

	return true;
}

void SFML_SpriteAnimation::incrementFrameNumber()
{
	m_currentFrameNumber++;
	if (m_currentFrameNumber >= m_frameRectList.size() - 1)
	{
		if (m_looped)
			resetCurrentAnimation();
		else
			m_currentFrameNumber = m_frameRectList.size() - 1;
	}
}

bool SFML_SpriteAnimation::animationCompleted()
{
	//Checks if animation is complete.
	if (m_currentFrameNumber >= m_frameRectList.size() - 1)
		return true;
	else
		return false;
}
