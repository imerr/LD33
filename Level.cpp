//
// Created by iMer on 22.08.2015.
//

#include <Engine/util/Random.hpp>
#include <Engine/Factory.hpp>
#include "Level.hpp"

Level::Level(engine::Game *game) : Scene(game), m_spawnTimer(2) {

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
		engine::util::RandomFloat r(0.0,1.0);
		engine::util::RandomFloat l(0, m_size.x);
		for (auto& obj: m_objects) {
			if (r() < obj.chance) {
				engine::Node* o = engine::Factory::CreateChildFromFile(obj.script, this);
				o->SetPosition(l(), -200);
			}
		}
	}
}
