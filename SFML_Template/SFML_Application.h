#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_TitleScreen.h"
#include "SFML_GameMenu.h"
#include "SFML_GameWorldLayer.h"
#include "SFML_Credits.h"
#include <SFML/Audio.hpp>
#include "SFML_GameWorld.h"
#include "SFML_BackstoryScreen.h"

class SFML_Application : public sf::NonCopyable
{
public:
	SFML_Application(int windowWidth, int windowHeight);
	~SFML_Application();

	void run();

	//Application State Enumeration
	enum AppState
	{
		E_TITLE_SCREEN,
		E_MAIN_MENU,
		E_GAME_PLAYING,
		E_EXITING,
		E_STORY_MENU,
		E_CREDITS_MENU
	};

	void SetAppState(AppState state)
	{
		m_appState = state;
	}

private:
	

	void update(sf::Time elapsedTime);
	void render();

	//Update FPS function
	void updateFPS(sf::Time elapsedTime);

	//True if the application has been successfully initialize
	bool				m_initialised;

	bool				m_moveRight;
	bool				m_moveLeft;

	bool				m_currentF4KeyPressed;
	bool				m_previousF4KeyPressed;

	bool				m_playBackStory;

	int					m_mainMenuSelection;

	//Member attributes for graphics/window
	sf::RenderWindow	m_window;

	std::size_t			m_screenWidth;
	std::size_t			m_screenHeight;

	//Member attribute for capturing events
	sf::Event			m_event;
	sf::Event			m_lastEvent;

	sf::Font			m_textFont;
	sf::Text			m_messageText;

	SFML_TitleScreen	m_titleScreen;
	SFML_GameMenu		m_mainMenu;

	AppState			m_appState;

	//Member attributes for FPS data
	sf::Time			m_timeCounter;
	sf::Time			m_desiredUpdateTime;
	std::size_t			m_FPSCounter;
	sf::Text			m_FPSdisplay;

	//Member attribute for credits screen
	SFML_Credits		m_creditsScreen;

	//Member attribute for Game World
	SFML_GameWorld		m_gameWorld;

	//Member attribute for Back story screen
	SFML_BackstoryScreen m_storyScreen;
};

