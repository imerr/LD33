//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_PLAYER_HPP
#define LD33_PLAYER_HPP

#include <Engine/SpriteNode.hpp>
#include <limits>
#include <SFML/Window/Event.hpp>

class Player : public engine::SpriteNode {
public:
	struct AABBAngle : public b2QueryCallback {
		AABBAngle(b2Vec2 referencePos, float min, float max) : refPos(referencePos), closest(nullptr),
													  dist(std::numeric_limits<float>::infinity()) {
			angle[0] = min;
			angle[1] = max;
		}
		b2Vec2 refPos;
		float angle[3];
		engine::Node *closest;
		float dist;

		bool ReportFixture(b2Fixture *f);
	};

protected:
	float m_jumpCooldown;
	engine::util::EventHandler<const sf::Event::KeyEvent&>* m_keyHandler;
public:
	Player(engine::Scene *);

	~Player();

	virtual uint8_t GetType() const;

	void OnHitAnimal();

protected:
	virtual void OnUpdate(sf::Time delta);
};


#endif //LD33_PLAYER_HPP
