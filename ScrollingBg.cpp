//
// Created by iMer on 22.08.2015.
//

#include "ScrollingBg.hpp"
#include <Engine/Scene.hpp>
#include <iostream>

ScrollingBg::ScrollingBg(engine::Scene *scene) : SpriteNode(scene) {

}

ScrollingBg::~ScrollingBg() {

}

bool ScrollingBg::initialize(Json::Value &root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}

	return true;
}

void ScrollingBg::OnUpdate(sf::Time interval) {
	const auto &pos = getPosition();
	float gravity = m_scene->GetWorld()->GetGravity().y * m_scene->GetPixelMeterRatio();
	if (pos.y > 0) {
		setPosition(pos.x, pos.y - m_size.y/2 + gravity * interval.asSeconds());
	} else {
		setPosition(pos.x, pos.y + gravity * interval.asSeconds());
	}
}
