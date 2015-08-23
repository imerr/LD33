//
// Created by iMer on 22.08.2015.
//

#include "Object.hpp"
#include "Player.hpp"
#include "misc.hpp"
#include "Level.hpp"
#include <Engine/Scene.hpp>
#include <Engine/ParticleSystem.hpp>
#include <iostream>
#include <Engine/ResourceManager.hpp>

sf::Sound* hitSound = nullptr; // yay amazing code
Object::Object(engine::Scene *scene) : SpriteNode(scene), m_contactHandler(this) {
	m_scene->OnContactPreSolve.AddHandler(&m_contactHandler);
	if (!hitSound) {
		hitSound = engine::ResourceManager::instance()->MakeSound("assets/sound/hitobject.wav");
	}
}

Object::~Object() {
	m_scene->OnContactPreSolve.RemoveHandler(&m_contactHandler);
}

void Object::OnUpdate(sf::Time interval) {
	m_body->SetLinearVelocity(
		b2Vec2(0, m_scene->GetWorld()->GetGravity().y * m_scene->GetPixelMeterRatio() * interval.asSeconds()));
	if (GetPosition().y > m_scene->GetSize().y + m_size.y*2) {
		Delete();
	}
}

uint8_t Object::GetType() const{
	return NT_OBJECT;
}

void Object::ContactHandler::handle(b2Contact *contact, const b2Manifold *manifold) {
	void *uA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *uB = contact->GetFixtureB()->GetBody()->GetUserData();
	Player *player = nullptr;
	if (uA == m_me && static_cast<engine::Node *>(uB)->GetType() == NT_PLAYER) {
		player = static_cast<Player *>(uB);
	} else if (uB == m_me && static_cast<engine::Node *>(uA)->GetType() == NT_PLAYER) {
		player = static_cast<Player *>(uB);
	} else { return; }
	if (m_me->IsRender()) {
		m_me->OnHit(player);
	}
	contact->SetEnabled(false);
}

bool Object::initialize(Json::Value &root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_points = static_cast<uint32_t>(root.get("points", 10).asInt());
	m_energy = static_cast<uint32_t>(root.get("energy", 100.0).asFloat());
	return true;
}

void Object::OnHit(Player* player) {
	if (GetType() == NT_OBJECT) {
		hitSound->play();
	}
	SetRender(false);
	player->OnHit();
	engine::Node* death = GetChildByID("death");
	if (death) {
		auto p = static_cast<engine::ParticleSystem*>(death);
		p->SetActive(true);
	} else {
		Delete();
	}
}