// Acetylene ~ Graphics Implementation
// Record 01-000D
// 2024.03.14 @ 17.05

// File:	Graphics.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.05
// Purpose:	Implement functions declared in Graphics.h.

#include "Acetylene.h"

#include <fstream>
#include <algorithm>

/*
Reference to the picoPNG's decodePNG function.
@param std::vector<unsigned char>& out_image: A reference to the buffer to fill
with decoded PNG data, by default this will contain 32-bit RGBA color.
@param unsigned long& image_width: A reference to the value to fill with the
width of the decoded image in pixels.
@param unsigned long& image_height: A reference to the value to fill with the
height of the decoded image in pixels.
@param const unsigned char* in_png: The encoded buffer of PNG data for picoPNG
to decode.
@param size_t in_size: The length of the in_png buffer.
@param bool convert_to_rgba32: Optional parameter which toggles whether picoPNG
will do color conversion to 32-bit RGBA color.
@return int: 0 if success, not 0 if some error occurred.
*/
extern int decodePNG(std::vector<unsigned char>&, unsigned long&,
	unsigned long&, const unsigned char*, size_t, bool = true);

namespace Acetylene {
	// Implement Shader class functions.

	void Shader::operator = (const Shader& s) {
		std::vector<unsigned int> attribSizes;
		for (const VertexAttribute& va : s.m_attributes) {
			attribSizes.push_back(va.size);
		}
		m_camera = s.m_camera;
		init(s.m_vSource, s.m_fSource, *m_camera, s.m_cameraUniform,
			attribSizes);
	}

