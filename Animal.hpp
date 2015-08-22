//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_ANIMAL_HPP
#define LD33_ANIMAL_HPP

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
public:
	Animal(engine::Scene*);
	virtual ~Animal();
	virtual uint8_t GetType() const;

protected:
	virtual void OnUpdate(sf::Time interval);

};


#endif //LD33_ANIMAL_HPP
