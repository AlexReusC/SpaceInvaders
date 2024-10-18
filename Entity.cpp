#include "Entity.h"

Entity::Entity(const size_t id, const std::string& tag)
    : m_id(id), m_tag(tag)
{
}

bool Entity::isActive() const
{
    return false;
}

const std::string& Entity::tag() const
{
    // TODO: insert return statement here
    return "";
}

const size_t Entity::id() const
{
    return size_t();
}

void Entity::destroy()
{
}
