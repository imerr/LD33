//
// Created by iMer on 22.08.2015.
//

#include <Engine/util/Random.hpp>
#include <Engine/Factory.hpp>
#include <Engine/Text.hpp>
#include <sstream>
#include "Level.hpp"

Level::Level(engine::Game *game) : Scene(game), m_spawnTimer(2), m_score(0) {

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
	engine::Scene::OnUpdate(interval);
	m_spawnTimer -= interval.asSeconds();
	if (m_spawnTimer < 0) {
		m_spawnTimer = 0.5;
		engine::util::RandomFloat chance(0.0,1.0);
		engine::util::RandomFloat xpos(0, m_size.x);
		engine::util::RandomFloat rotation(0, 360);
		for (auto& obj: m_objects) {
			if (chance() < obj.chance) {
				engine::Node* o = engine::Factory::CreateChildFromFile(obj.script, this);
				o->SetPosition(xpos(), -200);
				o->SetRotation(rotation());
			}
		}
	}
}

void Level::AddScore(uint32_t score) {
	m_score += score;
	std::ostringstream ss;
	ss << "Score: " << m_score;
	static_cast<engine::Text*>(m_ui->GetChildByID("score"))->SetText(ss.str());
}
