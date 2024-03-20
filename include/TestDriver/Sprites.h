// Acetylene ~ Test Driver Sprites Header
// Record 01-0016
// 2024.03.18 @ 15.26

// File:	Sprites.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.18 @ 15.26
// Purpose:	Declare all the classes to use for moving characters in the
//			TestDriver program.

#ifndef TD_SPRITES_H
#define TD_SPRITES_H

#include <Acetylene/Acetylene.h>

using namespace Acetylene;

// Animation representing a spark floating up from the bottom of a scene.
class Ember : public Animation {
public:
	/*
	Initialize this ember's position.
	@param const glm::vec3& position: The initial position of this ember.
	*/
	void init(const glm::vec3&);
	/*
	Update this ember's physics.
	@param float dt: The number of frames elapsed since the last update.
	*/
	void update(float);
	/*
	Free this ember's memory.
	*/
	void destroy();
	/*
	Test whether this ember has played its full animation.
	@return bool: Whether the ember is still alive.
	*/
	bool isAlive() const;

private:
	// Whether this ember has finished playing its animation.
	bool m_alive = true;
	// Timer for changing the velocity of this ember.
	float m_timer = 0.0f;
};

// A sprite representing a collectible coin within the game scene.
class Coin : public Sprite {
public:
	/*
	Initialize this coin's position.
	@param const glm::vec3& position: The position for this coin.
	*/
	void init(const glm::vec3&);
	/*
	Free this coin's memory.
	*/
	void destroy();
	/*
	Test whether this coin has been marked as collected by the player.
	@return bool: Whether this coin has been collected.
	*/
	bool isCollected() const;
	/*
	Set whether this coin has been collected by the player.
	@param bool collected: Whether the coin has been collected.
	*/
	void setCollected(bool);

private:
	// Whether this coin has been collected by the player.
	bool m_collected = false;
};

// Forward declare the GameScene class for the player.
class GameScene;

// A set of animations representing the user-controlled player character in the
// main game scene.
class Player {
public:
	/*
	Initialize the player's position.
	@param const glm::vec3& position: The initial position for the player.
	@param GameScene& parent: A reference to the game scene the player appears
	in.
	*/
	void init(const glm::vec3&, GameScene&);
	/*
	Draw the player's graphics.
	*/
	void draw();
	/*
	Process user input to the player.
	*/
	void processInput();
	/*
	Update the player's physics.
	@param float dt: The number of frames elapsed since the last update.
	*/
	void update(float);
	/*
	Free the player's memory.
	*/
	void destroy();
	/*
	Get the player's current position.
	@return const glm::vec3&: The current position of the player.
	*/
	const glm::vec3& getPosition() const;

private:
	// The game scene the player appears in.
	GameScene* m_parent = 0;
	// The walking animation to play when the player moves horizontally.
	Animation m_horizontalAnimation;
	// The walking animation to play when the player moves vertically. 
	Animation m_verticalAnimation;
	// A pointer to the animation currently being displayed on the game scene.
	Animation* m_animation = 0;
};

#endif