//
// Created by iMer on 23.08.2015.
//

#ifndef LD33_POWERUP_HPP
#define LD33_POWERUP_HPP

#include "Object.hpp"
enum PowerUpTypes {
	PU_RAINBOWS,
	PU_SPEED,
	PU_HEALTH,
	PU_DOUBLE,
	PU_ZIGZAG,
	PU_END
};
class PowerUp: public Object {
protected:
	uint8_t m_type;
public:
	PowerUp(engine::Scene*);
	~PowerUp();
	virtual void OnHit(Player* player);
	virtual bool initialize(Json::Value& root);
};


#endif //LD33_POWERUP_HPP
