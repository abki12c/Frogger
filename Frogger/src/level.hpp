#pragma once

#include "moving_object.hpp"
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

enum class BlockType { Water, Grass, Home, HomeGoal, Road };

struct Block {
	Box box;
	BlockType type;
};

class Level : public GameObject
{
private:
	graphics::Brush m_brush_time_green, m_brush_time_orange, m_brush_time_red;
	graphics::Brush m_brush_text;
	graphics::Brush m_brush_frog_safe;
	graphics::Brush m_brush_lives;
	graphics::Brush m_brush_score;

	const float m_total_time = 60.0f; // Total time in seconds
	float m_remaining_time = 60.0f; // Remaining time in seconds
	float m_levelCompleteSoundTimer = 0.0f; // Timer to delay the main music

	bool m_playedTimeRunsOutSound = false;
	bool m_levelCompleteSoundPlayed = false;

	std::list<MovingObject*> m_dynamic_objects;
	std::array<bool,4> m_visited_goals;
	
	// collidable blocks
	std::vector<Block> m_blocks;
	const float m_block_size = 79.0f;
	std::unordered_map<BlockType, graphics::Brush> m_blockBrushes;
	graphics::Brush m_block_brush_debug;

	std::array<Lane, 8> lanes;

	void spawnMovingObjects();
	void removeInactiveObjects();
	void readFile(const std::string& filename);
	void drawBlock(int i);
	void checkCollisions();

public:
	void parseJson();
	void resetLevel();
	void update(float dt) override;
	void draw() override;
	void init() override;	
	Level(const std::string& name = "Level0");
	~Level();
};