#include "SFML_GameWorld.h"
#include "SFML_Application.h"

SFML_GameWorld::SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow)
: m_windowWidth(windowWidth)
, m_pauseMenu("Media/MenuInfo/pause_menu.txt", 1.5f)
, m_windowHeight(windowHeight)
, m_camera(windowWidth, windowHeight)
, m_zoomDistance(30)
, m_zombieNumber(50)
, m_paused(false)
, m_previousEscapeButtonPressed(false)
{
	//Create a game world layer object using texture
	SFML_GameWorldLayer* background = new SFML_GameWorldLayer("Media/Textures/dirt.png", 1.0f, windowWidth, windowHeight);
	m_gameWorldLayerList.push_back(background);

	SFML_GameWorldLayer* fog = new SFML_GameWorldLayer("Media/Textures/fog.png", 0.8f, windowWidth, windowHeight);
	m_gameWorldLayerList.push_back(fog);

	SFML_GameWorldLayer* forground = new SFML_GameWorldLayer("Media/Textures/wire.png", 0.3f, windowWidth, windowHeight);
	m_gameWorldLayerList.push_back(forground);

	//Sort the game world in descending order
	std::sort(m_gameWorldLayerList.begin(), m_gameWorldLayerList.end(), layerSortFunction);

	//Set cameras initial position
	m_cameraPosition = sf::Vector2f(0, 0);

	m_parentWindow = parentWindow;

	//NPC animations
	for (int i = 0; i < m_zombieNumber; i++)
	{
		SFML_NPCSpriteObject* npcSprite = new SFML_NPCSpriteObject();
		npcSprite->setIdleAnimation("Media/Textures/zombie-idle.png", "Media/SpriteInfo/zombie-idle.txt", 1.0f);
		npcSprite->setWalkingAnimation("Media/Textures/zombie-move.png", "Media/SpriteInfo/zombie-move.txt", 1.0f);
		npcSprite->setAttackingAnimation("Media/Textures/zombie-attack.png", "Media/SpriteInfo/zombie-attack.txt", 1.0f);
		npcSprite->setBloodAnimation("Media/Textures/blood.png", "Media/SpriteInfo/blood.txt", 1.0f);

		npcSprite->setPosition(rand() % 10000 - 1000, rand() % 10000 - 1000);
		npcSprite->setTargetLocation(sf::Vector2f(rand() % 10000 - 1000, rand() % 10000 - 1000));
		m_zombieCharacterList.push_back(npcSprite);
	}

	m_playerCharacter = new SFML_PlayableSpriteObject();
	//Flashlight animations
	m_playerCharacter->setFlashlightIdleAnimation("Media/Textures/hero_flashlight_idle.png", "Media/SpriteInfo/hero_flashlight_idle.txt", 1.0f);
	m_playerCharacter->setFlashlightWalkingAnimation("Media/Textures/hero_flashlight_move.png", "Media/SpriteInfo/hero_flashlight_move.txt", 1.0f);
	m_playerCharacter->setFlashlightMeleeAnimation("Media/Textures/hero_flashlight_melee.png", "Media/SpriteInfo/hero_flashlight_melee.txt", 1.0f);
	//Knife animations
	m_playerCharacter->setKnifeIdleAnimation("Media/Textures/hero_knife_idle.png", "Media/SpriteInfo/hero_knife_idle.txt", 1.0f);
	m_playerCharacter->setKnifeWalkingAnimation("Media/Textures/hero_knife_move.png", "Media/SpriteInfo/hero_knife_move.txt", 1.0f);
	m_playerCharacter->setKnifeMeleeAnimation("Media/Textures/hero_knife_melee.png", "Media/SpriteInfo/hero_knife_melee.txt", 1.0f);
	//Pistol animations
	m_playerCharacter->setPistolIdleAnimation("Media/Textures/hero_pistol_idle.png", "Media/SpriteInfo/hero_pistol_idle.txt", 1.0f);
	m_playerCharacter->setPistolWalkingAnimation("Media/Textures/hero_pistol_move.png", "Media/SpriteInfo/hero_pistol_move.txt", 1.0f);
	m_playerCharacter->setPistolMeleeAnimation("Media/Textures/hero_pistol_melee.png", "Media/SpriteInfo/hero_pistol_melee.txt", 1.0f);
	m_playerCharacter->setPistolShootAnimation("Media/Textures/hero_pistol_shoot.png", "Media/SpriteInfo/hero_pistol_shoot.txt", 1.0f);
	m_playerCharacter->setPistolReloadAnimation("Media/Textures/hero_pistol_reload.png", "Media/SpriteInfo/hero_pistol_reload.txt", 1.0f);
	//Rifle animations
	m_playerCharacter->setRifleIdleAnimation("Media/Textures/hero_rifle_idle.png", "Media/SpriteInfo/hero_rifle_idle.txt", 1.0f);
	m_playerCharacter->setRifleWalkingAnimation("Media/Textures/hero_rifle_move.png", "Media/SpriteInfo/hero_rifle_move.txt", 1.0f);
	m_playerCharacter->setRifleMeleeAnimation("Media/Textures/hero_rifle_melee.png", "Media/SpriteInfo/hero_rifle_melee.txt", 1.0f);
	m_playerCharacter->setRifleShootAnimation("Media/Textures/hero_rifle_shoot.png", "Media/SpriteInfo/hero_rifle_shoot.txt", 1.0f);
	m_playerCharacter->setRifleReloadAnimation("Media/Textures/hero_rifle_reload.png", "Media/SpriteInfo/hero_rifle_reload.txt", 1.0f);
	

	m_playerCharacter->equipFlashlight();

	m_playerCharacter->setPosition(0, 0);

	m_hud = new SFML_HUD(*m_playerCharacter, m_windowWidth, m_windowHeight);

	m_pauseMenu.SetCharacterSize(50);
	m_pauseMenu.SetPosition(windowWidth / 2 - m_pauseMenu.GetHalfWidth(), windowHeight / 2 - m_pauseMenu.GetHalfHeight());
}

