#include "entity.hpp"

Entity::Entity(const Vec2i &position) : m_Position(position) {}
Vec2i Entity::GetPosition() const { return m_Position; }
Entity::~Entity(){};
