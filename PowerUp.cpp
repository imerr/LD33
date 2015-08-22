//
// Created by iMer on 23.08.2015.
//

#include <iostream>
#include "PowerUp.hpp"
#include "Level.hpp"

const static char *PowerUpNames[PU_END] = {
	"rainbow",
	"speed"
};

PowerUp::PowerUp(engine::Scene *scene) : Object(scene) {

}

PowerUp::~PowerUp() {

}

void PowerUp::OnHit(Player *player) {
	static_cast<Level *>(m_scene)->PowerUp(m_type);
	Object::OnHit(player);
}

bool PowerUp::initialize(Json::Value &root) {
	if (!Object::initialize(root)) {
		return false;
	}
	std::string powerSt = root.get("powerup", "rainbow").asString();
	for (uint8_t i = 0; i < PU_END; i++) {
		if (powerSt == PowerUpNames[i]) {
			m_type = i;
		}
	}
	return true;
}
