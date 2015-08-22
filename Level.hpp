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
	float m_spawnTimer;
	uint32_t m_score;
public:
	Level(engine::Game * game);
	~Level();
	virtual bool initialize(Json::Value& root);
	void AddScore(uint32_t score);

protected:
	virtual void OnUpdate(sf::Time interval);

};


#endif //LD33_LEVEL_HPP
