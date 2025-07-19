#include "gameobject.hpp"
#include "gamestate.hpp"

GameObject::GameObject(const std::string& name) : m_name(name), m_state(GameState::getInstance()), m_id(m_next_id++) {}

int GameObject::m_next_id = 1;