SFML_GameWorld::~SFML_GameWorld()
{
	//Clear the GameWorldLayer List
	for (auto it = m_gameWorldLayerList.begin(); it != m_gameWorldLayerList.end(); it++)
	{
		delete (*it);
	}
	m_gameWorldLayerList.clear();

	//Clear the memory space used for the animated object
	delete m_animatedObject;
}

void SFML_GameWorld::update(float elapsedTime, const sf::Event& event, SFML_Application* app)
{
	m_event = event;

	float startTime = 1.0f;

	//After 1 second the game should be able to pause
	if (m_accumulatedTime > startTime)
		SetPreviousEscapeButton(false);

	if (!m_paused)
	{
		processEvents(elapsedTime);

		m_parentWindow->setMouseCursorVisible(false);

		m_playerCharacter->update(elapsedTime);
		m_camera.setPosition(m_playerCharacter->getObjectWorldPosition());

		//Update all objects within array
		for (std::size_t i = 0; i < m_gameWorldLayerList.size(); i++)
			m_gameWorldLayerList[i]->followCamera(&m_camera);

		m_gameWorldLayerList[1]->update(elapsedTime);


		//m_animatedObject->update(elapsedTime);

		//Update each zombie object in the list
		for (auto zombieIt = m_zombieCharacterList.begin(); zombieIt != m_zombieCharacterList.end(); )
		{
			(*zombieIt)->update(elapsedTime);

			if ((*zombieIt)->m_zombieHealth <= 0) //zombie health
			{
				(*zombieIt)->toDeadState();
			}

			if ((*zombieIt)->getCurrentState() == SFML_NPCSpriteObject::DEAD)
			{
				if ((*zombieIt)->getCurrentAnimation()->animationCompleted())
				{
					zombieIt = m_zombieCharacterList.erase(zombieIt);
					m_zombieNumber--;
				}
				else 
				{
					zombieIt++;
				}
			}
			else if ((*zombieIt)->getCurrentState() == SFML_NPCSpriteObject::IDLE)
			{
				(*zombieIt)->setTargetLocation(sf::Vector2f(rand() % 4000 - 2000, rand() % 4000 - 2000));
				zombieIt++;
			}
			else
			{
				zombieIt++;
			}
		}

		//Update all bullets
		for (unsigned int i = 0; i < m_bullets.size(); i++)
			m_bullets[i]->update(elapsedTime);

		//Player-Zombie Collision
		for (unsigned int i = 0; i < m_zombieCharacterList.size(); i++)
		{
			if (m_playerCharacter->getGlobalBounds().intersects(m_zombieCharacterList[i]->getGlobalBounds()))
			{
				m_playerCharacter->setTintColour(sf::Color(255, 0, 0));
				break;
			}
			else
			{
				m_playerCharacter->setTintColour(sf::Color(255, 255, 255));
			}
		}

		//Bullet-Zombie Collision
		for (auto bulletIt = m_bullets.begin(); bulletIt != m_bullets.end(); )
		{
			bool hit = false;
			for (auto zombieIt = m_zombieCharacterList.begin(); zombieIt != m_zombieCharacterList.end(); )
			{
				if ((*bulletIt)->getGlobalBounds().intersects((*zombieIt)->getGlobalBounds()))
				{
					bulletIt = m_bullets.erase(bulletIt);
					(*zombieIt)->setTintColour(sf::Color(255, 0, 0));
					hit = true;
					(*zombieIt)->m_zombieHealth--;
					break;
				}
				else
				{
					(*zombieIt)->setTintColour(sf::Color(255, 255, 255));
					zombieIt++;
				}
			}

			if (hit == false)
				bulletIt++;
		}

		//Knife-Zombie Collision
		if (m_playerCharacter->GetEquippedWeapon() == SFML_PlayableSpriteObject::KNIFE && m_playerCharacter->GetPlayerAction() == SFML_PlayableSpriteObject::MELEE)
		{
			for (auto zombieIt = m_zombieCharacterList.begin(); zombieIt != m_zombieCharacterList.end(); )
			{
				if (m_playerCharacter->getGlobalBounds().intersects((*zombieIt)->getGlobalBounds()))
				{
					(*zombieIt)->toDeadState();
					break;
				}
				else
				{
					zombieIt++;
				}
			}
		}

		//Rifle/Pistol-Zombie Collision
		if (m_playerCharacter->GetPlayerAction() == SFML_PlayableSpriteObject::MELEE)
		{
			if (m_playerCharacter->GetEquippedWeapon() == SFML_PlayableSpriteObject::PISTOL || m_playerCharacter->GetEquippedWeapon() == SFML_PlayableSpriteObject::RIFLE)
				MeleeAttack();
		}

		//pause menu functionality
		if (!m_previousEscapeButtonPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_paused = true;
				SetPreviousEscapeButton(true);
			}
				
		}
		
	}
	else
	{
		int selection = m_pauseMenu.update(sf::Mouse::getPosition(*m_parentWindow));

		m_parentWindow->setMouseCursorVisible(true);

		switch (selection)
		{
		case 0:
			m_paused = false;
			SetPreviousEscapeButton(false);
			break;
		case 1: app->SetAppState(SFML_Application::E_MAIN_MENU);
			ResetVariables();
			break;
		case 2: app->SetAppState(SFML_Application::E_EXITING);
		}

		if (m_previousEscapeButtonPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_paused = false;
				SetPreviousEscapeButton(false);
			}
		}
		
	}

	m_hud->update(m_windowHeight);

	m_accumulatedTime += elapsedTime;

	//if game finishes close window
	if (m_playerCharacter->GetHealth() == 0 || m_zombieNumber == 0)
		app->SetAppState(SFML_Application::E_EXITING);
}

