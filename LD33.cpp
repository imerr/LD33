//
// Created by iMer on 22.08.2015.
//

#include <Engine/Factory.hpp>
#include "LD33.hpp"
#include "Level.hpp"

LD33::LD33() {
	//m_window.setVerticalSyncEnabled(true);
	m_windowTitle = "LD33 - A Peaceful Farm Stroll";
	m_scene = engine::Factory::create<Level>("assets/scripts/level.json", this);
}

LD33::~LD33() {

}
