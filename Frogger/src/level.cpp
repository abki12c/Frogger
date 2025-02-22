#include <sgg/graphics.h>
#include "level.h"
#include "gamestate.h"
#include "util.h"
#include "config.h"
#include "vehicle.h"
#include <fstream>
#include <iostream>
#include "log.h"

void Level::spawnMovingObjects()
{
	int active_count;
	// Update game objects in each lane
	for (auto& lane : lanes) {
		// Decrement spawn timer
		lane.spawn_timer -= graphics::getDeltaTime() / 1000.0f;

		// Check if it's time to spawn a new game object and under a certain limit
		if (lane.spawn_timer <= 0.0f ) {
			// Determine spawn position based on speed
			float x_position = (lane.speed > 0) ? 0.f : CANVAS_WIDTH;

			GameObject* game_object;
			if(lane.obj_sprite.find("L",0) == 0) {
				Log* log = new Log("log", x_position, lane.y_position, lane.speed, lane.obj_width, lane.obj_height, lane.obj_sprite);
				game_object = log;
			}else{
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
		std::remove_if(m_dynamic_objects.begin(), m_dynamic_objects.end(), [](GameObject* game_object) {
			if (game_object && !game_object->isActive()) {
				delete game_object;
				game_object = nullptr;
				return true;
			}
			return false;
			}),
		m_dynamic_objects.end());
}

void Level::readFile(const std::string& filename)
{
	std::ifstream inFile(std::string(ASSET_PATH) + filename);

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

			switch (line[x]) {
				case '~':
					m_blocks.push_back(Box(block_x, block_y, m_block_size, m_block_size));
					m_block_names.push_back("water.png");
					break;
				case '-':
					m_blocks.push_back(Box(block_x, block_y, m_block_size, m_block_size));
					m_block_names.push_back("grass.png");
					break;
				case '*':
					m_blocks.push_back(Box(block_x, block_y, m_block_size, m_block_size));
					m_block_names.push_back("home.png");
					break;
				case 'R':
					m_blocks.push_back(Box(block_x, block_y, m_block_size, m_block_size));
					m_block_names.push_back("home_goal.png");
					break;
				case 'x':
					m_blocks.push_back(Box(block_x, block_y, m_block_size, m_block_size));
					m_block_names.push_back("road.png");
					break;
			}
		}
		y++;
	}

	inFile.close();
}


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	std::string& name = m_block_names[i];

	float x = box.m_pos_x + 35;
	float y = box.m_pos_y + 25;

	m_block_brush.texture = std::string(ASSET_PATH) + m_block_names[i];

	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	if (m_state->getDebugMode())
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

	// Draw additional sprite for visited home_goal.png
    if (m_block_names[i] == "home_goal.png" && (i==2 || i==5 || i==8 || i==11) && m_visited_goals[(i-2)/3]) {
		graphics::drawRect(x, y, m_block_size-10, m_block_size-10, m_brush_frog_safe);
	}

}