void SFML_GameWorld::processEvents(float elapsedTime)
{
	//Zoom camera based on mouse scroll
	if (m_event.type == sf::Event::MouseWheelScrolled && m_lastEvent.type != sf::Event::MouseWheelScrolled)
	{
		if (m_event.mouseWheelScroll.delta > 0.0f && m_zoomDistance <= 60)
		{
			m_camera.cameraZoomLevelUp(elapsedTime);
			m_zoomDistance++;
		}
		else if (m_event.mouseWheelScroll.delta < 0.0f && m_zoomDistance >= 0)
		{
			m_camera.cameraZoomLevelDown(elapsedTime);
			m_zoomDistance--;
		}
	}

	m_lastEvent = m_event;

	//Get camera transform (world -> screen)
	sf::Transform transform;
	transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	//Take inverse (screen -> world)
	sf::Transform inverse = transform.getInverse();

	//Get mouse position (in screen space)
	sf::Vector2f mouseScreenPosition = sf::Vector2f(sf::Mouse::getPosition(*m_parentWindow));

	//Convert to world space co-ordinate
	sf::Vector2f worldPoint = inverse.transformPoint(mouseScreenPosition);

	//Get sprite global bounding rectangle
	sf::FloatRect globalRect = m_spriteObject.getGlobalBounds();

	if (globalRect.contains(worldPoint))
		m_spriteObject.setTintColour(sf::Color(255, 0, 0));
	else
		m_spriteObject.setTintColour(sf::Color(255, 255, 255));


	for (unsigned int i = 0; i < m_zombieCharacterList.size(); i++)
	{
		if (m_zombieCharacterList[i]->getCurrentState() == SFML_NPCSpriteObject::DEAD)
			continue;

		//If zombie can see or hear the player
		if (m_zombieCharacterList[i]->isInVision(m_playerCharacter->getPosition()) || m_zombieCharacterList[i]->isInHearingRange(m_playerCharacter->getPosition()))
		{
			m_zombieCharacterList[i]->setTargetLocation(m_playerCharacter->getPosition());
			m_zombieCharacterList[i]->setTintColour(sf::Color(255, 255, 0));

			if (m_zombieCharacterList[i]->getGlobalBounds().intersects(m_playerCharacter->getGlobalBounds()))
			{
				if (m_zombieCharacterList[i]->getCurrentState() != SFML_NPCSpriteObject::NPCState::ATTACKING)
				{
					m_zombieCharacterList[i]->toAttackingState();
				}
			}
			else
			{
				m_zombieCharacterList[i]->toWalkingState();
			}
		}
		else
		{
			//The player is outside the npc's vision cone
			m_zombieCharacterList[i]->setTintColour(sf::Color(255, 255, 255));
			m_zombieCharacterList[i]->toWalkingState();
		}	

		if (m_zombieCharacterList[i]->getCurrentState() == SFML_NPCSpriteObject::ATTACKING)
		{
			if (m_zombieCharacterList[i]->getCurrentAnimation()->animationCompleted())
			{
				m_playerCharacter->HurtPlayer();
				m_zombieCharacterList[i]->toWalkingState();
			}
		}
	}


	SFML_BulletSpriteObject* bullet = m_playerCharacter->processEvents(elapsedTime, transform, mouseScreenPosition);
	if (bullet)
		m_bullets.push_back(bullet);
	
}

