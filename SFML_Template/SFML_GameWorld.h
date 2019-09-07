#pragma once

#include <SFML/Graphics.hpp>
#include "SFML_GameWorldLayer.h"
#include "SFML_SpriteObject.h"
#include "SFML_AnimationSpriteObject.h"
#include "SFML_NPCSpriteObject.h"
#include "SFML_PlayableSpriteObject.h"
#include "SFML_BulletSpriteObject.h"
#include "SFML_MenuSystem.h"
#include "SFML_HUD.h"

class SFML_Application;

class SFML_GameWorld : public sf::Drawable
{
public:
	SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow );
	~SFML_GameWorld();

	void			update(float elapsedTime, const sf::Event& event, SFML_Application* app);
	void			SetPausedState(bool paused) { m_paused = paused; }
	void			SetPreviousEscapeButton(bool pressed) { m_previousEscapeButtonPressed = pressed; }

private:
	void			processEvents(float elapsedTime);
	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

	static bool		layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b);

	void			MeleeAttack();

	void			ResetVariables();

	//Member attributes to store render window dimensions
	int				m_windowWidth;
	int				m_windowHeight;

	int				m_zoomDistance;

	bool			m_paused;
	bool			m_previousEscapeButtonPressed;

	float			m_accumulatedTime;

	//Array to store multiple game world layers
	std::vector<SFML_GameWorldLayer*> m_gameWorldLayerList;

	//Member attribute to store the camera object
	SFML_2DCamera		m_camera;

	//Member attribute to store cameras position
	sf::Vector2f		m_cameraPosition;

	//Member attribute for capturing events
	sf::Event			m_event;
	sf::Event			m_lastEvent;

	sf::RenderWindow*	m_parentWindow;

	//Member attributes for game objects
	SFML_SpriteObject	m_spriteObject;

	SFML_AnimationSpriteObject*				m_animatedObject;
	int										m_idleAnimationID;

	std::vector<SFML_NPCSpriteObject*>		m_zombieCharacterList;
	int										m_zombieNumber;

	std::vector<SFML_BulletSpriteObject*>	m_bullets;

	SFML_PlayableSpriteObject*				m_playerCharacter;

	SFML_MenuSystem							m_pauseMenu;

	SFML_HUD*								m_hud;
};

