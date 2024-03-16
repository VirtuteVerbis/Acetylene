// Acetylene ~ Entity Header
// Record 01-000A
// 2024.03.14 @ 17.01

// File:	Entity.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.01
// Purpose:	Declare several classes representing 2D entities in a virtual world.

#ifndef AC_ENTITY_H
#define AC_ENTITY_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Acetylene {
	// Abstract 2D entity with position and velocity.
	class Entity {
	public:
		// The position and depth of this entity.
		glm::vec3 position = glm::vec3();
		// The velocity of this entity.
		glm::vec2 velocity = glm::vec2();

		/*
		Initialize this entity's memory.
		@param const glm::vec3& position: The initial 2D position and depth of
		this entity.
		@param const glm::vec2& velocity: The initial 2D velocity of this
		entity.
		*/
		void init(const glm::vec3&, const glm::vec2&);
		/*
		Update this entity's position by its velocity.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float);
		/*
		Free this entity's memory.
		*/
		virtual void destroy();
	};

	// Class for generating 2D projection matrices with position and scale.
	class Camera : public Entity {
	public:
		// The scale and scale velocity of this camera's projection.
		struct Scale {
			// The current scale of this camera.
			float scale = 1.0f;
			// The scale velocity of this camera.
			float velocity = 0.0f;
		} scale;

		/*
		Assignment operator for cameras.
		@param const Camera& c: The camera to copy the position, velocity,
		and scale of.
		*/
		void operator = (const Camera&);
		/*
		Initialize this camera's memory and projection matrix.
		*/
		void init();
		/*
		Update this camera's projection matrix if required.
		@param float dt: The number of frames elapsed since the last update.
		*/
		void update(float) override;
		/*
		Get the projection matrix for this camera's position and scale.
		@return glm::mat4&: The camera matrix.
		*/
		glm::mat4& getMatrix();
		/*
		Free this camera's memory.
		*/
		void destroy() override;

	private:
		// This camera's current projection matrix.
		glm::mat4 m_matrix = glm::mat4();
		// The position of this camera in the previous update.
		glm::vec3 m_prevPosition = glm::vec3();
		// The scale of this camera in the previous update.
		float m_prevScale = 1.0f;
		// The dimensions of the window in the previous update.
		glm::ivec2 m_prevWindowDimensions = glm::ivec2();

		/*
		Update this camera's projection matrix.
		*/
		void updateMatrix();
	};

	// Abstract 2D sprite with single textured quad.
	class Sprite : public Entity {
	public:
		// The dimensions of this sprite.
		glm::vec2 dimensions = glm::vec2();
		// The rotation and rotation velocity of this sprite in degrees.
		struct Rotation {
			// The current rotation of this sprite in degrees.
			float rotation = 0.0f;
			// The rotation velocity of this sprite in degrees.
			float velocity = 0.0f;
		} rotation;
		// The texture ID and coordinates for this sprite.
		struct Texture {
			// The texture file name for this sprite.
			std::string fileName = "";
			// The texture coordinates for this sprite.
			glm::vec4 coordinates = glm::vec4();
		} texture;
		// The color of this sprite if no texture.
		glm::vec4 color = glm::vec4();
		// Whether to reflect this sprite horizontally or vertically.
		struct Reflection {
			// Whether to reflect this sprite horizontally.
			bool horizontal = false;
			// Whether to reflect this sprite vertically.
			bool vertical = false;
		} reflect;

		/*
		Initialize this sprite's memory.
		@param const glm::vec3& position: The initial position and depth of
		this sprite.
		@param const glm::vec2& dimensions: The initial dimensions of this
		sprite.
		@param const std::string& textureFileName: The file name of the texture
		to render on this sprite.
		*/
		void init(const glm::vec3&, const glm::vec2&, const std::string&);
		/*
		Update this sprite's physics.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this sprite's memory.
		*/
		virtual void destroy() override;
		/*
		Test whether this sprite intersects another.
		@param const Sprite& s: The other sprite to test against.
		*/
		virtual bool intersects(const Sprite&);

	protected:
		// Allow the graphics manager class to access protected Sprite memory.
		friend class GraphicsManager;

		// Indices for drawing sprites with OpenGL.
		static std::vector<unsigned int> Indices;
		// This sprite's vertex data.
		std::vector<float> m_vertices;

		/*
		Populate this sprites vertex data based on position, dimensions,
		rotation, color, and texture coordinates.
		*/
		virtual void draw();
		/*
		Rotate a 2D point about the center of this sprite.
		@param const glm::vec3& p: The 2D point and its depth.
		@return glm::vec3: The rotated point and its depth.
		*/
		glm::vec3 rotate(const glm::vec3&);
	};

	// Looping animation class based on 2D sprite.
	class Animation : public Sprite {
	public:
		/*
		Initialize this animation's memory.
		@param const glm::vec3& position: The initial position of this animated
		sprite.
		@param const glm::vec2& dimensions: The initial dimensions of this
		animated sprite.
		@param const std::string& textureFileName: The file name of the texture
		to derive this animation's frames from.
		@param const glm::ivec2& frameDimensions: The number of frames in the
		texture in the horizontal and vertical directions.
		@param float frameTime: The number of frames each frame of this
		animation will be displayed for.
		*/
		void init(const glm::vec3&, const glm::vec2&, const std::string&,
			const glm::ivec2&, float);
		/*
		Update this animation's physics and texture.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Start playing this animation from the current frame.
		*/
		virtual void play();
		/*
		Pause this animation on the current frame.
		*/
		virtual void pause();
		/*
		Stop this animation and reset to its first frame.
		*/
		virtual void stop();
		/*
		Test whether this animation is currently playing.
		@return bool: Whether this animation is playing.
		*/
		virtual bool isPlaying() const;
		/*
		Free this animation's memory.
		*/
		virtual void destroy() override;

	protected:
		// Time step to display each frame for.
		float m_frameTime = 0.0f;
		// The texture coordinates for the frames of this animation.
		std::vector<glm::vec2> m_frameCoords;
		// Whether this animation is currently playing.
		bool m_playing = false;
		// Timer for tracking the animation frame.
		float m_timer = 0.0f;
		// The current frame index of this animation.
		unsigned int m_frame = 0;
	};

	// Enumeration of alignments for rendering text.
	enum Alignment {
		AC_ALIGN_LEFT, AC_ALIGN_RIGHT, AC_ALIGN_BOTTOM, AC_ALIGN_TOP,
		AC_ALIGN_CENTER, AC_ALIGN_NONE,
	};

	// A single character in a font.
	class Glyph : public Sprite {
	public:
		// Bearing offset for this sprite's character.
		glm::ivec2 bearing = glm::ivec2();
		// Width of this character.
		unsigned int offset = 0;

		/*
		Populate this glyphs vertices.
		*/
		void draw() override;
	};

	// Set of glyphs loaded from a TTF file via FreeType.
	class Font {
	public:
		// The set of characters in this font mapped to their glyphs.
		std::map<char, Glyph> glyphs;
		// The minimum bearing of the characters in this font.
		float minBearing = 0.0f;
		// The maximum bearing of the characters in this font.
		float maxBearing = 0.0f;
	};
}

#endif