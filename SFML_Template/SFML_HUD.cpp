#include "SFML_HUD.h"

SFML_HUD::SFML_HUD(SFML_PlayableSpriteObject& playerObject, int windowWidth, int windowHeight)
	: m_WeaponTextures(4)
	, m_player(playerObject)
	, m_playerHealthSprite(10)
	, m_bulletSprite(20)
{
	//HUD
	m_hudTexture.loadFromFile("Media/Textures/hud.png");
	m_hudSprite.setTexture(m_hudTexture);
	m_hudSprite.setPosition(0, 0);

	//Solider
	m_soldierTexture.loadFromFile("Media/Textures/HUD_Soldier.png");
	m_soldierSprite.setTexture(m_soldierTexture);
	m_soldierSprite.setPosition(100, (windowHeight - m_soldierSprite.getGlobalBounds().height) - 10);

	//Weapons
	m_WeaponTextures[0].loadFromFile("Media/Textures/HUD_Flashlight.png");
	m_WeaponTextures[1].loadFromFile("Media/Textures/HUD_Knife.png");
	m_WeaponTextures[2].loadFromFile("Media/Textures/HUD_Pistol.png");
	m_WeaponTextures[3].loadFromFile("Media/Textures/HUD_Rifle.png");

	m_WeaponSprite.setTexture(m_WeaponTextures[0]);

	//Health system
	int healthSpacing = 10;

	m_playerHealthTexture.loadFromFile("Media/Textures/HUD_Health.png");
	m_playerHealthSprite[0].setTexture(m_playerHealthTexture);

	float healthIconWidth = m_playerHealthSprite[0].getGlobalBounds().width;
	float healthIconHeight = m_playerHealthSprite[0].getGlobalBounds().height;

	m_playerHealthSprite[0].setPosition(windowWidth - ((healthIconWidth + healthSpacing) * m_playerHealthSprite.size()), (windowHeight - healthIconHeight) - healthSpacing);
	
	for (int i = 1; i < m_playerHealthSprite.size(); i++)
	{
		m_playerHealthSprite[i].setTexture(m_playerHealthTexture);
		m_playerHealthSprite[i].setPosition((m_playerHealthSprite[i - 1].getPosition().x + m_playerHealthSprite[i - 1].getGlobalBounds().width) + healthSpacing , m_playerHealthSprite[0].getPosition().y);
	}

	//Bullet System
	int bulletSpacing = 5;

	m_bulletTexture.loadFromFile("Media/Textures/bullet.png");
	m_bulletSprite[0].setTexture(m_bulletTexture);

	float bulletIconWidth = m_bulletSprite[0].getGlobalBounds().width;
	float bulletIconHeight = m_bulletSprite[0].getGlobalBounds().height;

	m_bulletSprite[0].setPosition((m_soldierSprite.getPosition().x / 2) - (m_bulletSprite[0].getGlobalBounds().width / 2), windowHeight - 50);

	for (int i = 1; i < m_bulletSprite.size(); i++)
	{
		m_bulletSprite[i].setTexture(m_bulletTexture);
		m_bulletSprite[i].setPosition(m_bulletSprite[0].getPosition().x, m_bulletSprite[i - 1].getPosition().y - m_bulletSprite[i - 1].getGlobalBounds().height - bulletSpacing);
	}
}

SFML_HUD::~SFML_HUD()
{

}

void SFML_HUD::update(int windowHeight)
{
	//Sets textures based on which weapon is equipped.
	switch (m_player.GetEquippedWeapon())
	{
	case SFML_PlayableSpriteObject::FLASHLIGHT:
		m_WeaponSprite.setTexture(m_WeaponTextures[0]);
		m_spriteTextureSize = m_WeaponTextures[0].getSize();
		break;
	case SFML_PlayableSpriteObject::KNIFE:
		m_WeaponSprite.setTexture(m_WeaponTextures[1]);
		m_spriteTextureSize = m_WeaponTextures[1].getSize();
		break;
	case SFML_PlayableSpriteObject::PISTOL:
		m_WeaponSprite.setTexture(m_WeaponTextures[2]);
		m_spriteTextureSize = m_WeaponTextures[2].getSize();
		break;
	case SFML_PlayableSpriteObject::RIFLE:
		m_WeaponSprite.setTexture(m_WeaponTextures[3]);
		m_spriteTextureSize = m_WeaponTextures[3].getSize();
		break;
	}

	m_WeaponSprite.setTextureRect(sf::IntRect(0, 0, m_spriteTextureSize.x, m_spriteTextureSize.y));
	m_WeaponSprite.setPosition((m_soldierSprite.getPosition().x + m_soldierSprite.getLocalBounds().width) + 20, (windowHeight - m_WeaponSprite.getGlobalBounds().height) - 10);
	//position next to m_soldierSpirte
}

void SFML_HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_hudSprite);
	target.draw(m_WeaponSprite);
	target.draw(m_soldierSprite);

	for (int i = 0; i < m_player.GetHealth(); i++)
	{
		target.draw(m_playerHealthSprite[i]);
	}

	for (int i = 0; i < m_player.GetBulletCount(); i++)
	{
		target.draw(m_bulletSprite[i]);
	}
}

