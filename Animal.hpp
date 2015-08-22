//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_ANIMAL_HPP
#define LD33_ANIMAL_HPP

#include "Object.hpp"

class Animal: public Object {
public:
	Animal(engine::Scene*);
	virtual ~Animal();
	virtual uint8_t GetType() const;

protected:
	virtual void OnHit(Player*);
};


#endif //LD33_ANIMAL_HPP
