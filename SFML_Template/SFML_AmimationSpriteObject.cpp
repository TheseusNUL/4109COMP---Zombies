#include "SFML_AnimationSpriteObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_AnimationSpriteObject::SFML_AnimationSpriteObject()
: m_currentAnimation(NULL)
, m_paused(false)
, m_currentAnimationID(-1)
{
}


SFML_AnimationSpriteObject::~SFML_AnimationSpriteObject()
{
	//Deleting texture objects
	for (std::vector<sf::Texture*>::iterator it = m_spriteTextureList.begin(); it != m_spriteTextureList.end(); ++it)
		delete (*it);

	m_spriteTextureList.clear();

	//Deleting animation objects
	for (std::vector<SFML_SpriteAnimation*>::iterator it = m_animatedSpriteList.begin(); it != m_animatedSpriteList.end(); ++it)
		delete (*it);

	m_animatedSpriteList.clear();

}

int SFML_AnimationSpriteObject::addAnimation(const std::string& spriteSheetFilename, const std::string& spriteInfoFilename, float animationTime)
{
	//Create new objects
	sf::Texture* spriteTexture = new sf::Texture();

	//Load Texture file
	if (!spriteTexture->loadFromFile(spriteSheetFilename))
		return -1;

	//Store to vector
	m_spriteTextureList.push_back(spriteTexture);

	SFML_SpriteAnimation* animation = new SFML_SpriteAnimation();
	
	//Store address of object in animation
	animation->setSpriteSheet(spriteTexture);

	int frameCounter = 0;

	std::ifstream inputFile(spriteInfoFilename);

	if (inputFile.is_open())
	{
		std::string line;
		while(std::getline(inputFile, line))
		{
			ParseIntFile(line);

			//use them as the animation frame
			animation->addFrame(sf::IntRect(m_intLeft, m_intTop, m_intWidth, m_intHeight), sf::Vector2f(m_intXcenter - m_intLeft, m_intYcenter - m_intTop));

			//Keep track of the counter - how many frames.
			frameCounter++;
		}

		inputFile.close();
	}

	if (frameCounter > 0)
	{
		animation->setFrameTime(animationTime / frameCounter);
		m_animatedSpriteList.push_back(animation);
		m_initialised = true;
		return m_animatedSpriteList.size() - 1;
	}
	else
		return -1;
}

void SFML_AnimationSpriteObject::setCurrentAnimation(int animationIndex)
{
	SFML_SpriteAnimation* newAnimation = m_animatedSpriteList[animationIndex];

	if(newAnimation) //checking if animation is valid
		if (m_currentAnimation != newAnimation)
		{
			m_currentAnimation = newAnimation;
			m_spriteTexture = m_currentAnimation->getSpriteSheet();
			m_currentAnimation->resetCurrentAnimation();
			m_currentAnimation->getCurrentFrame(&m_textureRect, &m_spriteOriginPoint);
			m_currentTime = 0;
		}
}	

void SFML_AnimationSpriteObject::update(float elapsedTime)
{
	if (!m_paused && m_currentAnimation)
	{
		float frameTime = m_currentAnimation->getTimePerFrame();
		m_currentTime += elapsedTime;

		if (m_currentTime >= frameTime)
		{
			m_currentAnimation->incrementFrameNumber();
			m_currentAnimation->getCurrentFrame(&m_textureRect, &m_spriteOriginPoint);
			m_currentTime -= frameTime;
		}
	}

	SFML_SpriteObject::update(elapsedTime);
}

void SFML_AnimationSpriteObject::stop()
{
	m_paused = true;
	m_currentTime = 0;
}

void SFML_AnimationSpriteObject::ParseIntFile(const std::string& line)
{
	std::istringstream stringStream(line);
	stringStream >> m_intLeft;
	stringStream >> m_intTop;
	stringStream >> m_intWidth;
	stringStream >> m_intHeight;
	stringStream >> m_intXcenter;
	stringStream >> m_intYcenter;
}
