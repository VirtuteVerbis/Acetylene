// Acetylene ~ Entity Implementation
// Record 01-000B
// 2024.03.14 @ 17.03

// File:	Entity.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.03
// Purpose:	Implement functions declared in Entity.h.

#include "Acetylene.h"

#include <glm/gtc/matrix_transform.hpp>

#define PI 3.1415926f

namespace Acetylene {
	// Implement Entity class functions.

	void Entity::init(const glm::vec3& position, const glm::vec2& velocity) {
		this->position = position;
		this->velocity = velocity;
	}

	void Entity::update(float dt) {
		position.x += dt * velocity.x;
		position.y += dt * velocity.y;
	}

	void Entity::destroy() {
		position = glm::vec3();
		velocity = glm::vec2();
	}

	// Implement Camera class functions.

	void Camera::operator = (const Camera& c) {
		position = c.position;
		velocity = c.velocity;
		scale = c.scale;
		m_prevPosition = position;
		m_prevScale = scale.scale;
		m_prevWindowDimensions = App::Window.getDimensions();
		updateMatrix();
	}

	void Camera::init() {
		Entity::init(glm::vec3(), glm::vec2());
		scale.scale = 1.0f;
		scale.velocity = 0.0f;
		m_prevPosition = position;
		m_prevScale = scale.scale;
		m_prevWindowDimensions = App::Window.getDimensions();
		updateMatrix();
	}

	void Camera::update(float dt) {
		Entity::update(dt);
		scale.scale += dt * scale.velocity * scale.scale;
		if (scale.scale < 0.0f) {
			scale.scale = 0.0f;
		}
		if (position != m_prevPosition || scale.scale != m_prevScale
			|| m_prevWindowDimensions != App::Window.getDimensions()) {
			updateMatrix();
			m_prevPosition = position;
			m_prevScale = scale.scale;
			m_prevWindowDimensions = App::Window.getDimensions();
		}
	}

	void Camera::updateMatrix() {
		glm::vec2 wd((float)App::Window.getDimensions().x,
			(float)App::Window.getDimensions().y);
		glm::vec2 wscale(wd.x / m_prevWindowDimensions.x,
			wd.y / m_prevWindowDimensions.y);
		position.x *= wscale.x;
		position.y *= wscale.y;
		m_matrix = glm::ortho(-wd.x / 2.0f, wd.x / 2.0f, -wd.y / 2.0f,
			wd.y / 2.0f);
		m_matrix = glm::translate(m_matrix, glm::vec3(-position.x, -position.y,
			0.0f));
		m_matrix = glm::scale(glm::mat4(1.0f),
			glm::vec3(scale.scale, scale.scale, 0.0f)) * m_matrix;
	}

	glm::mat4& Camera::getMatrix() {
		return m_matrix;
	}

	void Camera::destroy() {
		Entity::destroy();
		scale.scale = 1.0f;
		scale.velocity = 0.0f;
		m_matrix = glm::mat4();
		m_prevPosition = glm::vec3();
		m_prevScale = 1.0f;
		m_prevWindowDimensions = glm::ivec2();
	}

	// Implement Sprite class functions.

	std::vector<unsigned int> Sprite::Indices = { 0, 1, 2, 2, 3, 0 };

