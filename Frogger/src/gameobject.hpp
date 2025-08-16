#pragma once
#include <string>

class GameObject
{
	static int m_next_id;
protected:
	const class GameState* m_state;
	std::string m_name;
	int m_id = 0;
	bool m_active = true;	
public:
	GameObject(const std::string& name = "");
	virtual void update(float dt) = 0;
	virtual void init() = 0;
	virtual void draw() = 0;
	bool isActive() { return m_active; }
	void setActive(bool a) { m_active = a; }
	std::string getName() { return m_name; }
};