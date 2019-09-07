#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_MenuSystem.h"

class SFML_BackstoryScreen : public SFML_MenuSystem
{
public:
	SFML_BackstoryScreen(int windowWidth, int windowHeight);
	~SFML_BackstoryScreen();

	int update(sf::Time elapsedTime);

	void ResetVariables();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void LoadBackgroundImage(const std::string& filepath);


	sf::Texture					m_backgroundTexture;
	sf::Sprite					m_backgroundSprite;

	float						m_alphaChannel;
	float						m_accumulatedTime;

	enum FADESTATES
	{
		E_FADEIN,
		E_FADEOUT,
		E_WAIT
	};

	FADESTATES					m_state;
};

