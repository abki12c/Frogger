#include <sgg/graphics.h>
#include "level.hpp"
#include "gamestate.hpp"
#include "util.hpp"
#include "config.hpp"
#include "vehicle.hpp"
#include <fstream>
#include <iostream>
#include "log.hpp"
#include "rapidjson/document.h"

void Level::spawnMovingObjects()
{
	// Update game objects in each lane
	for (auto& lane : m_lanes) {
		// Decrement spawn timer
		lane.spawn_timer -= graphics::getDeltaTime() / 1000.0f;

		// Check if it's time to spawn a new game object and under a certain limit
		if (lane.spawn_timer <= 0.0f ) {
			// Determine spawn position based on speed
			float x_position = (lane.speed > 0) ? 0.f : CANVAS_WIDTH;

			MovingObject* game_object;
			if(lane.obj_sprite.find("L",0) == 0) {
				Log* log = new Log("log", x_position, lane.y_position, lane.speed, lane.obj_width, lane.obj_height, lane.obj_sprite);
				game_object = log;
			} else {
				Vehicle* new_vehicle = new Vehicle("vehicle", x_position, lane.y_position, lane.speed, lane.obj_width, lane.obj_height, lane.obj_sprite);
				game_object = new_vehicle;
			}


			// Add the game object to the game objects
			m_dynamic_objects.push_back(game_object);

			// Reset the spawn timer for this lane
			lane.spawn_timer = lane.default_spawn_timer;
		}
	}
}

void Level::removeInactiveObjects()
{
	// Remove inactive game objects
	m_dynamic_objects.erase(
		std::remove_if(m_dynamic_objects.begin(), m_dynamic_objects.end(), [](MovingObject* game_object) {
			if (game_object && !game_object->isActive()) {
				delete game_object;
				return true;
			}
			return false;
			}),
		m_dynamic_objects.end());
}

void Level::readFile(const std::string& filename)
{
	std::ifstream inFile(m_state->getFullAssetPath(filename));

	if (!inFile.is_open()) {
		std::cerr << "Error: Unable to open file " << filename << std::endl;
		return;
	}

	int y = 0;
	std::string line;
	while (std::getline(inFile, line)) {
		for (int x = 0; x < line.length(); ++x) {
			float block_x = x * m_block_size;
			float block_y = y * m_block_size;
			Block new_block;
			new_block.box = Box(block_x, block_y, m_block_size, m_block_size);

			switch (line[x]) {
				case '~':
					new_block.type = BlockType::Water;
					break;
				case '-':
					new_block.type = BlockType::Grass;
					break;
				case '*':
					new_block.type = BlockType::Home;
					break;
				case 'R':
					new_block.type = BlockType::HomeGoal;
					break;
				case 'x':
					new_block.type = BlockType::Road;
					break;
			}
			m_blocks.push_back(new_block);
		}
		y++;
	}

	inFile.close();
}


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i].box;
	BlockType blockType = m_blocks[i].type;

	float x = box.m_pos_x + 35;
	float y = box.m_pos_y + 25;

	graphics::drawRect(x, y, m_block_size, m_block_size, m_blockBrushes[blockType]);

	if (m_state->getDebugMode())
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

	// Draw additional sprite for visited home_goal.png
    if (m_blocks[i].type == BlockType::HomeGoal && (i==2 || i==5 || i==8 || i==11) && m_visited_goals[(i-2)/3]) {
		graphics::drawRect(x, y, m_block_size-10, m_block_size-10, m_brush_frog_safe);
	}

}