	void Sprite::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName) {
		Entity::init(position, glm::vec2(0.0f, 0.0f));
		this->dimensions = dimensions;
		rotation.rotation = 0.0f;
		rotation.velocity = 0.0f;
		texture.fileName = textureFileName;
		texture.coordinates = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m_vertices.clear();
	}

	void Sprite::update(float dt) {
		Entity::update(dt);
		rotation.rotation += dt * rotation.velocity;
	}

	void Sprite::destroy() {
		Entity::destroy();
		dimensions = glm::vec2();
		rotation.rotation = 0.0f;
		rotation.velocity = 0.0f;
		texture.fileName = "";
		texture.coordinates = glm::vec4();
		color = glm::vec4();
		reflect.horizontal = false;
		reflect.vertical = false;
		m_vertices.clear();
	}

	bool Sprite::intersects(const Sprite& s) {
		if (position.x > s.position.x + s.dimensions.x
			|| position.x + dimensions.x < s.position.x) {
			return false;
		}
		if (position.y > s.position.y + s.dimensions.y
			|| position.y + dimensions.y < s.position.y) {
			return false;
		}
		return true;
	}

	void Sprite::draw() {
		glm::vec3 bl(position.x, position.y, position.z);
		glm::vec3 br(position.x + dimensions.x, position.y, position.z);
		glm::vec3 tl(position.x + dimensions.x, position.y + dimensions.y,
			position.z);
		glm::vec3 tr(position.x, position.y + dimensions.y, position.z);
		if ((int)rotation.rotation % 360 != 0) {
			bl = rotate(bl);
			br = rotate(br);
			tl = rotate(tl);
			tr = rotate(tr);
		}
		if (!reflect.horizontal && !reflect.vertical) {
			m_vertices = {
				bl.x, bl.y, bl.z,
				texture.coordinates.x,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				br.x, br.y, br.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tl.x, tl.y, tl.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tr.x, tr.y, tr.z,
				texture.coordinates.x, texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
			};
		}
		else if (reflect.horizontal && !reflect.vertical) {
			m_vertices = {
				bl.x, bl.y, bl.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				br.x, br.y, br.z,
				texture.coordinates.x,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tl.x, tl.y, tl.z,
				texture.coordinates.x, texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tr.x, tr.y, tr.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
			};
		}
		else if (!reflect.horizontal && reflect.vertical) {
			m_vertices = {
				bl.x, bl.y, bl.z,
				texture.coordinates.x, texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				br.x, br.y, br.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tl.x, tl.y, tl.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tr.x, tr.y, tr.z,
				texture.coordinates.x,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
			};
		}
		else if (reflect.horizontal && reflect.vertical) {
			m_vertices = {
				bl.x, bl.y, bl.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				br.x, br.y, br.z,
				texture.coordinates.x, texture.coordinates.y,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tl.x, tl.y, tl.z,
				texture.coordinates.x,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
				tr.x, tr.y, tr.z,
				texture.coordinates.x + texture.coordinates.z,
					texture.coordinates.y + texture.coordinates.w,
				color.x, color.y, color.z, color.w,
				(float)texture.fileName.empty(),
			};
		}
	}

	glm::vec3 Sprite::rotate(const glm::vec3& p) {
		float a = (PI / 180.0f) * rotation.rotation;
		glm::vec2 c = glm::vec2(position.x, position.y) + (dimensions / 2.0f);
		float rx = cos(a) * (p.x - c.x) - sin(a) * (p.y - c.y) + c.x;
		float ry = sin(a) * (p.x - c.x) + cos(a) * (p.y - c.y) + c.y;
		return glm::vec3(rx, ry, p.z);
	}

	// Implement Animation class functions.

	void Animation::init(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const glm::ivec2& frameDimensions, float frameTime) {
		Sprite::init(position, dimensions, textureFileName);
		texture.coordinates = glm::vec4(0.0f, 0.0f,
			1.0f / (float)frameDimensions.x, 1.0f / (float)frameDimensions.y);
		m_frameCoords.resize(frameDimensions.x * frameDimensions.y);
		for (int y = 0; y < frameDimensions.y; y++) {
			for (int x = 0; x < frameDimensions.x; x++) {
				m_frameCoords[x + y * frameDimensions.x]
					= glm::vec2(texture.coordinates.z * x,
						texture.coordinates.w * y);
			}
		}
		m_frameTime = frameTime;
	}

	void Animation::update(float dt) {
		Sprite::update(dt);
		if (!m_playing) {
			return;
		}
		if (m_timer > m_frameTime) {
			m_timer = 0.0f;
			m_frame = (m_frame + 1) % m_frameCoords.size();
		}
		m_timer += dt;
		texture.coordinates.x = m_frameCoords.at(m_frame).x;
		texture.coordinates.y = m_frameCoords.at(m_frame).y;
	}

	void Animation::play() {
		m_playing = true;
	}

	void Animation::pause() {
		m_playing = false;
	}

	void Animation::stop() {
		m_playing = false;
		m_timer = 0.0f;
		m_frame = 0;
		texture.coordinates.x = m_frameCoords.at(0).x;
		texture.coordinates.y = m_frameCoords.at(0).y;
	}

	bool Animation::isPlaying() const {
		return m_playing;
	}

	void Animation::destroy() {
		Sprite::destroy();
		m_frameTime = 0.0f;
		m_frameCoords.clear();
		m_playing = false;
		m_timer = 0.0f;
		m_frame = 0;
	}

	// Implement Glyph class functions.

	void Glyph::draw() {
		glm::vec3 bl(position.x, position.y, position.z);
		glm::vec3 br(position.x + dimensions.x, position.y, position.z);
		glm::vec3 tl(position.x + dimensions.x, position.y + dimensions.y,
			position.z);
		glm::vec3 tr(position.x, position.y + dimensions.y, position.z);
		m_vertices = {
			bl.x, bl.y, bl.z,
			texture.coordinates.x,
				texture.coordinates.y + texture.coordinates.w,
			color.x, color.y, color.z, color.w,
			2.0f,
			br.x, br.y, br.z,
			texture.coordinates.x + texture.coordinates.z,
				texture.coordinates.y + texture.coordinates.w,
			color.x, color.y, color.z, color.w,
			2.0f,
			tl.x, tl.y, tl.z,
			texture.coordinates.x + texture.coordinates.z,
				texture.coordinates.y,
			color.x, color.y, color.z, color.w,
			2.0f,
			tr.x, tr.y, tr.z,
			texture.coordinates.x, texture.coordinates.y,
			color.x, color.y, color.z, color.w,
			2.0f,
		};
	}
}