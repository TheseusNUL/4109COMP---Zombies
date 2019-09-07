#pragma once

#include "SFML_AnimationSpriteObject.h"
class SFML_NPCSpriteObject : public SFML_AnimationSpriteObject
{
public:
			SFML_NPCSpriteObject();
	virtual ~SFML_NPCSpriteObject();

	virtual void	update(float elapsedTime);

	void	setTargetLocation(sf::Vector2f targetLocation);
	void	setMovementSpeed(float speed);

	int		setIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setAttackingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setBloodAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);

	void	toIdleState();
	void	toWalkingState();
	void	toAttackingState();
	void	toDeadState();

	bool	isInVision(sf::Vector2f targetpoint);
	bool	isInHearingRange(sf::Vector2f targetpoint);

	int				m_zombieHealth;

	enum NPCState
	{
		IDLE,
		WALKING,
		ATTACKING,
		DEAD
	};

	NPCState getCurrentState()
	{
		return m_state;
	}

private:

	NPCState		m_state;
	
	sf::Vector2f	m_targetLocation;
	float			m_moveSpeed;
	int				m_idleAnimationID;
	int				m_walkingAnimationID;
	int				m_attackingAnimationID;

	int				m_bloodAnimationID;

	float			m_visionConeAngle;
	float			m_visionRange;

	float			m_hearingRadius;

	sf::Vector2f	m_currentPosition;
	sf::Vector2f	m_vector;
};

