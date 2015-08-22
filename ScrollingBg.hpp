//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_SCROLLINGBG_HPP
#define LD33_SCROLLINGBG_HPP


#include <Engine/SpriteNode.hpp>

class ScrollingBg: public engine::SpriteNode {
public:
	ScrollingBg(engine::Scene*);
	~ScrollingBg();
	virtual bool initialize(Json::Value& root);

protected:
	void OnUpdate(sf::Time interval);
};


#endif //LD33_SCROLLINGBG_HPP
