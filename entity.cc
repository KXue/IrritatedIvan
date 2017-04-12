#include "entity.hpp"
#include <iostream>
Entity::Entity(const Vec2i &position, GameMap *map) : m_Position(position), m_pMap(map){}
Vec2i Entity::GetPosition() const { return m_Position; }
Entity::~Entity(){
  m_pMap = nullptr;
};
string Entity::Decide(){
  return "";
}
