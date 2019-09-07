#pragma once

#include <SFML/Graphics.hpp>
#include "SFML_SpriteAnimation.h"
#include "SFML_SpriteObject.h"

class SFML_AnimationSpriteObject : public SFML_SpriteObject
{
public:
			SFML_AnimationSpriteObject();
	virtual	~SFML_AnimationSpriteObject();

	//Member function to add animation sequence
	int						addAnimation(const std::string& spriteSheetFilename, const std::string& spriteInfoFilename, float animationTime);

	//Member function that sets the current animation
	void					setCurrentAnimation(int animationIndex);

	//Member function that returns the address to the current animation
	inline SFML_SpriteAnimation*	getCurrentAnimation() { return m_currentAnimation; }

	//Member functions that updates object states and variables
	virtual void			update(float elapsedTime);

	//Member functions to control playback
	inline void				play() { m_paused = false; }
	inline void				pause() {m_paused = true;}
	void					stop();

	void					ParseIntFile(const std::string& line);

private:
	std::vector<sf::Texture*>	m_spriteTextureList;
	int							m_currentAnimationID;
	float						m_currentTime;
	bool						m_paused;

	int m_intLeft, m_intTop, m_intWidth, m_intHeight, m_intXcenter, m_intYcenter;

protected:
	std::vector<SFML_SpriteAnimation*> m_animatedSpriteList;
	SFML_SpriteAnimation*				m_currentAnimation;
};


