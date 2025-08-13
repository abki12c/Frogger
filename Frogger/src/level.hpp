#pragma once

#include "gameobject.hpp"
#include <vector>
#include <list>
#include <string>
#include <array>
#include <sgg/graphics.h>
#include "player.hpp"

struct Lane {
	int16_t y_position;
	float speed;
	float spawn_timer;
	float default_spawn_timer;
	float obj_width;
	float obj_height;
	std::string obj_sprite;
};

class Level : public GameObject
{
private:
	graphics::Brush m_brush_time;
	graphics::Brush m_brush_text;
	graphics::Brush m_brush_frog_safe;
	graphics::Brush m_brush_lives;
	graphics::Brush m_brush_score;

	const float m_total_time = 60.0f; // Total time in seconds
	float m_remaining_time = 60.0f; // Remaining time in seconds
	float m_levelCompleteSoundTimer = 0.0f; // Timer to delay the main music

	bool m_playedTimeRunsOutSound = false;
	bool m_levelCompleteSoundPlayed = false;

	std::list<GameObject*> m_dynamic_objects;
	std::array<bool,4> m_visited_goals;
	
	// collidable blocks
	std::vector<Box> m_blocks;
	std::vector<std::string> m_block_names;
	const float m_block_size = 79.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	std::array<Lane, 8> lanes;

	void spawnMovingObjects();
	void removeInactiveObjects();
	void readFile(const std::string& filename);
	void drawBlock(int i);
	void checkCollisions();

public:
	void resetLevel();
	void update(float dt) override;
	void draw() override;
	void init() override;	
	Level(const std::string& name = "Level0");
	~Level();
};