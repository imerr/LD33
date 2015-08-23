//
// Created by iMer on 22.08.2015.
//

#include <SFML/Window/Keyboard.hpp>
#include "Player.hpp"
#include "misc.hpp"
#include "Bar.hpp"
#include "Level.hpp"
#include <Engine/Scene.hpp>
#include <iostream>
#include <Engine/Game.hpp>
#include <Engine/ParticleSystem.hpp>
#include <Engine/ResourceManager.hpp>

Player::Player(engine::Scene *scene) : SpriteNode(scene), m_lungeCooldown(0), m_maxEnergy(1000), m_energy(1000),
									   m_dead(false), m_prevTarget(nullptr), m_kills(0), m_lifeTime(0.0f) {
	m_lungeSound = engine::ResourceManager::instance()->MakeSound("assets/sound/lunge2.ogg");
	m_nomSound = engine::ResourceManager::instance()->MakeSound("assets/sound/crunch.ogg");
	m_keyHandler = m_scene->GetGame()->OnKeyDown.MakeHandler([this](const sf::Event::KeyEvent &e) {
		if (!m_render || static_cast<Level*>(m_scene)->IsPaused()) {
			return;
		}
		float speed = 10;
		if (m_lungeCooldown < 0) {
			b2AABB aabb;
			bool query = false;
			float angle[2];
			auto p = m_body->GetPosition();
			sf::Vector2f ws = m_scene->GetSize();
			ws.x /= m_scene->GetPixelMeterRatio();
			ws.y /= m_scene->GetPixelMeterRatio();
			const float leeway = 50;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				query = true;
				aabb.lowerBound.Set(0, 0);
				aabb.upperBound.Set(ws.x, p.y);
				angle[0] = 270 - leeway;
				angle[1] = 270 + leeway;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				query = true;
				aabb.lowerBound.Set(0, p.y);
				aabb.upperBound.Set(ws.x, ws.y);
				angle[0] = 90 - leeway;
				angle[1] = 90 + leeway;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				query = true;
				aabb.lowerBound.Set(0, 0);
				aabb.upperBound.Set(p.x, ws.y);
				angle[0] = 180 - leeway;
				angle[1] = 180 + leeway;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				query = true;
				aabb.lowerBound.Set(p.x, 0);
				aabb.upperBound.Set(ws.x, ws.y);
				angle[0] = -leeway;
				angle[1] = leeway;
			}
			bool found = false;
			if (query) {
				AABBAngle q(m_body->GetPosition(), angle[0], angle[1], m_prevTarget);
				m_scene->GetWorld()->QueryAABB(&q, aabb);
				if (q.closest) {
					m_prevTarget = q.closest;
					found = true;
					b2Vec2 delta = q.closest->GetBody()->GetPosition() - m_body->GetPosition();
					delta.Normalize();
					float dist = std::min<float>(q.dist, 200 / m_scene->GetPixelMeterRatio());
					float gravityAdd = 3.0 * (dist / q.dist) / speed;
					dist *= speed;
					m_body->SetLinearVelocity(
						b2Vec2(dist * delta.x,
							   dist * (delta.y + gravityAdd)));
					m_lungeCooldown = 0.3f;
					m_lungeSound->play();
					ChangeEnergy(-50.0f);
				}
			}
			if (query && found) {
			} else {
				const b2Vec2 &gpos = m_body->GetPosition();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					m_body->ApplyLinearImpulse(b2Vec2(0, -speed), gpos, true);
					m_lungeCooldown = 0.7f;
					m_lungeSound->play();
					ChangeEnergy(-50.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					m_body->ApplyLinearImpulse(b2Vec2(0, speed), gpos, true);
					m_lungeCooldown = 0.7f;
					m_lungeSound->play();
					ChangeEnergy(-50.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					m_body->ApplyLinearImpulse(b2Vec2(-speed, 0), gpos, true);
					m_lungeCooldown = 0.7f;
					m_lungeSound->play();
					ChangeEnergy(-50.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					m_body->ApplyLinearImpulse(b2Vec2(speed, 0), gpos, true);
					m_lungeCooldown = 0.7f;
					m_lungeSound->play();
					ChangeEnergy(-50.0f);
				}
			}
		}
	});
}

Player::~Player() {
	m_scene->GetGame()->OnKeyDown.RemoveHandler(m_keyHandler);
	delete m_lungeSound;
	delete m_nomSound;
}

void Player::OnUpdate(sf::Time delta_) {
	if (!m_render) {
		return;
	}
	if (m_dead) {
		m_body->SetActive(false);
		SetRender(false);
		return;
	}
	float delta = delta_.asSeconds();
	m_lifeTime += delta;
	auto gpos_ = GetGlobalPosition();
	float speed = 6 * delta;
	float ratio = m_scene->GetPixelMeterRatio();
	b2Vec2 gpos(gpos_.x * ratio, gpos_.y * ratio);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_body->ApplyLinearImpulse(b2Vec2(0, -speed), gpos, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_body->ApplyLinearImpulse(b2Vec2(0, speed), gpos, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_body->ApplyLinearImpulse(b2Vec2(-speed, 0), gpos, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_body->ApplyLinearImpulse(b2Vec2(speed, 0), gpos, true);
	}
	m_lungeCooldown -= delta;
	if (m_lifeTime < 120)
		ChangeEnergy(-.5f);
	else if (m_lifeTime < 360)
		ChangeEnergy(-.6f);
	else if (m_lifeTime < 600)
		ChangeEnergy(-.8f);
}

uint8_t Player::GetType() const {
	return NT_PLAYER;
}

void Player::OnHitAnimal(float energy) {
	m_nomSound->play();
	ChangeEnergy(energy);
	m_lungeCooldown = 0;
	m_kills++;
	if (m_kills == 2) {
		GetChildByID("guide")->Delete();
	}
}

bool Player::AABBAngle::ReportFixture(b2Fixture *f) {
	b2Body *b = f->GetBody();
	float d = b2Distance(b->GetPosition(), refPos);
	engine::Node *n = static_cast<engine::Node *>(b->GetUserData());
	// Dont want the walls, or smoke particles, or already killed ones
	if (n->GetType() < engine::NT_END || n->GetType() == NT_PLAYER || !n->IsRender() || n == ignore) {
		return true;
	}
	if (d < dist) {
		float a = engine::util::b2AngleDeg(refPos, b->GetPosition());
		if (a > angle[0] && a < angle[1] || (angle[0] < 0 && a > angle[0] + 360)) {
			closest = n;
			dist = d;
			angle[2] = a;
		}
	}
	return true;
}

void Player::ChangeEnergy(float energy) {
	m_energy += energy;
	m_energy = std::min(m_energy, m_maxEnergy);
	Bar *bar = static_cast<Bar *>(m_scene->GetUi()->GetChildByID("energy"));
	bar->SetPct(m_energy / m_maxEnergy);
	if (m_energy < 0) {
		static_cast<Level *>(m_scene)->GameOver();
		m_dead = true;
		engine::ParticleSystem* trail = static_cast<engine::ParticleSystem*>(GetChildByID("smoketrail"));
		trail->SetActive(false);
		engine::ParticleSystem* death = static_cast<engine::ParticleSystem*>(GetChildByID("death"));
		death->SetActive(true);
	}
}

void Player::OnHit() {
	m_body->SetLinearVelocity(m_body->GetLinearVelocity() * 0.1f);
}
