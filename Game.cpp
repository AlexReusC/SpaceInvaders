#include "Game.h"

#include <iostream>
#include <fstream>
#include <math.h>

#define PI 3.14159265

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& config)
{
	//Read config file
	std::ifstream fin(config);
	std::string placeholder;
	int FL, FS;
	fin >> placeholder >> m_windowConfig.WW >> m_windowConfig.WH >> FL >> FS;
	fin >> placeholder >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
	fin >> placeholder  >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
	fin >> placeholder  >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L >> m_bulletConfig.S;


	m_window.create(sf::VideoMode(m_windowConfig.WW, m_windowConfig.WH), "Space Invaders");
	m_window.setFramerateLimit(FL);

	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sLifeSpan();
		sUserInput();
		sRender();

		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{
	m_player->CTransform->velocity = { 0.0f, 0.0f };
	if (m_player->CInput->up)
	{
		m_player->CTransform->velocity.y = -2;

	}
	if (m_player->CInput->left)
	{
		m_player->CTransform->velocity.x = -2;

	}
	if (m_player->CInput->down)
	{
		m_player->CTransform->velocity.y = 2;

	}
	if (m_player->CInput->right)
	{
		m_player->CTransform->velocity.x = 2;

	}

	for (auto e : m_entities.getEntities())
	{
		e->CTransform->pos.x += e->CTransform->velocity.x;
		e->CTransform->pos.y += e->CTransform->velocity.y;
	}

	for (auto e : m_entities.getEntities("specialWpn"))
	{
		e->CTransform->pos.x -= 1;
		e->CTransform->pos.y -= 1;
	}

	for (auto e : m_entities.getEntities("enemy"))
	{
		if (e->CTransform->pos.x + m_enemyConfig.CR > m_windowConfig.WW || e->CTransform->pos.x - m_enemyConfig.CR < 0)
		{
			e->CTransform->velocity.x *= -1;
		}
		if (e->CTransform->pos.y + m_enemyConfig.CR > m_windowConfig.WH || e->CTransform->pos.y - m_enemyConfig.CR < 0)
		{
			e->CTransform->velocity.y *= -1;
		}
	}


}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::W:
					m_player->CInput->up = true;
					break;
				case sf::Keyboard::A:
					m_player->CInput->left = true;
					break;
				case sf::Keyboard::S:
					m_player->CInput->down = true;
					break;
				case sf::Keyboard::D:
					m_player->CInput->right = true;
					break;
				default:
					break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->CInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->CInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->CInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->CInput->right = false;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				spawnSpecialWeapon();
			}
		}
	}
}

void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities("bullet"))
	{
		e->CLifeSpan->remaining--;
		if (e->CLifeSpan->remaining <= 0)
		{
			e->destroy();
		}
	}

	for (auto e : m_entities.getEntities("smallEnemy"))
	{
		e->CLifeSpan->remaining--;
		if (e->CLifeSpan->remaining <= 0)
		{
			e->destroy();
		}
	}

	for (auto e : m_entities.getEntities("specialWpn"))
	{
		e->CLifeSpan->remaining--;
		if (e->CLifeSpan->remaining <= 0)
		{
			e->destroy();
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	m_player->CShape->circle.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);

	m_player->CTransform->angle += 1.0f;
	m_player->CShape->circle.setRotation(m_player->CTransform->angle);
	m_window.draw(m_player->CShape->circle);

	for (auto enemy : m_entities.getEntities("enemy"))
	{
		enemy->CShape->circle.setPosition(enemy->CTransform->pos.x, enemy->CTransform->pos.y);

		enemy->CTransform->angle += 1.0f;
		enemy->CShape->circle.setRotation(m_player->CTransform->angle);
		m_window.draw(enemy->CShape->circle);
	}

	for (auto bullet : m_entities.getEntities("bullet"))
	{
		bullet->CShape->circle.setPosition(bullet->CTransform->pos.x, bullet->CTransform->pos.y);
		auto alpha = bullet->CLifeSpan->remaining > 0 ? (bullet->CLifeSpan->remaining * 0xFF) / bullet->CLifeSpan->total : 0;
		bullet->CShape->circle.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, alpha));

		bullet->CTransform->angle += 1.0f;
		m_window.draw(bullet->CShape->circle);
	}

	for (auto specialWpn : m_entities.getEntities("specialWpn"))
	{
		specialWpn->CShape->circle.setPosition(specialWpn->CTransform->pos.x, specialWpn->CTransform->pos.y );
		specialWpn->CShape->circle.setRadius(specialWpn->CShape->circle.getRadius() + 1);
		auto alpha = specialWpn->CLifeSpan->remaining > 0 ? (specialWpn->CLifeSpan->remaining * 0xFF) / specialWpn->CLifeSpan->total : 0;
		specialWpn->CShape->circle.setOutlineColor(sf::Color(0xFF, 0, 0, alpha));


		m_window.draw(specialWpn->CShape->circle);
	}

	for (auto smallEnemy : m_entities.getEntities("smallEnemy"))
	{
		smallEnemy->CShape->circle.setPosition(smallEnemy->CTransform->pos.x, smallEnemy->CTransform->pos.y);

		smallEnemy->CTransform->angle += 1.0f;
		auto alpha = smallEnemy->CLifeSpan->remaining > 0 ? (smallEnemy->CLifeSpan->remaining * 0xFF) / smallEnemy->CLifeSpan->total : 0;
		smallEnemy->CShape->circle.setFillColor(sf::Color(smallEnemy->CShape->circle.getFillColor().r, smallEnemy->CShape->circle.getFillColor().g, smallEnemy->CShape->circle.getFillColor().b, alpha));

		smallEnemy->CShape->circle.setRotation(smallEnemy->CTransform->angle);
		m_window.draw(smallEnemy->CShape->circle);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	if ( m_currentFrame - m_lastEnemySpawnTime >= 220)
	{
		spawnEnemy();
		m_currentFrame = m_lastEnemySpawnTime;
	}
}

