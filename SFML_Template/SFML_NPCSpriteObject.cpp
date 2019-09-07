#include "SFML_NPCSpriteObject.h"


SFML_NPCSpriteObject::SFML_NPCSpriteObject()
	: m_targetLocation(0, 0)
	, m_moveSpeed(100.0f)
	, m_state(IDLE)
	, m_idleAnimationID(-1)
	, m_walkingAnimationID(-1)
	, m_attackingAnimationID(-1)
	, m_visionConeAngle(90.0f)
	, m_visionRange(1200.0f)
	, m_hearingRadius(750.0f)
	, m_zombieHealth(5)
{

}

SFML_NPCSpriteObject::~SFML_NPCSpriteObject()
{
}

int SFML_NPCSpriteObject::setIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_idleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_idleAnimationID;
}

int	SFML_NPCSpriteObject::setWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_walkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_walkingAnimationID;
}

int SFML_NPCSpriteObject::setAttackingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_attackingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_attackingAnimationID]->setLoop(false);
	return m_attackingAnimationID;
}

int SFML_NPCSpriteObject::setBloodAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_bloodAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_bloodAnimationID]->setLoop(false);
	return m_bloodAnimationID;
}

void SFML_NPCSpriteObject::toIdleState()
{
	m_state = IDLE;
	setCurrentAnimation(m_idleAnimationID);
}

void SFML_NPCSpriteObject::toWalkingState()
{
	m_state = WALKING;
	setCurrentAnimation(m_walkingAnimationID);
}

void SFML_NPCSpriteObject::toAttackingState()
{
	m_state = ATTACKING;
	setCurrentAnimation(m_attackingAnimationID);
}

void SFML_NPCSpriteObject::toDeadState()
{
	m_state = DEAD;
	setCurrentAnimation(m_bloodAnimationID);
}

bool SFML_NPCSpriteObject::isInVision(sf::Vector2f targetpoint)
{
	const float PI = 3.1415f;

	//Get the NPC's current location
	m_currentPosition = getPosition();

	//Calculate the vector from the NPC's current position to the target point
	sf::Vector2f m_vector = targetpoint - m_currentPosition;

	//Calculate the distance (L2-norm) between the current location and the target position
	float distance = sqrt(m_vector.x * m_vector.x + m_vector.y * m_vector.y);

	//Check if distance is outside the vision range
	if (distance > m_visionRange)
		return false;

	//Get the NPC's orientation. Note that getRotation() always produce angle in degree and in 0 - 360 range
	float faceAngle = getRotation();

	//Calculate the angle of the vector
	float targetAngle = atan2(m_vector.y, m_vector.x);

	//Convert to degree
	targetAngle = targetAngle * 180 / PI;

	//the angle could be outside 0 -  360 range, so it needs to be converted.
	while (targetAngle > 360)
		targetAngle = targetAngle - 360;
	while (targetAngle < 0)
		targetAngle = targetAngle + 360;

	//Calculate the absolute difference between targetAngle and faceAngle
	float angleDifference = fabs(targetAngle - faceAngle);

	//If within half of the vision cone angle then it is within the vision cone
	if (angleDifference <= m_visionConeAngle / 2)
		return true;
	else
		return false;
}

bool SFML_NPCSpriteObject::isInHearingRange(sf::Vector2f targetpoint)
{
	m_currentPosition = getPosition();

	m_vector = targetpoint - m_currentPosition;

	float distance = sqrt(m_vector.x * m_vector.x + m_vector.y * m_vector.y);

	if (distance > m_hearingRadius)
		return false;
}

void SFML_NPCSpriteObject::setTargetLocation(sf::Vector2f targetLocation)
{
	m_targetLocation = targetLocation;
}

void SFML_NPCSpriteObject::setMovementSpeed(float speed)
{
	m_moveSpeed = speed;
}

void SFML_NPCSpriteObject::update(float elapsedTime)
{
	const float PI = 3.14515f;

	if (m_state != DEAD)
	{
		sf::Vector2f currentPosition = getPosition();

		//Calculate the vector from the NPC's current Position to the target point
		sf::Vector2f faceDirection = m_targetLocation - currentPosition;

		//Calculate the distance (L2-norm) between the current location and the target position
		float distance = sqrt(faceDirection.x * faceDirection.x + faceDirection.y * faceDirection.y);

		/*If the distance is less than the displacement to cover in this frame then the npc has reached it's destination
		So set the animation to idle. Otherwise it needs to keep moving.*/
		if (distance < elapsedTime * m_moveSpeed)
		{
			setPosition(m_targetLocation);

			if (m_state != ATTACKING)
				toIdleState();
		}
		else
		{
			//Rotate the npc to face the target rotation
			float angle = atan2(faceDirection.y, faceDirection.x);
			setRotation(angle / PI * 180);

			//Calculate the movement vector
			sf::Vector2f unitMoveVector = sf::Vector2f(cos(angle), sin(angle));
			sf::Vector2f totalMoveVector = unitMoveVector * elapsedTime * m_moveSpeed;
			move(totalMoveVector);

			if (m_state != ATTACKING)
				toWalkingState();
		}
	}

	SFML_AnimationSpriteObject::update(elapsedTime);
}
