//
// Created by iMer on 22.08.2015.
//

#ifndef LD33_LEVEL_HPP
#define LD33_LEVEL_HPP


#include <Engine/Scene.hpp>

class Level: public engine::Scene {
public:
	struct Object {
		float chance;
		std::string script;
	};
protected:
	std::vector<Object> m_objects;
public:
	Level(engine::Game * game);
	~Level();
	virtual bool initialize(Json::Value& root);

protected:
	virtual void OnUpdate(sf::Time interval);

	float m_spawnTimer;
};


#endif //LD33_LEVEL_HPP
