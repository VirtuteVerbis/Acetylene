// Acetylene ~ Graphics Header
// Record 01-000C
// 2024.03.14 @ 17.04

// File:	Graphics.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.04
// Purpose:	Declare the OpenGL shader and graphics rendering system used by
//			Acetylene apps.

#ifndef AC_GRAPHICS_H
#define AC_GRAPHICS_H

#include "Entity.h"

#include <map>
#include <ft2build.h>
#include <freetype/freetype.h>

namespace Acetylene {
	// Compiler and wrapper for OpenGL GLSL vertex and fragment shader program.
	class Shader {
	public:
		/*
		Assignment operator for shaders, copies the source code, recompiles the
		shader, and copies the camera.
		@param const Shader& s: The shader to copy.
		*/
		void operator = (const Shader&);
		/*
		Compile an OpenGL shader and initialize it in memory.
		@param const std::string& vSource: The source code of the vertex
		shader.
		@param const std::string& fSource: The source code of the fragment
		shader.
		@param Camera& camera: The camera to use for drawing with this shader.
		@param const std::string& cameraUniform: The name of the 4x4 matrix
		uniform variable in the shader for the camera projection matrix.
		@param const std::vector<unsigned int>& attributeSizes: The sizes in
		number of floats of the vertex attributes which will be passed to this
		shader for drawing.
		@return bool: Whether the shader compiled and initialized successfully.
		*/
		bool init(const std::string&, const std::string&, Camera&,
			const std::string&, const std::vector<unsigned int>&);
		/*
		Enable this shader and its vertex attributes.
		*/
		void begin();
		/*
		Disable this shader and its vertex attributes.
		*/
		void end();
		/*
		Free this shader's memory.
		*/
		void destroy();
		/*
		Get this shader's camera.
		@return Camera&: A reference to the camera.
		*/
		Camera& getCamera();
		/*
		Set this shader's camera.
		@param Camera& camera: A reference to the new camera.
		*/
		void setCamera(Camera&);
		/*
		Upload an integer uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param int value: The integer value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadInt(const std::string&, int);
		/*
		Upload an floating point uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param float value: The float value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadFloat(const std::string&, float);
		/*
		Upload an 2D vector uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param glm::vec2& value: The 2D vector value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadVec2(const std::string&, glm::vec2&);
		/*
		Upload an 3D vector uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param glm::vec3& value: The 3D vector value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadVec3(const std::string&, glm::vec3&);
		/*
		Upload an 4D vector uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param float value: The 4D vector value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadVec4(const std::string&, glm::vec4&);
		/*
		Upload an 4x4 matrix uniform variable to this shader.
		@param const std::string& name: The uniform variable name.
		@param glm::mat4& value: The 4x4 matrix value for this variable.
		@return bool: Whether the upload was successful.
		*/
		bool uploadMat4(const std::string&, glm::mat4&);

	private:
		// This shader's vertex shading source code.
		std::string m_vSource = "";
		// This shader's fragment shading source code.
		std::string m_fSource = "";
		// The OpenGL program ID for this shader.
		GLuint m_ID = 0;
		// Vertex attribute data.
		struct VertexAttribute {
			// The size in bytes of this attribute.
			unsigned int size = 0;
			// The offset to the start of this attribute in vertex data.
			unsigned int offset = 0;
		};
		// The set of vertex attributes for this shader.
		std::vector<VertexAttribute> m_attributes;
		// Size in bytes of all vertex attributes.
		int m_stride = 0;
		// This shader's camera.
		Camera* m_camera = 0;
		// The uniform variable name for this shader's camera matrix.
		std::string m_cameraUniform = "";

		/*
		Get the location of a uniform variable in this shader.
		@param const std::string& name: The name of the uniform variable.
		@param GLint& location: A reference to an integer to populate with the
		location of the uniform variable if it could be found.
		@return bool: Whether the uniform variable was found.
		*/
		bool getUniformLocation(const std::string&, GLint&);
	};