void Level::checkCollisions()
{
	m_state->getPlayer()->setIsOnLog(false);

	// Check collisions with dynamic objects
	for (auto& gameObject : m_dynamic_objects) {
		if (gameObject && gameObject->isActive()) {
			if (dynamic_cast<Vehicle*>(gameObject)) {
				Vehicle* vehicle = dynamic_cast<Vehicle*>(gameObject);

				// Create a Box for the game object
				Box vehicleBox(vehicle->getPosX(), vehicle->getPosY(), vehicle->getWidth(), vehicle->getHeight());

				// Check for collision
				if (m_state->getPlayer()->intersect(vehicleBox)) {
					graphics::playSound(std::string(ASSET_PATH) + "sound/splat.wav", 0.5f, false);
					m_state->getPlayer()->resetPlayerPosition();
					m_state->getPlayer()->reduceLives();
					break;
				}
			} else {
				Log* log = dynamic_cast<Log*>(gameObject);

				// Create a Box for the game object
				Box logBox(log->getPosX(), log->getPosY(), log->getWidth(), log->getHeight());

				// Check for collision
				if (m_state->getPlayer()->intersect(logBox)) {
					m_state->getPlayer()->setIsOnLog(true);
					m_state->getPlayer()->setOnLogSpeed(log->getSpeed());
					break;
				}
			}	
		}
	}

	// check collision with blocks
	for (int i = 0; i < m_blocks.size(); i++) {
		if (m_block_names[i] == "home_goal.png" && m_state->getPlayer()->intersect(m_blocks[i])) {
			if (!m_visited_goals[(i - 2) / 3]) {
				m_visited_goals[(i - 2) / 3] = true;
				if (m_visited_goals[0] && m_visited_goals[1] && m_visited_goals[2] && m_visited_goals[3]) {
					// collected all frogs
					m_state->getInstance()->updateScore(50);
					m_visited_goals[0] = false;
					m_visited_goals[1] = false;
					m_visited_goals[2] = false;
					m_visited_goals[3] = false;

					if (!m_levelCompleteSoundPlayed) {
						graphics::stopMusic();
						graphics::playSound(std::string(ASSET_PATH) + "sound/songLevelComplete.mp3", 0.5f, false);
						m_levelCompleteSoundPlayed = true;
						m_levelCompleteSoundTimer = 0.0f;
					}
					
				}
				m_state->getInstance()->updateScore(20);

				
				m_state->getPlayer()->resetPlayerPosition();
				m_remaining_time = m_total_time;
			}
			else {
				graphics::playSound(std::string(ASSET_PATH) + "sound/squash.wav", 0.5f, false);
				m_state->getPlayer()->resetPlayerPosition();
				m_state->getPlayer()->reduceLives();
			}
		}
		else if (m_block_names[i] == "home.png" && m_state->getPlayer()->intersect(m_blocks[i])) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/squash.wav", 0.5f, false);
			m_state->getPlayer()->resetPlayerPosition();
			m_state->getPlayer()->reduceLives();
		} 
		else if (m_block_names[i] == "water.png" && !m_state->getPlayer()->getIsOnLog() && m_state->getPlayer()->intersect(m_blocks[i])) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/splash.wav", 1.0f, false);
			m_state->getPlayer()->resetPlayerPosition();
			m_state->getPlayer()->reduceLives();
		}
	}
}

void Level::resetLevel()
{
	for (int i = 0; i < 4; i++) {
		m_visited_goals[i] = false;
	}

	m_remaining_time = 60.0f;
	m_levelCompleteSoundTimer = 0.0f;

	m_playedTimeRunsOutSound = false;
	m_levelCompleteSoundPlayed = false;
}

void Level::update(float dt)
{
	checkCollisions();

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

	// Decrease remaining time
	m_remaining_time -= dt / 1000.0f;
	if (m_remaining_time < 0) {
		m_remaining_time = m_total_time; // reset timer
		m_playedTimeRunsOutSound = false;
		m_state->getPlayer()->reduceLives();
	}

	if (m_remaining_time < m_total_time * 0.3f && !m_playedTimeRunsOutSound) {
		graphics::playSound(std::string(ASSET_PATH) + "sound/runningOutOfTime.wav", 0.5f, false);
		m_playedTimeRunsOutSound = true;
	}

	// game over
	if (m_state->getPlayer()->getLives() == 0) {
		graphics::stopMusic();
		m_state->getInstance()->setStatus(GameState::STATUS_GAME_OVER);
		graphics::playSound(std::string(ASSET_PATH) + "sound/game-over.wav",0.5f,false);
	}


	// Handle level completion music delay
	if (m_levelCompleteSoundPlayed) {
		m_levelCompleteSoundTimer += dt / 1000.0f;
		if (m_levelCompleteSoundTimer >= 6.0f) {
			graphics::playMusic(std::string(ASSET_PATH) + "sound/main-song.mp3", 0.5f, true);
			m_levelCompleteSoundPlayed = false;
		}
	}
	
	GameObject::update(dt);
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
	int lives = m_state->getPlayer()->getLives();
	for (int i = 0; i < lives;i++) {
		graphics::drawRect(70 + 40*i, CANVAS_HEIGHT - 35, 30.f, 30.f, m_brush_lives);
	}

	// Draw time bar
	float time_bar_width = (m_remaining_time / m_total_time) * 100.0f; // Full width = 100 units
	float time_bar_x = CANVAS_WIDTH - 70 - (100 - time_bar_width) / 2; // Centering
	graphics::drawText(CANVAS_WIDTH - 100, CANVAS_HEIGHT - 60, 30.f, "Time", m_brush_text);
	graphics::drawRect(time_bar_x, CANVAS_HEIGHT -40, time_bar_width,20.f, m_brush_time);

	if (m_remaining_time < m_total_time * 0.3f) {
		SETCOLOR(m_brush_time.fill_color, 255.0f, 0.0f, 0.0f);
	}
	else if (m_remaining_time < m_total_time * 0.6f) {
		SETCOLOR(m_brush_time.fill_color, 255.0f, 165.0f, 0.0f);
	}
	else {
		SETCOLOR(m_brush_time.fill_color, 0.0f, 128.0f, 0.0f);
	}

	// Draw Score
	graphics::drawText(CANVAS_WIDTH/2, CANVAS_HEIGHT - 60, 30.f, "Score", m_brush_text);
	graphics::drawText(CANVAS_WIDTH/2, CANVAS_HEIGHT - 25, 30.f, std::to_string(m_state->getInstance()->getScore()), m_brush_score);
}

