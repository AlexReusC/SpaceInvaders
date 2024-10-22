#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}

	for (auto& newEntity : m_entitiesToAdd)
	{
		m_entities.push_back(newEntity);
		m_entityMap[newEntity->tag()].push_back(newEntity);
	}
	m_entitiesToAdd.clear();
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}


void EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto it = std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e) {return !(e->isActive());});
	vec.erase(it, vec.end());
}



const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}