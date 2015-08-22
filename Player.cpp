//
// Created by iMer on 22.08.2015.
//

#include <SFML/Window/Keyboard.hpp>
#include "Player.hpp"
#include "misc.hpp"
#include <Engine/Scene.hpp>
#include <iostream>
#include <Engine/Game.hpp>

Player::Player(engine::Scene *scene) : SpriteNode(scene), m_jumpCooldown(0) {
	m_keyHandler = m_scene->GetGame()->OnKeyDown.AddHandler([this](const sf::Event::KeyEvent &e) {
		float speed = 10;
		if (m_jumpCooldown < 0) {
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
				AABBAngle q(m_body->GetPosition(), angle[0], angle[1]);
				m_scene->GetWorld()->QueryAABB(&q, aabb);
				if (q.closest) {
					found = true;
					b2Vec2 delta = q.closest->GetBody()->GetPosition() - m_body->GetPosition();
					delta.Normalize();
					float dist = std::min<float>(q.dist, 200 / m_scene->GetPixelMeterRatio()) * speed;

					m_body->SetLinearVelocity(
						b2Vec2(dist * delta.x, dist * (delta.y + (m_scene->GetWorld()->GetGravity().x))));
					m_jumpCooldown = 0.5;
				}
			}
			if (query && found) {
			} else {
				const b2Vec2& gpos = m_body->GetPosition();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					m_body->ApplyLinearImpulse(b2Vec2(0, -speed), gpos, true);
					m_jumpCooldown = 0.7;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					m_body->ApplyLinearImpulse(b2Vec2(0, speed), gpos, true);
					m_jumpCooldown = 0.7;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					m_body->ApplyLinearImpulse(b2Vec2(-speed, 0), gpos, true);
					m_jumpCooldown = 0.7;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					m_body->ApplyLinearImpulse(b2Vec2(speed, 0), gpos, true);
					m_jumpCooldown = 0.7;
				}
			}
		}
	});
}

Player::~Player() {
	m_scene->GetGame()->OnKeyDown.RemoveHandler(m_keyHandler);
}

void Player::OnUpdate(sf::Time delta_) {
	float delta = delta_.asSeconds();
	auto gpos_ = GetGlobalPosition();
	float speed = 3.5 * delta;
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
	m_jumpCooldown -= delta;
}

uint8_t Player::GetType() const {
	return NT_PLAYER;
}

void Player::OnHitAnimal() {
	m_body->SetLinearVelocity(m_body->GetLinearVelocity() * 0.1);
	m_jumpCooldown = 0.01;
}

bool Player::AABBAngle::ReportFixture(b2Fixture *f) {
	b2Body *b = f->GetBody();
	float d = b2Distance(b->GetPosition(), refPos);
	engine::Node *n = static_cast<engine::Node *>(b->GetUserData());
	if (n->GetType() < engine::NT_END || n->GetType() == NT_PLAYER) { // Dont want the walls, or smoke particles
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