void Level::init()
{
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();


	readFile("level.txt");

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.outline_color, 0.f, 0.f, 255.f);

	m_brush_time.outline_opacity = 0.0f;
	SETCOLOR(m_brush_time.fill_color, 0.f, 128.f, 0.f);

	m_brush_text.outline_opacity = 0.0f;
	SETCOLOR(m_brush_text.fill_color, 255.f, 255.f, 255.f);

	m_brush_frog_safe.texture = std::string(ASSET_PATH) + "safe.png";
	m_brush_frog_safe.outline_opacity = 0.0f;

	m_brush_lives.texture = std::string(ASSET_PATH) + "life.png";
	m_brush_lives.outline_opacity = 0.0f;

	m_visited_goals[0] = false;
	m_visited_goals[1] = false;
	m_visited_goals[2] = false;
	m_visited_goals[3] = false;

	lanes = { {
		{ 735, 0.07f, 3.0f, 3.0f, 73.5f, 40.5f, "car1.png" },
		{660, -0.1f, 4.0f, 4.0f, 78.0f, 42.0f, "car2.png"},
		{585, 0.1f, 2.5f,2.5f, 75.0f, 45.0f, "car3.png"},
		{500, -0.1f, 3.5f,3.5f, 72.0f, 43.5f, "car4.png"},
		{418, 0.1f, 5.0f,5.0f, 109.5f, 52.5f, "truck.png"},
		{265, -0.1f, 2.5f,2.5f, 80.0f, 38.4f, "Log-small.png"},
		{190, 0.1f, 3.5f,3.5f, 106.6f, 38.4f, "Log-medium.png"},
		{115, -0.1f, 4.0f,4.0f, 160.0f, 38.4f, "Log-long.png"}
	} };

	// Prepopulate the dynamic game objects list with vehicles and logs
	for (const auto& lane : lanes) {
		// Set initial x_position for each lane
		float x_position = (lane.speed > 0) ? -lane.obj_width + 300 : CANVAS_WIDTH - 300 + lane.obj_width;

		GameObject* game_object;
		if (lane.obj_sprite.find("L", 0) == 0) {
			Log* log = new Log("log", x_position, lane.y_position, lane.speed, lane.obj_width, lane.obj_height, lane.obj_sprite);
			game_object = log;
		}
		else {
			Vehicle* new_vehicle = new Vehicle("vehicle", x_position, lane.y_position, lane.speed, lane.obj_width, lane.obj_height, lane.obj_sprite);
			game_object = new_vehicle;
		}

		m_dynamic_objects.push_back(game_object);
	}
}

Level::Level(const std::string & name)
	: GameObject(name)
{

}

Level::~Level()
{
	for (auto p_go : m_dynamic_objects)
		delete p_go;


}