//
// Created by iMer on 22.08.2015.
//

#include "Bar.hpp"

Bar::Bar(engine::Scene *scene) : SpriteNode(scene) {

}

Bar::~Bar() {

}

void Bar::SetPct(float pct) {
	pct = std::max(0.0f, std::min(pct, 1.0f));
	engine::SpriteNode *node = static_cast<engine::SpriteNode *>(GetChildByID("full"));
	sf::IntRect &rect = node->GetTextureRect();
	// scaling..
	int scale = m_size.x/m_textureRect.width;
	if (scale == 0) scale = 1; // prevent divison by zero
	rect.width = static_cast<int>(m_textureRect.width * pct);
	node->SetFlipped(false); // update texture coordinates.. meh
	node->SetSize(sf::Vector2f((static_cast<int>((m_size.x * pct)/scale)*scale), m_size.y));
}

