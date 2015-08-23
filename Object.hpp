//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_OBJECT_HPP
#define LD33_OBJECT_HPP

#include <stdint.h>
#include <Engine/SpriteNode.hpp>
class Player;
class Object: public engine::SpriteNode {
	class ContactHandler: public engine::util::EventHandler<b2Contact*, const b2Manifold*> {
		Object* m_me;
	public:
		ContactHandler(Object* a): m_me(a) {}
		virtual void handle(b2Contact*, const b2Manifold* manifold);
	};
protected:
	ContactHandler m_contactHandler;
	uint32_t m_points;
	float m_energy;
	float m_move;
public:
	Object(engine::Scene*);
	virtual ~Object();
	virtual uint8_t GetType() const;
	virtual bool initialize(Json::Value& root);
	float GetEnergy() {
		return m_energy;
	}

protected:
	virtual void OnUpdate(sf::Time interval);

	virtual void OnHit(Player* player);

};


#endif //LD33_OBJECT_HPP
