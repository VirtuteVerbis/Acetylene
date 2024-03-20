// Acetylene ~ Test Driver Ember Implementation
// Record 01-0017
// 2024.03.18 @ 15.30

// File:	Ember.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.18 @ 15.31
// Purpose:	Implement the functions declared in Sprites.h for the Ember class.

#include "Sprites.h"

// Implement Ember class functions.

void Ember::init(const glm::vec3& position) {
	Animation::init(position, glm::vec2(8, 8), "Assets/textures/ember.png",
		glm::ivec2(8, 1), 40.0f);
	velocity.x = (((float)rand() / (float)RAND_MAX) * 0.5f) + 0.5f;
	velocity.y = (((float)rand() / (float)RAND_MAX) * 2.5f) + 1.5f;
	play();
	reflect.horizontal = (rand() % 2 == 0);
	reflect.vertical = (rand() % 2 == 0);
}

void Ember::update(float dt) {
	if (m_alive) {
		Animation::update(dt);
	}
	if (m_timer > 40.0f * 8) {
		m_alive = false;
	}
	if ((int)m_timer % 10 == 0 && rand() % 2 == 0) {
		velocity.x = -velocity.x;
	}
	m_timer += dt;
}

void Ember::destroy() {
	Animation::destroy();
	m_alive = true;
	m_timer = 0.0f;
}

bool Ember::isAlive() const {
	return m_alive;
}