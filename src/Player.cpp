// Acetylene ~ Test Driver Player Implementation
// Record 01-0019
// 2024.03.20 @ 03.21

// File:	Player.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.20 @ 03.21
// Purpose:	Implement the functions declared in Sprites.h for the Player class.

#include "TestDriver.h"

// Implement Player class functions.

void Player::init(const glm::vec3& position, GameScene& parent) {
	m_parent = &parent;
	m_horizontalAnimation.init(position, glm::vec2(32.0f, 64.0f),
		"Assets/textures/player_horizontal.png", glm::ivec2(4, 1), 10.0f);
	m_verticalAnimation.init(position, glm::vec2(32.0f, 64.0f),
		"Assets/textures/player_vertical.png", glm::ivec2(4, 1), 10.0f);
	m_animation = &m_horizontalAnimation;
}

void Player::draw() {
	App::Graphics.submit(*m_animation);
	App::Graphics.submit(TestDriver::OPTIONS.OPTIONS.username,
		glm::vec3(m_animation->position.x - 32.0f,
			m_animation->position.y + m_animation->dimensions.y, 0.01f),
		glm::vec4(m_animation->position.x - 32.0f,
			m_animation->position.y + m_animation->dimensions.y, 96.0f, 32.0f),
		0.15f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		"Assets/fonts/Work_Sans/static/WorkSans-Regular.ttf", AC_ALIGN_CENTER,
		AC_ALIGN_CENTER);
}

void Player::processInput() {
	if (App::Input.isKeyDown(AC_KEY_A)) {
		if (m_animation != &m_horizontalAnimation) {
			m_horizontalAnimation.position = m_verticalAnimation.position;
			m_animation = &m_horizontalAnimation;
		}
		if (!m_animation->isPlaying()) {
			m_animation->play();
		}
		m_animation->reflect.horizontal = true;
		m_horizontalAnimation.velocity.x = -2.5f;
		m_verticalAnimation.velocity.x = -2.5f;
	}
	else if (App::Input.isKeyDown(AC_KEY_D)) {
		if (m_animation != &m_horizontalAnimation) {
			m_horizontalAnimation.position = m_verticalAnimation.position;
			m_animation = &m_horizontalAnimation;
		}
		if (!m_animation->isPlaying()) {
			m_animation->play();
		}
		m_animation->reflect.horizontal = false;
		m_horizontalAnimation.velocity.x = 2.5f;
		m_verticalAnimation.velocity.x = 2.5f;
	}
	else {
		m_horizontalAnimation.stop();
		m_animation->velocity.x = 0.0f;
	}
	if (App::Input.isKeyDown(AC_KEY_S)) {
		if (m_animation != &m_verticalAnimation) {
			m_verticalAnimation.position = m_horizontalAnimation.position;
			m_animation = &m_verticalAnimation;
		}
		if (!m_animation->isPlaying()) {
			m_animation->play();
		}
		m_horizontalAnimation.velocity.y = -2.5f;
		m_verticalAnimation.velocity.y = -2.5f;
	}
	else if (App::Input.isKeyDown(AC_KEY_W)) {
		if (m_animation != &m_verticalAnimation) {
			m_verticalAnimation.position = m_horizontalAnimation.position;
			m_animation = &m_verticalAnimation;
		}
		if (!m_animation->isPlaying()) {
			m_animation->play();
		}
		m_horizontalAnimation.velocity.y = 2.5f;
		m_verticalAnimation.velocity.y = 2.5f;
	}
	else {
		m_verticalAnimation.stop();
		m_animation->velocity.y = 0.0f;
	}
}

void Player::update(float dt) {
	glm::vec2 newPos(m_animation->position.x + m_animation->velocity.x,
		m_animation->position.y + m_animation->velocity.y);
	if (newPos.x < 0.0f || newPos.x + m_animation->dimensions.x > 256.0f
		|| newPos.y < 0.0f || newPos.y + m_animation->dimensions.y > 256.0f) {
		m_animation->velocity = glm::vec2(0.0f, 0.0f);
	}
	m_animation->update(dt);
	for (Coin& c : m_parent->m_coins) {
		if ((*m_animation).intersects(c)) {
			c.setCollected(true);
			App::Audio.playEffect("Assets/audio/ding.wav");
		}
	}
}

void Player::destroy() {
	m_horizontalAnimation.destroy();
	m_verticalAnimation.destroy();
	m_animation = 0;
}

const glm::vec3& Player::getPosition() const {
	return m_animation->position;
}