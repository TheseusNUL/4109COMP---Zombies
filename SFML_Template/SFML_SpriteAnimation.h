#pragma once
#include <SFML/Graphics.hpp>

class SFML_SpriteAnimation
{
public:
	SFML_SpriteAnimation();
	~SFML_SpriteAnimation();

public:
	//Member function to store the memory address of the texture to use
	inline void		setSpriteSheet(sf::Texture* spriteTexture) {m_spriteSheet = spriteTexture;};

	//Member function to add a frame (Rectangle and Center)
	void			addFrame(sf::IntRect textureRect, sf::Vector2f center);

	//Member function that returns current frame information (Rectangle and Center)
	bool			getCurrentFrame(sf::IntRect* RECT, sf::Vector2f* center);

	//Member function to set the delay between showing consecutive frames
	inline void		setFrameTime(float timePerFrame) { m_timePerFrame = timePerFrame; }

	//Member function that returns the delay between showing consecutive frames
	inline float	getTimePerFrame() { return m_timePerFrame; }

	//Member function that returns the address of the texture used
	inline sf::Texture*	getSpriteSheet() {return m_spriteSheet; }

	//Member function that returns the number of frames in this animation
	inline int		getNumberofFrames() {return m_frameRectList.size();}

	//Member functions to set and get the loop flag
	inline void		setLoop(bool loop) { m_looped = loop; }
	inline bool		looped() const {return m_looped;}

	//Member functions to set and get interruptible flag
	inline void		setInterrupt(bool trueFlase) { m_canBeInterrupted = trueFlase; }
	inline bool		canBeInterrupted() const { return m_canBeInterrupted; }

	//Member functions to increment and get frame number, and reset animation
	void			incrementFrameNumber();
	inline int		getCurrentFrameNumber() { return m_currentFrameNumber; }
	inline void		resetCurrentAnimation() { m_currentFrameNumber = 0; }

	/* Member function to check if the animation is complete
	This will never be true if the animation is looping*/
	bool			animationCompleted();

private:
	//Frame information
	std::vector<sf::IntRect>		m_frameRectList;
	std::vector<sf::Vector2f>		m_frameCenterList;

	//Texture object
	sf::Texture*					m_spriteSheet;

	//Current frame numbers and time delay
	int								m_currentFrameNumber;
	float							m_timePerFrame;

	//Animation controls
	bool							m_looped;
	bool							m_canBeInterrupted;
};