	bool Shader::init(const std::string& vSource, const std::string& fSource,
		Camera& camera, const std::string& cameraUniform,
		const std::vector<unsigned int>& attributeSizes) {
		m_vSource = vSource;
		m_fSource = fSource;
		const char* vSourceC = vSource.c_str();
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vSourceC, 0);
		glCompileShader(vShader);
		int vs;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vs);
		if (!vs) {
			char vbuf[1024];
			glGetShaderInfoLog(vShader, 1024, 0, vbuf);
			ACLOG(Shader, Warning, "Failed to compile vertex shader ", vShader,
				".\n", vbuf);
			return false;
		}
		ACLOG(Shader, Message, "Compiled vertex shader ", vShader, ".");
		const char* fSourceC = fSource.c_str();
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fSourceC, 0);
		glCompileShader(fShader);
		int fs;
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fs);
		if (!fs) {
			char fbuf[1024];
			glGetShaderInfoLog(fShader, 1024, 0, fbuf);
			ACLOG(Shader, Message, "Failed to compile fragment shader ",
				fShader, ".\n", fbuf);
			return false;
		}
		ACLOG(Shader, Message, "Compiled fragment shader ", fShader, ".");
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vShader);
		glAttachShader(m_ID, fShader);
		glLinkProgram(m_ID);
		int ps;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &ps);
		if (!ps) {
			char pbuf[1024];
			glGetProgramInfoLog(m_ID, 1024, 0, pbuf);
			ACLOG(Shader, Warning, "Failed to link shader program ", m_ID,
				".\n", pbuf);
			return false;
		}
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		ACLOG(Shader, Message, "Linked shader program ", m_ID, ".");
		m_attributes.resize(attributeSizes.size());
		m_stride = 0;
		for (unsigned int a = 0; a < attributeSizes.size(); a++) {
			m_attributes[a].size = attributeSizes.at(a);
			m_attributes[a].offset = m_stride * sizeof(float);
			m_stride += attributeSizes.at(a);
		}
		m_stride *= sizeof(float);
		m_camera = &camera;
		m_camera->init();
		m_cameraUniform = cameraUniform;
		return true;
	}

	void Shader::begin() {
		glUseProgram(m_ID);
		for (unsigned int a = 0; a < m_attributes.size(); a++) {
			glVertexAttribPointer(a, m_attributes.at(a).size, GL_FLOAT,
				GL_FALSE, m_stride, (void*)m_attributes.at(a).offset);
			glEnableVertexAttribArray(a);
		}
		if (!m_cameraUniform.empty()) {
			uploadMat4(m_cameraUniform, m_camera->getMatrix());
		}
	}

	void Shader::end() {
		for (unsigned int a = 0; a < m_attributes.size(); a++) {
			glDisableVertexAttribArray(a);
		}
		glUseProgram(0);
	}

	void Shader::destroy() {
		ACLOG(Shader, Message, "Destroying shader ", m_ID, ".");
		glDeleteProgram(m_ID);
		m_vSource = "";
		m_fSource = "";
		m_ID = 0;
		m_attributes.clear();
		m_stride = 0;
		m_camera->destroy();
		m_camera = 0;
		m_cameraUniform = "";
	}

	Camera& Shader::getCamera() {
		return *m_camera;
	}

	void Shader::setCamera(Camera& camera) {
		m_camera = &camera;
	}

	bool Shader::uploadInt(const std::string& name, int value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniform1i(location, value);
		return true;
	}

	bool Shader::uploadFloat(const std::string& name, float value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniform1f(location, value);
		return true;
	}

	bool Shader::uploadVec2(const std::string& name, glm::vec2& value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniform2f(location, value.x, value.y);
		return true;
	}

	bool Shader::uploadVec3(const std::string& name, glm::vec3& value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniform3f(location, value.x, value.y, value.z);
		return true;
	}

	bool Shader::uploadVec4(const std::string& name, glm::vec4& value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniform4f(location, value.x, value.y, value.z, value.w);
		return true;
	}

	bool Shader::uploadMat4(const std::string& name, glm::mat4& value) {
		GLint location;
		if (!getUniformLocation(name, location)) {
			return false;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0]));
		return true;
	}

	bool Shader::getUniformLocation(const std::string& name, GLint& location) {
		location = glGetUniformLocation(m_ID, name.c_str());
		return location != GL_INVALID_VALUE && location != GL_INVALID_OPERATION;
	}

	// Implement Renderer class functions.

	bool GraphicsManager::init(const glm::vec3& backgroundColor) {
		if (glewInit() != GLEW_OK) {
			ACLOG(Renderer, Error, "Failed to initialize GLEW library.");
			return false;
		}
		const GLubyte* version = glGetString(GL_VERSION);
		ACLOG(Renderer, Message, "Initialized GLEW library: ", version, ".");
		if (FT_Init_FreeType(&m_freeType) != FT_Err_Ok) {
			ACLOG(Renderer, Error, "Failed to initialize FreeType library.");
			return false;
		}
		ACLOG(Renderer, Message, "Initialized FreeType library.");
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z,
			1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		ACLOG(Renderer, Message, "Set up initial OpenGL parameters.");
		m_defaultShader.init(
			"#version 330 core\n"
			"layout (location = 0) in vec3 vertexPosition;\n"
			"layout (location = 1) in vec2 vertexTexturePosition;\n"
			"layout (location = 2) in vec4 vertexColor;\n"
			"layout (location = 3) in float mode;\n"
			"uniform mat4 cameraMatrix;\n"
			"out vec2 fragmentTexturePosition;\n"
			"out vec4 fragmentColor;\n"
			"out float fragmentMode;\n"
			"void main() {\n"
			"	gl_Position = cameraMatrix * vec4(vertexPosition, 1.0);\n"
			"	fragmentTexturePosition = vertexTexturePosition;\n"
			"	fragmentColor = vertexColor;\n"
			"	fragmentMode = mode;\n"
			"}\n",

			"#version 330 core\n"
			"in vec2 fragmentTexturePosition;\n"
			"in vec4 fragmentColor;\n"
			"in float fragmentMode;\n"
			"out vec4 FragmentColor;\n"
			"uniform sampler2D textureSampler;\n"
			"void main() {\n"
			"	if (fragmentMode >= 2.0) {\n"
			"		vec4 textureColor = vec4(1.0, 1.0, 1.0,\n"
			"			texture(textureSampler, fragmentTexturePosition).r);\n"
			"		FragmentColor = fragmentColor * textureColor;\n"
			"	} else {\n"
			"		FragmentColor = (1.0 - fragmentMode)"
			"			* texture(textureSampler, fragmentTexturePosition)"
			"			+ fragmentMode * fragmentColor;\n"
			"	}\n"
			"}\n",

			m_defaultCamera, "cameraMatrix", { 3, 2, 4, 1 }
		);
		ACLOG(Renderer, Message, "Compiled default shader.");
		return true;
	}

	void GraphicsManager::update(float dt) {
		m_defaultCamera.update(dt);
	}

	void GraphicsManager::begin() {
		glClear(GL_COLOR_BUFFER_BIT);
		m_sprites.clear();
		m_glyphs.clear();
	}

	void GraphicsManager::submit(const std::string& text,
		const glm::vec3& position, const glm::vec4& box, float scale,
		const glm::vec4& color, const std::string& fontFileName,
		Alignment horizontalAlignment, Alignment verticalAlignment,
		Shader& shader) {
		if (text.empty()) {
			return;
		}
		const Font* font = getFont(fontFileName);
		if (font == 0) {
			return;
		}
		if (font->glyphs.empty()) {
			return;
		}
		if (m_glyphs.find(&shader) == m_glyphs.end()) {
			m_glyphs[&shader] = std::vector<Glyph>(0);
		}
		float labelWidth = 0.0f;
		for (char c : text) {
			labelWidth += font->glyphs.at(c).offset;
		}
		labelWidth *= scale;
		float labelHeight = (font->minBearing + font->maxBearing) * scale;
		float x = 0.0f;
		switch (horizontalAlignment) {
		case AC_ALIGN_LEFT: {
			x = box.x;
			break;
		}
		case AC_ALIGN_RIGHT: {
			x = (box.x + box.z) - labelWidth;
			break;
		}
		case AC_ALIGN_CENTER: {
			float m = box.x + (box.z / 2.0f);
			x = m - (labelWidth / 2.0f);
			break;
		}
		default: {
			x = position.x;
			break;
		}
		}
		x -= font->glyphs.at(text.at(0)).bearing.x * scale;
		float y = 0.0f;
		switch (verticalAlignment) {
		case AC_ALIGN_BOTTOM: {
			y = box.y + (font->minBearing * scale);
			break;
		}
		case AC_ALIGN_TOP: {
			y = (box.y + box.w) - labelHeight;
			break;
		}
		case AC_ALIGN_CENTER: {
			float m = box.y + (box.w / 2.0f);
			y = m - (labelHeight / 2.0f);
			break;
		}
		default: {
			y = position.y;
			break;
		}
		}
		for (unsigned int i = 0; i < text.length(); i++) {
			char c = text.at(i);
			Glyph g = font->glyphs.at(c);
			g.init(glm::vec3(x + (g.bearing.x * scale),
				y - ((g.dimensions.y - g.bearing.y) * scale), position.z),
				g.dimensions * scale, g.texture.fileName);
			if (!(g.position.x < box.x
				|| g.position.x + g.dimensions.x > box.x + box.z
				|| g.position.y < box.y
				|| g.position.y + g.dimensions.y > box.y + box.w)) {
				g.color = color;
				g.draw();
				m_glyphs[&shader].push_back(g);
			}
			else if (g.position.x + g.dimensions.x > box.x + box.z) {
				break;
			}
			x += g.offset * scale;
		}
	}

	void GraphicsManager::submit(const std::string& text,
		const glm::vec3& position, const glm::vec4& box, float scale,
		const glm::vec4& color, const std::string& fontFileName,
		Alignment hAlign, Alignment vAlign) {
		submit(text, position, box, scale, color, fontFileName, hAlign, vAlign,
			m_defaultShader);
	}

	/*
	Compare two sprites by their depth.
	@param const std::pair<Shader*, Sprite*>& a: A reference to the first
	sprite to compare paired with its shader.
	@param const std::pair<Shader*, Sprite*>& b: A reference to the second
	sprite to compare paired with its shader.
	@return Whether the z-coordinate of the position of sprite a is less than
	that of sprite b.
	*/
	bool CompareSpriteDepth(const std::pair<Shader*, Sprite*>& a,
		const std::pair<Shader*, Sprite*>& b) {
		if (a.second == 0 || b.second == 0) {
			return false;
		}
		return a.second->position.z < b.second->position.z;
	}

	void GraphicsManager::end() {
		if (!m_glyphs.empty()) {
			for (std::pair<Shader*, std::vector<Glyph>> p : m_glyphs) {
				submit(m_glyphs[p.first], *p.first);
			}
		}
		if (m_sprites.empty()) {
			return;
		}
		std::stable_sort(m_sprites.begin(), m_sprites.end(),
			CompareSpriteDepth);
		glBindVertexArray(m_VAO);
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		unsigned int count = 0;
		Shader* cShader = m_sprites.front().first;
		cShader->begin();
		GLuint cTexture = getTexture(
			m_sprites.front().second->texture.fileName);
		glBindTexture(GL_TEXTURE_2D, cTexture);
		for (const std::pair<Shader*, Sprite*>& sPair : m_sprites) {
			if (cShader != sPair.first) {
				if (count > 0) {
					drawVertices(vertices, indices);
					count = 0;
				}
				cShader->end();
				cShader = sPair.first;
				cShader->begin();
			}
			if (cTexture != getTexture(sPair.second->texture.fileName)) {
				if (count > 0) {
					drawVertices(vertices, indices);
					count = 0;
				}
				cTexture = getTexture(sPair.second->texture.fileName);
				glBindTexture(GL_TEXTURE_2D, cTexture);
			}
			for (float v : sPair.second->m_vertices) {
				vertices.push_back(v);
			}
			for (unsigned int i : Sprite::Indices) {
				indices.push_back(i + 4 * count);
			}
			count++;
		}
		if (count > 0) {
			drawVertices(vertices, indices);
		}
	}

	void GraphicsManager::destroy() {
		ACLOG(Renderer, Message, "Destroying renderer.");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
		ACLOG(Renderer, Message, "Freed vertex array, vertex buffer, and",
			" index buffer.");
		unsigned int tCount = 0;
		for (const std::pair<const std::string, GLuint>& t : m_textures) {
			glDeleteTextures(1, &t.second);
			tCount++;
		}
		m_textures.clear();
		ACLOG(Renderer, Message, "Deleted ", tCount, " textures from OpenGL.");
		m_sprites.clear();
		m_defaultShader.destroy();
		glUseProgram(0);
		ACLOG(Renderer, Message, "Freed default shader.");
		unsigned int fCount = (unsigned int)m_fonts.size();
		m_fonts.clear();
		ACLOG(Renderer, Message, "Freed ", fCount, " fonts.");
		FT_Done_FreeType(m_freeType);
		m_freeType = 0;
		ACLOG(Renderer, Message, "Freed FreeType library.");
	}

	Camera& GraphicsManager::getDefaultCamera() {
		return m_defaultCamera;
	}

	Shader& GraphicsManager::getDefaultShader() {
		return m_defaultShader;
	}

	void GraphicsManager::setDefaultShader(Shader& defaultShader) {
		m_defaultShader = defaultShader;
	}

	const GLuint GraphicsManager::getTexture(const std::string& fileName) {
		if (fileName.empty()) {
			return 0;
		}
		if (m_textures.find(fileName) != m_textures.end()) {
			return m_textures[fileName];
		}
		ACLOG(Renderer, Message, "Reading texture from \"", fileName, "\".");
		std::ifstream file(fileName, std::ios::in | std::ios::binary
			| std::ios::ate);
		if (!file.good()) {
			ACLOG(Renderer, Warning, "Failed to open texture file.");
			return 0;
		}
		std::streamsize fileSize = 0;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		fileSize -= file.tellg();
		if (fileSize <= 0) {
			ACLOG(Renderer, Warning, "Texture file has invalid size.");
			return 0;
		}
		std::vector<unsigned char> buffer;
		buffer.resize((size_t)fileSize);
		file.read((char*)(&buffer[0]), fileSize);
		file.close();
		std::vector<unsigned char> textureData;
		unsigned long w = 0, h = 0;
		if (decodePNG(textureData, w, h, &buffer[0], (size_t)fileSize) != 0) {
			ACLOG(Renderer, Warning, "Failed to decode PNG data.");
			return 0;
		}
		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, &textureData[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_textures[fileName] = textureID;
		return textureID;
	}

	const Font* GraphicsManager::getFont(const std::string& fileName) {
		if (m_fonts.find(fileName) != m_fonts.end()) {
			return &m_fonts[fileName];
		}
		ACLOG(Renderer, Message, "Loading font \"", fileName, "\".");
		FT_Face face;
		if (FT_New_Face(m_freeType, fileName.c_str(), 0, &face)
			!= FT_Err_Ok) {
			ACLOG(Renderer, Warning, "Failed to load font \"", fileName,
				"\".");
			return 0;
		}
		FT_Set_Pixel_Sizes(face, 0, 64);
		Font font;
		for (unsigned char g = 0; g < 128; g++) {
			if (FT_Load_Char(face, g, FT_LOAD_RENDER) != FT_Err_Ok) {
				ACLOG(Renderer, Warning, "Failed to load character '", (char)g,
					"' for font \"", fileName, "\".");
				continue;
			}
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
				face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);
			font.glyphs[g] = Glyph();
			font.glyphs[g].dimensions = glm::vec2(face->glyph->bitmap.width,
				face->glyph->bitmap.rows);
			font.glyphs[g].bearing = glm::ivec2(face->glyph->bitmap_left,
				face->glyph->bitmap_top);
			font.glyphs[g].offset = face->glyph->advance.x >> 6;
			font.glyphs[g].texture.fileName = fileName + "@" + (char)g;
			m_textures[font.glyphs[g].texture.fileName] = textureID;
		}
		FT_Done_Face(face);
		for (std::map<char, Glyph>::iterator it = font.glyphs.begin();
			it != font.glyphs.end(); it++) {
			float mib = it->second.dimensions.y
				- (float)it->second.bearing.y;
			if (mib > font.minBearing) {
				font.minBearing = mib;
			}
			float mab = (float)it->second.bearing.y;
			if (mab > font.maxBearing) {
				font.maxBearing = mab;
			}
		}
		m_fonts[fileName] = font;
		return &m_fonts[fileName];
	}

	void GraphicsManager::drawVertices(std::vector<float>& vertices,
		std::vector<unsigned int>& indices) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
			vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * indices.size(), indices.data(),
			GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT,
			(void*)0);
		vertices.clear();
		indices.clear();
	}
}