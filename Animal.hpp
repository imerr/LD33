//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_ANIMAL_HPP
#define LD33_ANIMAL_HPP

#include <stdint.h>
#include <Engine/SpriteNode.hpp>

class Animal: public engine::SpriteNode {
	class ContactHandler: public engine::util::EventHandler<b2Contact*, const b2Manifold*> {
		Animal* m_me;
	public:
		ContactHandler(Animal* a): m_me(a) {}
		virtual void handle(b2Contact*, const b2Manifold* manifold);
	};
protected:
	ContactHandler m_contactHandler;
	uint32_t m_points;
	float m_energy;
public:
	Animal(engine::Scene*);
	virtual ~Animal();
	virtual uint8_t GetType() const;
	virtual bool initialize(Json::Value& root);
	float GetEnergy() {
		return m_energy;
	}

protected:
	virtual void OnUpdate(sf::Time interval);

	void OnKill();
};


#endif //LD33_ANIMAL_HPP