void Game::sCollision()
{
	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{ 
			auto distance = b->CTransform->pos - e->CTransform->pos;
			auto bulletRad = b->CShape->circle.getRadius();
			auto enemyRad = m_enemyConfig.CR;
			if ( std::pow(distance.x, 2) + std::pow(distance.y, 2) < std::pow(bulletRad + enemyRad, 2))
			{
				spawnSmallEnemies(e);
				b->destroy();
				e->destroy();
			}
		}
	}

	for (auto e : m_entities.getEntities("enemy"))
	{
		auto distance = e->CTransform->pos - m_player->CTransform->pos;
		auto enemyRad = e->CShape->circle.getRadius();
		auto playerRad = m_enemyConfig.CR;
		if (std::pow(distance.x, 2) + std::pow(distance.y, 2) < std::pow(enemyRad + playerRad, 2))
		{
			m_player->destroy();
			spawnPlayer();
		}
	}

	for (auto b : m_entities.getEntities("specialWpn"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			auto distance = b->CTransform->pos - e->CTransform->pos;
			auto bulletRad = b->CShape->circle.getRadius();
			auto enemyRad = m_enemyConfig.CR;
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) < std::pow(bulletRad + enemyRad, 2))
			{
				spawnSmallEnemies(e);
				b->destroy();
				e->destroy();
			}
		}
	}
}

void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");

	float ex = m_window.getSize().x / 2;
	float ey = m_window.getSize().y / 2;

	entity->CTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(0.0f, 0.0f), 0.0f);
	entity->CShape = std::make_shared<CShape>(32.0f, 8, sf::Color(0, 0, 0, 0), sf::Color(0xFF, 0, 0), 4.0f);
	entity->CInput = std::make_shared<CInput>();

	m_player = entity;
}

void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	int diffPx = 1 + m_window.getSize().x - (m_enemyConfig.SR * 2);
	int diffPy = 1 + m_window.getSize().y - (m_enemyConfig.SR * 2);
	float px = (rand() % diffPx) + m_enemyConfig.SR;
	float py = (rand() % diffPy) + m_enemyConfig.SR;

	int diffV = 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN;
	float v = (rand() % diffV) + m_enemyConfig.SMIN;
	float angle = rand() % 361;

	float vx = v * std::cos(angle * PI / 180);
	float vy = v * std::sin(angle * PI / 180);

	entity->CTransform = std::make_shared<CTransform>(Vec2(px, py), Vec2(vx, vy), 0.0f);
	int red = rand() % 0xFF;
	int green = rand() % 0xFF;
	int blue = rand() % 0xFF;
	int vertices = (rand() % 5) + 3;
	entity->CShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, sf::Color(red, green, blue), sf::Color(0xFF, 0xFF, 0xFF), 2.0f);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	for (int i = 0; i < entity->CShape->circle.getPointCount(); i++)
	{
		auto smallEnemy = m_entities.addEntity("smallEnemy");
		int angle = i * (360 / entity->CShape->circle.getPointCount());

		smallEnemy->CTransform = std::make_shared<CTransform>(Vec2(entity->CTransform->pos), Vec2(6 * std::cos(angle * PI / 180), 6 * std::sin(angle * PI / 180)), 0.0f);
		smallEnemy->CShape = std::make_shared<CShape>(15, entity->CShape->circle.getPointCount(), entity->CShape->circle.getFillColor(), sf::Color(0, 0, 0), 2);
		smallEnemy->CLifeSpan = std::make_shared<CLifespan>(15);
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bullet = m_entities.addEntity("bullet");
	auto normalizedVec = target.normalize(entity->CTransform->pos);

	bullet->CTransform = std::make_shared<CTransform>(
		m_player->CTransform->pos + (normalizedVec * m_player->CShape->circle.getRadius()),
		normalizedVec * 20,
		0);
	bullet->CShape = std::make_shared<CShape>(10, 8, sf::Color(0xFF, 0xFF, 0xFF), sf::Color(0, 0, 0), 2);
	bullet->CLifeSpan = std::make_shared<CLifespan>(60);
}

void Game::spawnSpecialWeapon()
{
	auto specialWpn = m_entities.addEntity("specialWpn");
	specialWpn->CTransform = std::make_shared<CTransform>(Vec2(m_player->CTransform->pos), Vec2(0, 0), 0);
	specialWpn->CShape = std::make_shared<CShape>(32.0f, 20, sf::Color(0, 0, 0, 0), sf::Color(0xFF, 0, 0), 4.0f);
	specialWpn->CLifeSpan = std::make_shared<CLifespan>(50);
}
