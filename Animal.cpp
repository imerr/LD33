//
// Created by iMer on 22.08.2015.
//

#include "Animal.hpp"
#include "Player.hpp"
#include "misc.hpp"
#include "Level.hpp"
#include <Engine/Scene.hpp>
#include <Engine/ParticleSystem.hpp>
#include <iostream>

Animal::Animal(engine::Scene *scene) : Object(scene) {
}

Animal::~Animal() {
}

void Animal::OnHit(Player* player) {
	static_cast<Level*>(m_scene)->AddScore(m_points);
	player->OnHitAnimal(m_energy);
	Object::OnHit(player);
}

uint8_t Animal::GetType() const {
	return NT_ANIMAL;
}
