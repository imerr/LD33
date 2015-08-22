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

Animal::Animal(engine::Scene *scene) : SpriteNode(scene), m_contactHandler(this) {
	m_scene->OnContactPreSolve.AddHandler(&m_contactHandler);
}

Animal::~Animal() {
	m_scene->OnContactPreSolve.RemoveHandler(&m_contactHandler);
}

void Animal::OnUpdate(sf::Time interval) {
	m_body->SetLinearVelocity(
		b2Vec2(0, m_scene->GetWorld()->GetGravity().y * m_scene->GetPixelMeterRatio() * interval.asSeconds()));
	if (GetPosition().y > m_scene->GetSize().y + m_size.y*2) {
		Delete();
	}
}

uint8_t Animal::GetType() const{
	return NT_ANIMAL;
}

void Animal::ContactHandler::handle(b2Contact *contact, const b2Manifold *manifold) {
	void *uA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *uB = contact->GetFixtureB()->GetBody()->GetUserData();
	Player *player = nullptr;
	if (uA == m_me && static_cast<engine::Node *>(uB)->GetType() == NT_PLAYER) {
		player = static_cast<Player *>(uB);
	} else if (uB == m_me && static_cast<engine::Node *>(uA)->GetType() == NT_PLAYER) {
		player = static_cast<Player *>(uB);
	} else { return; }
	if (m_me->IsRender()) {
		player->OnHitAnimal(m_me->GetEnergy());
		m_me->OnKill();
	}
	contact->SetEnabled(false);
}

bool Animal::initialize(Json::Value &root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_points = static_cast<uint32_t>(root.get("points", 10).asInt());
	m_energy = static_cast<uint32_t>(root.get("energy", 100.0).asFloat());
	return true;
}

void Animal::OnKill() {
	static_cast<Level*>(m_scene)->AddScore(m_points);
	SetRender(false);
	engine::Node* death = GetChildByID("death");
	if (death) {
		auto p = static_cast<engine::ParticleSystem*>(death);
		p->SetActive(true);
	} else {
		Delete();
	}
}