void SFML_GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Render each game layer with parallax factor >= 1.0
	for (unsigned int i = 0; i < m_gameWorldLayerList.size(); i++)
	{
		if (m_gameWorldLayerList[i]->getParallaxFactor() >= 1.0f)
			target.draw(*m_gameWorldLayerList[i]);
	}

	//Get the camera view-projection transform and set it to render state
	sf::RenderStates renderState;
	renderState.transform = m_camera.getProjTransform() * m_camera.getViewTransform();


	//Render each zombie object in the list
	for (unsigned int i = 0; i < m_zombieCharacterList.size(); i++)
		target.draw(*m_zombieCharacterList[i], renderState);

	//Render bullets
	for (unsigned int i = 0; i < m_bullets.size(); i++)
		target.draw(*m_bullets[i], renderState);

	target.draw(*m_playerCharacter, renderState);

	//Render each game world layer with parallax factor
	for (unsigned int i = 0; i < m_gameWorldLayerList.size(); i++)
	{
		if (m_gameWorldLayerList[i]->getParallaxFactor() < 1.0f)
			target.draw(*m_gameWorldLayerList[i]);
	}

	target.draw(*m_hud);

	if (m_paused)
		target.draw(m_pauseMenu);
}

bool SFML_GameWorld::layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b)
{
	return (a->getParallaxFactor() > b->getParallaxFactor());
}

void SFML_GameWorld::MeleeAttack()
{
	for (auto zombieIt = m_zombieCharacterList.begin(); zombieIt != m_zombieCharacterList.end(); )
	{
		if (m_playerCharacter->getGlobalBounds().intersects((*zombieIt)->getGlobalBounds()))
		{
			(*zombieIt)->setTintColour(sf::Color::Red);
			if (m_playerCharacter->getCurrentAnimation()->animationCompleted())
			{
				(*zombieIt)->m_zombieHealth--;
			}
			break;
		}
		else
		{
			zombieIt++;
		}
	}
}

void SFML_GameWorld::ResetVariables()
{

	for (int i = 0; i < m_zombieNumber; i++)
	{
		delete m_zombieCharacterList[i];
	}
	m_zombieCharacterList.clear();

	m_zombieNumber = 50;

	//NPC animations
	for (int i = 0; i < m_zombieNumber; i++)
	{
		SFML_NPCSpriteObject* npcSprite = new SFML_NPCSpriteObject();
		npcSprite->setIdleAnimation("Media/Textures/zombie-idle.png", "Media/SpriteInfo/zombie-idle.txt", 1.0f);
		npcSprite->setWalkingAnimation("Media/Textures/zombie-move.png", "Media/SpriteInfo/zombie-move.txt", 1.0f);
		npcSprite->setAttackingAnimation("Media/Textures/zombie-attack.png", "Media/SpriteInfo/zombie-attack.txt", 1.0f);
		npcSprite->setBloodAnimation("Media/Textures/blood.png", "Media/SpriteInfo/blood.txt", 1.0f);

		npcSprite->setPosition(rand() % 10000 - 1000, rand() % 10000 - 1000);
		npcSprite->setTargetLocation(sf::Vector2f(rand() % 10000 - 1000, rand() % 10000 - 1000));
		m_zombieCharacterList.push_back(npcSprite);
	}

	m_playerCharacter->SetHealth();
}