void Level::checkCollisions()
{
	m_state->getPlayer()->setIsOnLog(false);

	// Check collisions with dynamic objects
	for (auto& gameObject : m_dynamic_objects) {
		if (gameObject && gameObject->isActive()) {
			if (gameObject->getType() == ObjectType::vehicle) {
				Vehicle* vehicle = static_cast<Vehicle*>(gameObject);

				// Check for collision with vehicle
				if (m_state->getPlayer()->intersect(*vehicle)) {
					graphics::playSound(m_state->getFullAssetPath("sound/splat.wav"), 0.5f, false);
					m_state->getPlayer()->resetPlayer();
					m_state->getPlayer()->reduceLives();
					break;
				}
			} else {
				Log* log = static_cast<Log*>(gameObject);

				// Check for collision with log
				if (m_state->getPlayer()->intersect(*log)) {
					m_state->getPlayer()->setIsOnLog(true);
					m_state->getPlayer()->setOnLogSpeed(log->getSpeed());
					break;
				}
			}
		}
	}

	// check collision with blocks
	for (int i = 0; i < m_blocks.size(); i++) {
		if (m_blocks[i].type == BlockType::HomeGoal && m_state->getPlayer()->intersect(m_blocks[i].box)) {
			if (!m_visited_goals[(i - 2) / 3]) {
				m_visited_goals[(i - 2) / 3] = true;
				if (m_visited_goals[0] && m_visited_goals[1] && m_visited_goals[2] && m_visited_goals[3]) {
					// collected all frogs
					m_state->getInstance()->updateScore(50);
					m_visited_goals.fill(false);

					if (!m_levelCompleteSoundPlayed) {
						graphics::stopMusic();
						graphics::playMusic(m_state->getFullAssetPath("sound/songLevelComplete.mp3"), 0.5f, false);
						m_levelCompleteSoundPlayed = true;
						m_levelCompleteSoundTimer = 0.0f;
					}
					
				}
				m_state->getInstance()->updateScore(20);

				
				m_state->getPlayer()->resetPlayer();
				m_remaining_time = m_total_time;
			}else {
				graphics::playSound(m_state->getFullAssetPath("sound/squash.wav"), 0.5f, false);
				m_state->getPlayer()->resetPlayer();
				m_state->getPlayer()->reduceLives();
			}
		} else if (m_blocks[i].type == BlockType::Home && m_state->getPlayer()->intersect(m_blocks[i].box)) {
			graphics::playSound(m_state->getFullAssetPath("sound/squash.wav"), 0.5f, false);
			m_state->getPlayer()->resetPlayer();
			m_state->getPlayer()->reduceLives();
		} else if (m_blocks[i].type == BlockType::Water && !m_state->getPlayer()->getIsOnLog() && m_state->getPlayer()->intersect(m_blocks[i].box)) {
			graphics::playSound(m_state->getFullAssetPath("sound/splash.wav"), 1.0f, false);
			m_state->getPlayer()->resetPlayer();
			m_state->getPlayer()->reduceLives();
		}
	}
}

void Level::resetLevel()
{
	m_visited_goals.fill(false);

	m_remaining_time = 60.0f;
	m_levelCompleteSoundTimer = 0.0f;

	m_playedTimeRunsOutSound = false;
	m_levelCompleteSoundPlayed = false;
}

void Level::parseJson() {
	std::ifstream file(m_state->getFullAssetPath("lanes.json"));

	// Read the entire file into a string
	std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document doc;
	doc.Parse(json.c_str());

	if (doc.HasParseError()) {
		std::cerr << "Error parsing JSON: "
			<< doc.GetParseError() << std::endl;
		return;
	}

	for (int i = 0; i < 8; i++) {
		m_lanes[i].y_position = (int16_t)doc["lanes"][i]["y_position"].GetInt();
		m_lanes[i].speed = doc["lanes"][i]["speed"].GetFloat();
		m_lanes[i].spawn_timer = doc["lanes"][i]["spawn_timer"].GetFloat();
		m_lanes[i].default_spawn_timer = doc["lanes"][i]["default_spawn_timer"].GetFloat();
		m_lanes[i].obj_width = doc["lanes"][i]["obj_width"].GetFloat();
		m_lanes[i].obj_height = doc["lanes"][i]["obj_height"].GetFloat();
		m_lanes[i].obj_sprite = doc["lanes"][i]["obj_sprite"].GetString();
	}
}


void Level::update(float dt)
{
	// update player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	removeInactiveObjects();
	spawnMovingObjects();

	// Update game objects
	for (auto& game_object : m_dynamic_objects) {
		if (game_object) {
			game_object->update(dt);
		}
	}

	checkCollisions();

	// Decrease remaining time
	m_remaining_time -= dt / 1000.0f;
	if (m_remaining_time < 0) {
		m_remaining_time = m_total_time; // reset timer
		m_playedTimeRunsOutSound = false;
		m_state->getPlayer()->reduceLives();
		m_state->getPlayer()->resetPlayer();
		if (m_state->getPlayer()->getLives() > 1) {
			graphics::playSound(m_state->getFullAssetPath("sound/squash.wav"), 0.5f, false);
		}
	}

	if (m_remaining_time < m_total_time * 0.3f && !m_playedTimeRunsOutSound) {
		graphics::playSound(m_state->getFullAssetPath("sound/runningOutOfTime.wav"), 0.5f, false);
		m_playedTimeRunsOutSound = true;
	}

	// game over
	if (m_state->getPlayer()->getLives() == 0) {
		graphics::stopMusic();
		m_state->getInstance()->setStatus(STATUS_GAME_OVER);
		graphics::playSound(m_state->getFullAssetPath("sound/game-over.wav"),0.5f,false);
	}


	// Handle level completion music delay
	if (m_levelCompleteSoundPlayed) {
		m_levelCompleteSoundTimer += dt / 1000.0f;
		if (m_levelCompleteSoundTimer >= 6.0f) {
			graphics::playMusic(m_state->getFullAssetPath("sound/main-song.mp3"), 0.5f, true);
			m_levelCompleteSoundPlayed = false;
		}
	}
}

