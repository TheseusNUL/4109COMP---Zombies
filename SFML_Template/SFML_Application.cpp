#include "SFML_Application.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

SFML_Application::SFML_Application(int windowWidth, int windowHeight)
	: m_window(sf::VideoMode(windowWidth, windowHeight), "SFML Application", sf::Style::Fullscreen)
	, m_initialised(true)
	, m_screenWidth(windowWidth)
	, m_screenHeight(windowHeight)
	, m_moveRight(true)
	, m_moveLeft(false)
	, m_appState(E_TITLE_SCREEN)
	, m_previousF4KeyPressed(false)
	, m_gameWorld(windowWidth, windowHeight, &m_window)
	, m_timeCounter(sf::seconds(0))
	, m_desiredUpdateTime(sf::seconds(1.0f / 60.0f))
	, m_FPSCounter(0)
	, m_titleScreen("Media/Textures/Title_Screen.jpg", windowWidth, windowHeight)
	, m_mainMenu("Media/MenuInfo/main-menu.txt", windowWidth, windowHeight)
	, m_creditsScreen(windowWidth, windowHeight)
	, m_storyScreen(windowWidth, windowHeight)
	, m_playBackStory(true)
	, m_mainMenuSelection(-1)
{
	//Load fronts to be used for on-screen messages-----------------------
	m_initialised = m_textFont.loadFromFile("Media/Fonts/SegoeMarker.ttf");

	//Set the message text object to use the font just loaded
	m_messageText.setFont(m_textFont);

	m_messageText.setCharacterSize(10);

	m_FPSdisplay.setFont(m_textFont);
	m_FPSdisplay.setCharacterSize(20);

	sf::FloatRect textbox = m_messageText.getLocalBounds();
	m_messageText.setOrigin(textbox.width / 2, textbox.height / 2);

	//Set scale of menu to 85% and position at center of screen
	m_mainMenu.setScale(0.85f, 0.85f);
	m_mainMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);
	m_mainMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);
}


SFML_Application::~SFML_Application()
{

}

void SFML_Application::update(sf::Time elapsedTime)
{
	//Check if the player has tried to close the window, if slow close the application
	//This m_event triggers when 1) the x window button is pressed or 2) if Alt-F4 keys are pressed
	m_window.pollEvent(m_event);
	if (m_event.type == sf::Event::Closed)
		m_window.close();

	int selection;

	float cameraPanningSpeed = 60.0f;

	//Prepare the message text and it's position
	switch (m_appState)
	{
	case E_TITLE_SCREEN:
		if (m_event.type == sf::Event::KeyPressed)
			m_appState = E_MAIN_MENU;			
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			m_appState = E_EXITING;

		m_titleScreen.update(elapsedTime);
		break;

	case E_MAIN_MENU:
		m_mainMenuSelection = m_mainMenu.update(sf::Mouse::getPosition(m_window));
		
		m_gameWorld.SetPausedState(false);

		m_storyScreen.ResetVariables();


		switch (m_mainMenuSelection)
		{
		case 0:
			if (m_playBackStory)
				m_appState = E_STORY_MENU;
			else
				m_appState = E_GAME_PLAYING;
			break;
		case 1: m_appState = E_STORY_MENU;
			break;
		case 2:	m_appState = E_CREDITS_MENU;
			break;
		case 3:	m_appState = E_EXITING;
			break;
		}
		break;

	case E_STORY_MENU:
		if (m_storyScreen.update(elapsedTime) == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			// User selected "Start game"
			if (m_mainMenuSelection == 0)
			{
				m_appState = E_GAME_PLAYING;
				m_playBackStory = false;
			}

			//User selected "Story"
			else if (m_mainMenuSelection == 1)
			{
				m_appState = E_MAIN_MENU;
				m_playBackStory = false;
			}
		}

		break;
	
	case E_GAME_PLAYING:
		m_gameWorld.SetPreviousEscapeButton(true);
		m_gameWorld.update(elapsedTime.asSeconds(), m_event, this);
		break;

	case E_CREDITS_MENU:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_appState = E_MAIN_MENU;
			m_creditsScreen.PositionMenuItems();
		}
			
		if (m_creditsScreen.update(elapsedTime) == 0)
		{
			m_appState = E_MAIN_MENU;
			m_creditsScreen.PositionMenuItems();
		}

		m_creditsScreen.update(elapsedTime);
		break;
	
	case E_EXITING: m_window.close();
		break;
	}

	m_lastEvent = m_event;
}

void SFML_Application::render()
{
	m_window.clear();
	
	switch (m_appState)
	{
	case E_TITLE_SCREEN: m_window.draw(m_titleScreen);
		break;
	case E_MAIN_MENU: m_window.draw(m_mainMenu);
		break;
	case E_STORY_MENU: m_window.draw(m_storyScreen);
		break;
	case E_GAME_PLAYING: m_window.draw(m_gameWorld);
		break;
	case E_CREDITS_MENU: m_window.draw(m_creditsScreen);
		break;
	case E_EXITING:
		break;

	}

	m_currentF4KeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::F4);
	bool showFPS = false;

	//Toggle FPS Display
	if (m_currentF4KeyPressed && !m_previousF4KeyPressed)
		m_window.draw(m_FPSdisplay);
	
	m_window.display();
}

void SFML_Application::run()
{
	if (!m_initialised)
		return;

	sf::Clock clock;

	sf::Time lastUpdateTime = sf::Time::Zero;

	while (m_window.isOpen())
	{
		//Get the elapsed time since last loop
		sf::Time elapsedTime = clock.restart();

		//Add to the time since the last update
		lastUpdateTime += elapsedTime;

		while (lastUpdateTime > m_desiredUpdateTime)
		{
			//Update application states
			update(m_desiredUpdateTime);

			lastUpdateTime -= m_desiredUpdateTime;
		}

		//Calculate FPS
		updateFPS(elapsedTime);

		//Render result to the screen
		render();
	}
}

void SFML_Application::updateFPS(sf::Time elapsedTime)
{
	//Count the number of times the render() function is executed in a second
	m_timeCounter += elapsedTime;

	m_FPSCounter += 1;

	//If the time counter has passed one second then set the message string
	if (m_timeCounter >= sf::seconds(1.0f))
	{
		m_FPSdisplay.setString("Refresh Rate = " + std::to_string(m_FPSCounter) + "FPS \n" + "Frame Time = " +
		std::to_string(m_timeCounter.asMicroseconds() / m_FPSCounter) + " microseconds \n");
	
		
		m_timeCounter -= sf::seconds(1.0f);

		//Reset the FPS counter
		m_FPSCounter = 0;
	}
}
