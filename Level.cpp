//
// Created by iMer on 22.08.2015.
//

#include <Engine/util/Random.hpp>
#include <Engine/Factory.hpp>
#include <Engine/Text.hpp>
#include <sstream>
#include "Level.hpp"
#include "PowerUp.hpp"
#include <Engine/Game.hpp>
#include <Engine/ResourceManager.hpp>
#include "Player.hpp"

Level::Level(engine::Game *game) : Scene(game), m_spawnTimer(2), m_score(0), m_over(false), m_objectNode(nullptr),
								   m_rainbowTime(0), m_speed(false), m_speedTime(0.0), m_healthTime(0.0),
								   m_doubleTime(0.0), m_paused(false), m_zigzagTime(0.0), m_mouse(nullptr) {
	m_backgroundMusic = engine::ResourceManager::instance()->MakeSound(
		"assets/sound/amazingmusicthebestthingyouwilleverhear.wav");
	m_backgroundMusic->setLoop(true);
	m_backgroundMusic->play();
	m_keyDown = m_game->OnKeyDown.MakeHandler([this](const sf::Event::KeyEvent &e) {
		if (e.code == sf::Keyboard::M) {
			if (m_backgroundMusic->getStatus() == sf::Sound::Playing) {
				m_backgroundMusic->stop();
			} else {
				m_backgroundMusic->play();
			}
		}
	});
	m_powerUpSound = engine::ResourceManager::instance()->MakeSound("assets/sound/powerup.wav");
	m_gameoverSound = engine::ResourceManager::instance()->MakeSound("assets/sound/gameover.wav");
	m_game->GetWindow()->setMouseCursorVisible(true);
}

Level::~Level() {
	delete m_backgroundMusic;
	delete m_powerUpSound;
	delete m_gameoverSound;
	m_game->OnKeyDown.RemoveHandler(m_keyDown);
}

bool Level::initialize(Json::Value &root) {
	if (!engine::Scene::initialize(root)) {
		return false;
	}
	auto objects = root["objects"];
	for (size_t i = 0; i < objects.size(); i++) {
		m_objects.push_back({
								objects[i][0u].asFloat(),
								objects[i][1u].asString()
							});
	}
	return true;

}

void Level::OnUpdate(sf::Time interval) {
	if (m_over && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		m_game->SwitchScene(engine::Factory::create<Level>("assets/scripts/level.json", m_game));
	}
	engine::util::RandomFloat xpos(0, m_size.x);
	engine::util::RandomFloat rotation(0, 360);
	engine::Scene::OnUpdate(interval);
	float delta = interval.asSeconds();
	m_spawnTimer -= delta;
	m_rainbowTime -= delta;
	m_doubleTime -= delta;
	m_speedTime -= delta;
	m_healthTime -= delta;
	m_scoreAdd -= delta;
	m_zigzagTime -= delta;
	if (m_speedTime > 0 && !m_speed) {
		m_speed = true;
		m_world->SetGravity(b2Vec2(0.0, 6.0));
	} else if (m_speed && m_speedTime < 0) {
		m_speed = false;
		m_world->SetGravity(b2Vec2(0.0, 3.0));
	}
	if (m_spawnTimer < 0) {
			engine::util::RandomFloat chance(0.0, 1.0);
		if (m_rainbowTime > 0) {
			m_spawnTimer = 0.1;
			engine::Node *o = engine::Factory::CreateChildFromFile("assets/scripts/sheep_rainbow.json", m_objectNode);
			o->SetPosition(xpos(), -200);
			o->SetRotation(rotation());
		} else if (m_zigzagTime > 0) {
			m_spawnTimer = 0.3;
			float c = chance();
			// MEH
			engine::Node *o = nullptr;
			if (c < 0.3) o= engine::Factory::CreateChildFromFile("assets/scripts/cow.json", m_objectNode);
			else if (c < 0.6) o= engine::Factory::CreateChildFromFile("assets/scripts/pig.json", m_objectNode);
			else if (c < 0.75) o= engine::Factory::CreateChildFromFile("assets/scripts/sheep_white.json", m_objectNode);
			else if (c < 0.9) o= engine::Factory::CreateChildFromFile("assets/scripts/sheep_brown.json", m_objectNode);
			else o= engine::Factory::CreateChildFromFile("assets/scripts/sheep_black.json", m_objectNode);
			o->SetPosition(m_zig?362:662, -200);
			m_zig = !m_zig;
			o->SetRotation(rotation());
		} else {
			m_spawnTimer = 0.5;
			if (m_speed) m_spawnTimer /= 4;
			if (m_doubleTime > 0) m_spawnTimer /= 2;

			for (auto &obj: m_objects) {
				float c = obj.chance;
				if (m_doubleTime > 0) c *= 2;
				if (chance() < c) {
					engine::Node *o = engine::Factory::CreateChildFromFile(obj.script, m_objectNode);
					o->SetPosition(xpos(), -200);
					o->SetRotation(rotation());
				}
			}
		}
	}
	if (m_mouse) {
		if (!m_mouse->IsRender()) {
			m_mouse->GetBody()->SetGravityScale(1);
			m_game->GetWindow()->setMouseCursorVisible(false);
			m_mouse = nullptr;
		} else {
			auto pos = m_game->GetMousePosition();
			if (pos.y > 576) pos.y = -10;
			m_mouse->SetPosition(pos.x, pos.y);
		}
	}
	if (m_healthTime > 0) {
		Player *player = static_cast<Player *>(GetChildByID("player"));
		if (player) {
			player->ChangeEnergy(1.5f);
		}
	}
	if (m_scoreAdd < 0 && !m_over) {
		AddScore(1);
		m_scoreAdd = 0.5;
	}
}

void Level::AddScore(uint32_t score) {
	if (m_doubleTime > 0) score *= 2;
	m_score += score;
	std::ostringstream ss;
	ss << "Score: " << m_score;
	static_cast<engine::Text *>(m_ui->GetChildByID("score"))->SetText(ss.str());
}

void Level::GameOver() {
	m_gameoverSound->play();
	m_ui->GetChildByID("over")->SetActive(true);
	m_over = true;
}

void Level::PowerUp(uint8_t type) {
	m_powerUpSound->play();
	switch (type) {
		case PU_RAINBOWS:
			m_rainbowTime = 10.0f;
			m_spawnTimer = 0.0f;
			break;
		case PU_SPEED:
			m_speedTime = 10.0f;
			break;
		case PU_HEALTH:
			m_healthTime = 5.0f;
			break;
		case PU_DOUBLE:
			m_doubleTime = 10.0f;
			break;
		case PU_ZIGZAG:
			m_zigzagTime = 10.0f;
			break;
		default:
			break;
	}
}

void Level::update(sf::Time interval) {
	if (!m_game->IsFocus() && !m_paused) {
		m_paused = true;
		GetUi()->GetChildByID("paused")->SetActive(true);
	}
	if (m_paused) {
		m_ui->update(interval);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			m_paused = false;
			GetUi()->GetChildByID("paused")->SetActive(false);
		}
	} else {
		engine::Scene::update(interval);
	}
}

void Level::OnInitializeDone() {
	m_objectNode = GetChildByID("objects");
	m_mouse = GetChildByID("mouse");
}
