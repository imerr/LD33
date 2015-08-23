//
// Created by iMer on 22.08.2015.
//

#include <Engine/util/Random.hpp>
#include <Engine/Factory.hpp>
#include <Engine/Text.hpp>
#include <sstream>
#include "Level.hpp"
#include "PowerUp.hpp"
#include <Engine/Game.hpp>

Level::Level(engine::Game *game) : Scene(game), m_spawnTimer(2), m_score(0), m_over(false), m_objectNode(nullptr),
								   m_rainbowTime(0), m_speed(false), m_speedTime(0.0) {

}

Level::~Level() {

}

bool Level::initialize(Json::Value &root) {
	if (!engine::Scene::initialize(root)) {
		return false;
	}
	auto objects = root["objects"];
	for (size_t i = 0; i < objects.size(); i++) {
		m_objects.push_back({
								objects[i][0u].asFloat(),
								objects[i][1u].asString()
							});
	}
	return true;

}

void Level::OnUpdate(sf::Time interval) {
	if (m_over && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		m_game->SwitchScene(engine::Factory::create<Level>("assets/scripts/level.json", m_game));
	}
	engine::util::RandomFloat xpos(0, m_size.x);
	engine::util::RandomFloat rotation(0, 360);
	if (!m_objectNode) {
		m_objectNode = GetChildByID("objects");
	}
	engine::Scene::OnUpdate(interval);
	m_spawnTimer -= interval.asSeconds();
	m_rainbowTime -= interval.asSeconds();
	m_speedTime -= interval.asSeconds();
	if (m_speedTime > 0 && !m_speed) {
		m_speed = true;
		m_world->SetGravity(b2Vec2(0.0, 6.0));
	} else if (m_speed && m_speedTime < 0) {
		m_speed = false;
		m_world->SetGravity(b2Vec2(0.0, 3.0));
	}
	if (m_spawnTimer < 0) {
		if (m_rainbowTime > 0) {
			m_spawnTimer = 0.1;
			engine::Node *o = engine::Factory::CreateChildFromFile("assets/scripts/sheep_rainbow.json", m_objectNode);
			o->SetPosition(xpos(), -200);
			o->SetRotation(rotation());
		} else {
			m_spawnTimer = 0.5;
			if (m_speed) m_spawnTimer /= 2;
			engine::util::RandomFloat chance(0.0, 1.0);

			for (auto &obj: m_objects) {
				if (chance() < obj.chance) {
					engine::Node *o = engine::Factory::CreateChildFromFile(obj.script, m_objectNode);
					o->SetPosition(xpos(), -200);
					o->SetRotation(rotation());
				}
			}
		}
	}
}

void Level::AddScore(uint32_t score) {
	m_score += score;
	std::ostringstream ss;
	ss << "Score: " << m_score;
	static_cast<engine::Text *>(m_ui->GetChildByID("score"))->SetText(ss.str());
}

void Level::GameOver() {
	m_ui->GetChildByID("over")->SetActive(true);
	m_over = true;
}

void Level::PowerUp(uint8_t type) {
	switch (type) {
		case PU_RAINBOWS:
			m_rainbowTime = 10.0f;
			m_spawnTimer = 0.0f;
			break;
		case PU_SPEED:
			m_speedTime = 10.0f;
			break;
		default:
			break;
	}
}