	// App utility for drawing sprites and text via GLEW / OpenGL.
	class GraphicsManager {
	public:
		/*
		Initialize the GLEW library and set the window background color.
		@param const glm::vec3& backgroundColor: The RGB color for the
		background of the window.
		@return bool: Whether GLEW could be initialized and the default shader
		could be compiled.
		*/
		bool init(const glm::vec3&);
		/*
		Update the default camera for this renderer.
		@param float dt: The number of frames elapsed since the last update.
		*/
		void update(float);
		/*
		Clear the screen and all previous frame data.
		*/
		void begin();
		/*
		Submit a single sprite with a custom shader.
		@param T& sprite: A reference to the sprite object to draw (must have
		a draw function and an m_vertices member variable of type
		std::vector<float>).
		@param Shader& shader: A reference to the shader to draw the sprite
		with.
		*/
		template <typename T>
		void submit(T& sprite, Shader& shader) {
			// Get the sprite's vertex data.
			sprite.draw();
			Sprite* s = (Sprite*)&sprite;
			if (s->m_vertices.empty()) {
				return;
			}
			// Add the sprite to this frame.
			m_sprites.push_back(std::pair<Shader*, Sprite*>(&shader, s));
		}
		/*
		Submit a single sprite for the default shader.
		@param T& sprite: A reference to the sprite object to draw (must have
		a draw function and an m_vertices member variable of type
		std::vector<float>).
		*/
		template <typename T>
		void submit(T& sprite) {
			submit(sprite, m_defaultShader);
		}
		/*
		Submit a set of sprites with a custom shader.
		@param std::vector<T>& sprites: A reference to the set of sprite
		objects to draw (they must have a draw function and an m_vertices
		member variable of type std::vector<float>).
		@param Shader& shader: A reference to the shader to draw the sprites
		with.
		*/
		template <typename T>
		void submit(std::vector<T>& sprites, Shader& shader) {
			for (T& sprite : sprites) {
				submit(sprite, shader);
			}
		}
		/*
		Submit a set of sprites for the default shader.
		@param std::vector<T>& sprites: A reference to the set of sprite
		objects to draw (they must have a draw function and an m_vertices
		member variable of type std::vector<float>).
		*/
		template <typename T>
		void submit(std::vector<T>& sprites) {
			submit(sprites, m_defaultShader);
		}
		/*
		Submit a text string to render with a custom shader.
		@param const std::string& text: The text to draw.
		@param const glm::vec3& position: The position and depth to draw the
		text at.
		@param const glm::vec4& box: The bounding box to draw the text in (in
		format x, y, width, height).
		@param float scale: The scale to draw the text at.
		@param const glm::vec4& color: The RGBA color to draw the text in.
		@param const std::string& fontFileName: The file name of the font to
		draw the text in.
		@param Alignment horizontalAlignment: The horizontal alignment of the
		text in its bounding box.
		@param Alignment verticalAlignment: The vertical alignment of the text
		in its bounding box.
		@param Shader& shader: The shader to use to draw the text.
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&,
			float, const glm::vec4&, const std::string&, Alignment, Alignment,
			Shader&);
		/*
		Submit a text string to render with the default shader.
		@param const std::string& text: The text to draw.
		@param const glm::vec3& position: The position and depth to draw the
		text at.
		@param const glm::vec4& box: The bounding box to draw the text in (in
		format x, y, width, height).
		@param float scale: The scale to draw the text at.
		@param const glm::vec4& color: The RGBA color to draw the text in.
		@param const std::string& fontFileName: The file name of the font to
		draw the text in.
		@param Alignment horizontalAlignment: The horizontal alignment of the
		text in its bounding box.
		@param Alignment verticalAlignment: The vertical alignment of the text
		in its bounding box.
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&,
			float, const glm::vec4&, const std::string&, Alignment, Alignment);
		/*
		Draw all the sprites submitted this frame.
		*/
		void end();
		/*
		Free this renderer's memory.
		*/
		void destroy();
		/*
		Get the default camera for this renderer.
		@return Camera&: A reference to the default camera.
		*/
		Camera& getDefaultCamera();
		/*
		Get the default shader for this renderer.
		@return Shader&: A reference to the default shader.
		*/
		Shader& getDefaultShader();
		/*
		Set the default shader for this renderer.
		@param Shader& shader: A reference to the new default shader.
		*/
		void setDefaultShader(Shader&);
		
	private:
		// Allow the TextBox UI element to access font pointers.
		friend class TextBox;
		// Allow the TextArea UI element to access font pointers.
		friend class TextArea;
		// Allow the PasswordBox UI element to access font pointers.
		friend class PasswordBox;

		// The vertex array object for this renderer.
		GLuint m_VAO = 0;
		// The vertex buffer object for this renderer.
		GLuint m_VBO = 0;
		// The index buffer object for this renderer.
		GLuint m_IBO = 0;
		// Set of textures loaded from PNG files.
		std::map<std::string, GLuint> m_textures;
		// Set of sprites submitted this frame.
		std::vector<std::pair<Shader*, Sprite*>> m_sprites;
		// The default camera for this renderer.
		Camera m_defaultCamera;
		// The default shader for this renderer.
		Shader m_defaultShader;
		// Instance of the FreeType vendor library used to load TTF files.
		FT_Library m_freeType = 0;
		// Set of fonts loaded from TTF files.
		std::map<std::string, Font> m_fonts;
		// Set of text characters submitted to the renderer this frame.
		std::map<Shader*, std::vector<Glyph>> m_glyphs;

		/*
		Get a texture from memory by its file name or load it from a PNG.
		@param const std::string& fileName: The file name of the texture.
		@return const GLuint: The ID number of the texture in OpenGL.
		*/
		const GLuint getTexture(const std::string&);
		/*
		Get a font from memory by its file name or load it from a TTF.
		@param const std::string& fileName: The file name of the font.
		@return const Font*: A pointer to the font loaded.
		*/
		const Font* getFont(const std::string&);
		/*
		Draw a set of vertices by indices with OpenGL and clear them.
		@param std::vector<float>& vertices: A reference to the vertex data to
		draw with OpenGL.
		@param std::vector<unsigned int>& indices: A reference to the indices
		of the vertex data to draw with OpenGL.
		*/
		void drawVertices(std::vector<float>&, std::vector<unsigned int>&);
	};
}

#endif