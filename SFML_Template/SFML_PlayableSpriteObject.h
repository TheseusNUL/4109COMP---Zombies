#pragma once
#include "SFML_AnimationSpriteObject.h"
#include "SFML_BulletSpriteObject.h"

class SFML_PlayableSpriteObject :
	public SFML_AnimationSpriteObject
{
public:
	SFML_PlayableSpriteObject();
	virtual ~SFML_PlayableSpriteObject();

	virtual void update(float elapsedTime);
	SFML_BulletSpriteObject* processEvents(float elapsedTime, sf::Transform transform, sf::Vector2f mouseScreenPosition);

	int		setFlashlightIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setFlashlightWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setFlashlightMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);

	int		setKnifeIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setKnifeWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setKnifeMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);

	int		setPistolIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setPistolWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setPistolMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setPistolShootAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setPistolReloadAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);

	int		setRifleIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setRifleWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setRifleMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setRifleShootAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);
	int		setRifleReloadAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime);

	bool	toIdleState();
	bool	toWalkingState();
	bool	toMeleeState();
	bool	toShootState();
	bool	toReloadState();

	void	equipFlashlight();
	void	equipKnife();
	void	equipPistol();
	void	equipRifle();

	void	setSpeed(float speed);

	void	changeWeapon();

	int		GetBulletCount() { return m_bulletCount; }
	void	DecrementBulletCount() { m_bulletCount--; }
	void	ReloadBullets(){m_bulletCount = 20; }

	int		GetHealth() { return m_playerHealth; }
	void	SetHealth() { m_playerHealth = 10; }
	void	HurtPlayer() { m_playerHealth--; }

	enum EquippedWeapon
	{
		FLASHLIGHT,
		KNIFE,
		PISTOL,
		RIFLE
	};

	EquippedWeapon GetEquippedWeapon()
	{
		return m_equippedWeapon;
	}

	enum PlayerAction
	{
		IDLE,
		WALKING,
		MELEE,
		SHOOT,
		RELOAD
	};

	PlayerAction GetPlayerAction()
	{
		return m_playerAction;
	}

private:

	float			m_moveSpeed;
	sf::Vector2f	m_faceDirection;
	sf::Texture		m_bulletTexture;

	PlayerAction	m_playerAction;
	EquippedWeapon	m_equippedWeapon;

	int				m_flashlightIdleAnimationID;
	int				m_flashlightWalkingAnimationID;
	int				m_flashlightMeleeAnimationID;

	int				m_knifeIdleAnimationID;
	int				m_knifeWalkingAnimationID;
	int				m_knifeMeleeAnimationID;

	int				m_pistolIdleAnimationID;
	int				m_pistolWalkingAnimationID;
	int				m_pistolMeleeAnimationID;
	int				m_pistolShootAnimationID;
	int				m_pistolReloadAnimationID;

	int				m_rifleIdleAnimationID;
	int				m_rifleWalkingAnimationID;
	int				m_rifleMeleeAnimationID;
	int				m_rifleShootAnimationID;
	int				m_rifleReloadAnimationID;

	bool			m_readyToShoot;
	float			m_rangeWeaponCooldown;
	float			m_timeSinceLastShoot;

	int				m_bulletCount;

	int				m_playerHealth;

	SFML_BulletSpriteObject* shoot();
};

