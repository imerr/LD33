#include <Engine/Factory.hpp>
#include "LD33.hpp"
#include "ScrollingBg.hpp"
#include "Player.hpp"
#include "Animal.hpp"
#include "Bar.hpp"

int main() {
	engine::Factory::RegisterType("scrollingbg", engine::Factory::CreateChildNode<ScrollingBg>);
	engine::Factory::RegisterType("player", engine::Factory::CreateChildNode<Player>);
	engine::Factory::RegisterType("animal", engine::Factory::CreateChildNode<Animal>);
	engine::Factory::RegisterType("bar", engine::Factory::CreateChildNode<Bar>);
	LD33 game;
	game.run();
	return 0;
}