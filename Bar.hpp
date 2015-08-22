//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_BAR_HPP
#define LD33_BAR_HPP
#include <Engine/SpriteNode.hpp>

class Bar: public engine::SpriteNode {
public:
	Bar(engine::Scene* scene);
	~Bar();
	void SetPct(float pct);
};


#endif //LD33_BAR_HPP
