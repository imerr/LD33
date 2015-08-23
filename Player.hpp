//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_PLAYER_HPP
#define LD33_PLAYER_HPP

#include <Engine/SpriteNode.hpp>
#include <limits>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/Sound.hpp>

class Player : public engine::SpriteNode {
public:
	struct AABBAngle : public b2QueryCallback {
		AABBAngle(b2Vec2 referencePos, float min, float max, const Node* ignore) : refPos(referencePos), closest(nullptr),
													  dist(std::numeric_limits<float>::infinity()), ignore(ignore) {
			angle[0] = min;
			angle[1] = max;
		}
		b2Vec2 refPos;
		float angle[3];
		engine::Node *closest;
		float dist;
		const engine::Node* ignore;

		bool ReportFixture(b2Fixture *f);
	};

	void OnHit();

protected:
	float m_lungeCooldown;
	engine::util::EventHandler<const sf::Event::KeyEvent&>* m_keyHandler;
	float m_maxEnergy;
	float m_energy;
	bool m_dead;
	const engine::Node* m_prevTarget;
	uint32_t m_kills;
	sf::Sound* m_lungeSound;
	sf::Sound *m_nomSound;
	float m_lifeTime;
public:
	Player(engine::Scene *);

	~Player();

	virtual uint8_t GetType() const;

	void OnHitAnimal(float energy);
	void ChangeEnergy(float energy);

protected:
	virtual void OnUpdate(sf::Time delta);

};


#endif //LD33_PLAYER_HPP
