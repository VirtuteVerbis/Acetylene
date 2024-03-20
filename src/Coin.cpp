// Acetylene ~ Test Driver Coin Implementation
// Record 01-0018
// 2024.03.20 @ 02.51

// File:	Coin.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.20 @ 02.51
// Purpose:	Implement the functions declared in Sprites.h for the Coin class.

#include "Sprites.h"

// Implement Coin class functions.

void Coin::init(const glm::vec3& position) {
	Sprite::init(position, glm::vec2(16.0f, 16.0f), "Assets/textures/coin.png");
}

void Coin::destroy() {
	Sprite::destroy();
}

bool Coin::isCollected() const {
	return m_collected;
}

void Coin::setCollected(bool collected) {
	m_collected = collected;
}