void Level::draw()
{	
	// draw blocks
	for (int i = 0; i < m_blocks.size(); i++) {
		drawBlock(i);
	}

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->draw();
	

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	// draw lives
	graphics::drawText(110, CANVAS_HEIGHT - 60, 30.f, "Lives", m_brush_text);
	unsigned char lives = m_state->getPlayer()->getLives();
	for (int i = 0; i < lives;i++) {
		graphics::drawRect(70.f + 40.f*i, CANVAS_HEIGHT - 35, 30.f, 30.f, m_brush_lives);
	}

	// Draw time bar
	float time_bar_width = (m_remaining_time / m_total_time) * 100.0f; // Full width = 100 units
	float time_bar_x = CANVAS_WIDTH - 70 - (100 - time_bar_width) / 2; // Centering
	graphics::drawText(CANVAS_WIDTH - 100, CANVAS_HEIGHT - 60, 30.f, "Time", m_brush_text);

	if (m_remaining_time < m_total_time * 0.3f) {
		graphics::drawRect(time_bar_x, CANVAS_HEIGHT - 40, time_bar_width, 20.f, m_brush_time_red);
	} else if (m_remaining_time < m_total_time * 0.6f) {
		graphics::drawRect(time_bar_x, CANVAS_HEIGHT - 40, time_bar_width, 20.f, m_brush_time_orange);
	} else {
		graphics::drawRect(time_bar_x, CANVAS_HEIGHT - 40, time_bar_width, 20.f, m_brush_time_green);
	}

	// Draw Score
	graphics::drawText(CANVAS_WIDTH/2, CANVAS_HEIGHT - 60, 30.f, "Score", m_brush_text);
	graphics::drawText(CANVAS_WIDTH/2, CANVAS_HEIGHT - 25, 30.f, std::to_string(m_state->getInstance()->getScore()), m_brush_score);
}

void Level::init()
{
	readFile("level.txt");
	parseJson();

	graphics::Brush roadBrush;
	roadBrush.texture = m_state->getFullAssetPath("road.png");
	roadBrush.outline_opacity = 0.0f;

	graphics::Brush waterBrush;
	waterBrush.texture = m_state->getFullAssetPath("water.png");
	waterBrush.outline_opacity = 0.0f;

	graphics::Brush grassBrush;
	grassBrush.texture = m_state->getFullAssetPath("grass.png");
	grassBrush.outline_opacity = 0.0f;

	graphics::Brush homeBrush;
	homeBrush.texture = m_state->getFullAssetPath("home.png");
	homeBrush.outline_opacity = 0.0f;

	graphics::Brush homeGoalBrush;
	homeGoalBrush.texture = m_state->getFullAssetPath("home_goal.png");
	homeGoalBrush.outline_opacity = 0.0f;


	m_blockBrushes[BlockType::Road] = roadBrush;
	m_blockBrushes[BlockType::Water] = waterBrush;
	m_blockBrushes[BlockType::Grass] = grassBrush;
	m_blockBrushes[BlockType::Home] = homeBrush;
	m_blockBrushes[BlockType::HomeGoal] = homeGoalBrush;


	m_brush_time_green.outline_opacity = 0.0f;
	m_brush_time_orange.outline_opacity = 0.0f;
	m_brush_time_red.outline_opacity = 0.0f;

	SETCOLOR(m_brush_time_red.fill_color, 255.0f, 0.0f, 0.0f);
	SETCOLOR(m_brush_time_orange.fill_color, 255.0f, 165.0f, 0.0f);
	SETCOLOR(m_brush_time_green.fill_color, 0.0f, 128.0f, 0.0f);

	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.outline_color, 0.f, 0.f, 255.f);

	m_brush_text.outline_opacity = 0.0f;
	SETCOLOR(m_brush_text.fill_color, 255.f, 255.f, 255.f);

	m_brush_frog_safe.texture = m_state->getFullAssetPath("safe.png");
	m_brush_frog_safe.outline_opacity = 0.0f;

	m_brush_lives.texture = m_state->getFullAssetPath("life.png");
	m_brush_lives.outline_opacity = 0.0f;

	m_visited_goals.fill(false);
}

Level::Level(const std::string & name)
	: GameObject(name)
{
	init();
}

Level::~Level()
{
	for (auto p_go : m_dynamic_objects)
		delete p_go;
}