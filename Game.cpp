#include "Game.h"

#include <iostream>
#include <fstream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& config)
{
	//Read config file
	m_window.create(sf::VideoMode(1280, 720), "Space Invaders");
	m_window.setFramerateLimit(60);

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
}

void Game::sUserInput()
{
}

void Game::sLifeSpan()
{
}

void Game::sRender()
{
	m_window.clear();

	m_player->CShape->circle.setPosition(m_player->CTransform->pos.x, m_player->CTransform->pos.y);

	m_player->CTransform->angle += 1.0f;
	m_player->CShape->circle.setRotation(m_player->CTransform->angle);
	m_window.draw(m_player->CShape->circle);

	m_window.display();
}

void Game::sEnemySpawner()
{
}

void Game::sCollision()
{
}

void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");
	entity->CTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);
	entity->CShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
	entity->CInput = std::make_shared<CInput>();

	m_player = entity;
}

void Game::spawnEnemy()
{
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
}

void Game::spawnBullets(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
