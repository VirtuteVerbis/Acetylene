// Acetylene ~ UI Implementation
// Record 01-000F
// 2024.03.14 @ 17.10

// File:	UI.cpp
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.10
// Purpose:	Implement functions declared in UI.h.

#include "Acetylene.h"

namespace Acetylene {
	// Implement UIComponent class functions.

	UIListener& UIComponent::getParent() {
		return *m_parent;
	}

	unsigned int UIComponent::getID() const {
		return m_ID;
	}

	bool UIComponent::isEnabled() const {
		return m_enabled;
	}

	void UIComponent::setEnabled(bool enabled) {
		m_enabled = enabled;
	}

	bool UIComponent::isVisible() const {
		return m_visible;
	}

	void UIComponent::setVisible(bool visible) {
		m_visible = visible;
		if (!m_visible) {
			setEnabled(false);
		}
	}

	// Implement UIGroup class functions.

	void UIGroup::init(Scene& parent, unsigned int ID,
		const std::string& fontFileName, float textScale,
		const glm::vec4& textColor) {
		m_parent = &parent;
		m_ID = ID;
		m_components.clear();
		m_nextID = 0;
		m_shader = &App::Graphics.getDefaultShader();
		m_fontFileName = fontFileName;
		m_textScale = textScale;
		m_textColor = textColor;
		m_enabled = true;
		m_visible = true;
		m_prevWindowDimensions = App::Window.getDimensions();
		ACLOG(UIGroup, Message, "Initialized UI group ", m_ID, ".");
	}

	bool UIGroup::addComponent(UIComponent& component) {
		if (std::find(m_components.begin(), m_components.end(), &component)
			!= m_components.end()) {
			ACLOG(UIGroup, Warning, "UI component ", component.getID(),
				" already present in group ", m_ID, ".");
			return false;
		}
		component.m_parent = this;
		component.m_ID = m_nextID++;
		if (m_enabled) {
			App::Input.addListener(component);
		}
		m_components.push_back(&component);
		ACLOG(UIGroup, Message, "Added UI component ", component.getID(),
			" to group ", m_ID, ".");
		return true;
	}

	bool UIGroup::removeComponent(UIComponent& component) {
		std::vector<UIComponent*>::iterator it = std::find(
			m_components.begin(), m_components.end(), &component);
		if (it == m_components.end()) {
			ACLOG(UIGroup, Warning, "UI component ", component.getID(),
				" not present in group ", m_ID, ".");
			return false;
		}
		if (m_enabled) {
			App::Input.removeListener(component);
		}
		m_components.erase(it);
		ACLOG(UIGroup, Message, "Removed UI component ", component.getID(),
			" from group ", m_ID, ".");
		return true;
	}

	void UIGroup::draw() {
		if (m_visible) {
			for (UIComponent* component : m_components) {
				if (component->isVisible()) {
					component->draw();
				}
			}
		}
	}

	void UIGroup::processInput() {
		if (m_enabled) {
			for (UIComponent* component : m_components) {
				if (component->isEnabled()) {
					component->processInput();
				}
			}
		}
	}

	void UIGroup::componentEvent(int cID, int eID) {
		m_parent->componentEvent(m_ID, cID, eID);
	}

	void UIGroup::update(float dt) {
		for (UIComponent* component : m_components) {
			component->update(dt);
		}
		if (m_prevWindowDimensions != App::Window.getDimensions()) {
			glm::vec2 wd((float)App::Window.getDimensions().x,
				(float)App::Window.getDimensions().y);
			glm::vec2 scale(wd.x / (float)m_prevWindowDimensions.x,
				wd.y / (float)m_prevWindowDimensions.y);
			m_prevWindowDimensions = wd;
			for (UIComponent* component : m_components) {
				glm::vec2 dimensions = component->getDimensions();
				dimensions.x *= scale.x;
				dimensions.y *= scale.y;
				component->setDimensions(dimensions);
				glm::vec3 position = component->getPosition();
				position.x *= scale.x;
				position.y *= scale.y;
				component->setPosition(position);
			}
		}
	}

	void UIGroup::destroy() {
		ACLOG(UIGroup, Message, "Destroying UI group ", m_ID, ".");
		m_parent = 0;
		m_ID = 0;
		for (UIComponent* cmpt : m_components) {
			cmpt->destroy();
		}
		m_components.clear();
		m_nextID = 0;
		m_shader = 0;
		m_fontFileName = "";
		m_enabled = true;
		m_visible = true;
		m_prevWindowDimensions = glm::ivec2();
	}

	Scene& UIGroup::getParent() {
		return *m_parent;
	}

	unsigned int UIGroup::getID() const {
		return m_ID;
	}

	Shader& UIGroup::getShader() {
		return *m_shader;
	}

	void UIGroup::setShader(Shader& shader) {
		m_shader = &shader;
	}

	const std::string& UIGroup::getFontFileName() const {
		return m_fontFileName;
	}

	void UIGroup::setFontFileName(const std::string& fontFileName) {
		m_fontFileName = fontFileName;
	}

	float UIGroup::getTextScale() const {
		return m_textScale;
	}

	void UIGroup::setTextScale(float textScale) {
		m_textScale = textScale;
	}

	const glm::vec4& UIGroup::getTextColor() const {
		return m_textColor;
	}

	void UIGroup::setTextColor(const glm::vec4& textColor) {
		m_textColor = textColor;
	}

	bool UIGroup::isEnabled() const {
		return m_enabled;
	}

	void UIGroup::setEnabled(bool enabled) {
		if (m_enabled == enabled) {
			return;
		}
		m_enabled = enabled;
		if (m_enabled) {
			for (UIComponent* component : m_components) {
				App::Input.addListener(*component);
			}
			ACLOG(UIGroup, Message, "Enabled UI group ", m_ID, ".");
		}
		else {
			for (UIComponent* component : m_components) {
				App::Input.removeListener(*component);
			}
			ACLOG(UIGroup, Message, "Disabled UI group ", m_ID, ".");
		}
	}

	bool UIGroup::isVisible() const {
		return m_visible;
	}

	void UIGroup::setVisible(bool visible) {
		m_visible = visible;
	}

	// Implement Label class functions.

	void Label::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment) {
		m_background.init(glm::vec3(), glm::vec2(), textureFileName);
		setPosition(position);
		setDimensions(dimensions);
		m_labelText = labelText;
		m_labelAlignment = labelAlignment;
		m_horizontalLabelAlignment = horizontalLabelAlignment;
		m_verticalLabelAlignment = verticalLabelAlignment;
	}

	glm::vec3 Label::getPosition() const {
		glm::ivec2 wd = App::Window.getDimensions();
		return glm::vec3(m_background.position.x / wd.x,
			m_background.position.y / wd.y, m_background.position.z);
	}

	void Label::setPosition(const glm::vec3& position) {
		glm::ivec2 wd = App::Window.getDimensions();
		m_background.position = glm::vec3(position.x * wd.x, position.y * wd.y,
			position.z);
	}

	glm::vec2 Label::getDimensions() const {
		glm::ivec2 wd = App::Window.getDimensions();
		return glm::vec2(m_background.dimensions.x / wd.x,
			m_background.dimensions.y / wd.y);
	}

	void Label::setDimensions(const glm::vec2& dimensions) {
		glm::ivec2 wd = App::Window.getDimensions();
		m_background.dimensions = glm::vec2(dimensions.x * wd.x,
			dimensions.y * wd.y);
	}

	const std::string& Label::getLabelText() const {
		return m_labelText;
	}

	void Label::setLabelText(const std::string& labelText) {
		m_labelText = labelText;
	}

	Alignment Label::getLabelAlignment() const {
		return m_labelAlignment;
	}

	void Label::setLabelAlignment(Alignment labelAlignment) {
		m_labelAlignment = labelAlignment;
	}

	Alignment Label::getHorizontalLabelAlignment() const {
		return m_horizontalLabelAlignment;
	}

	void Label::setHorizontalLabelAlignment(
		Alignment horizontalLabelAlignment) {
		m_horizontalLabelAlignment = horizontalLabelAlignment;
	}

	Alignment Label::getVerticalLabelAlignment() const {
		return m_verticalLabelAlignment;
	}

	void Label::setVerticalLabelAlignment(Alignment verticalLabelAlignment) {
		m_verticalLabelAlignment = verticalLabelAlignment;
	}

	void Label::draw() {
		App::Graphics.submit(m_background, m_parent->getShader());
		glm::vec2 wd = App::Window.getDimensions();
		glm::vec3 textPosition = getPosition();
		textPosition.x *= wd.x;
		textPosition.y *= wd.y;
		textPosition.z += 0.1f;
		glm::vec2 textDimensions = getDimensions();
		textDimensions.x *= wd.x;
		textDimensions.y *= wd.y;
		switch (m_labelAlignment) {
		case AC_ALIGN_LEFT: {
			textPosition.x -= textDimensions.x;
			break;
		}
		case AC_ALIGN_RIGHT: {
			textPosition.x += textDimensions.x;
			break;
		}
		case AC_ALIGN_BOTTOM: {
			textPosition.y -= textDimensions.y;
			break;
		}
		case AC_ALIGN_TOP: {
			textPosition.y += textDimensions.y;
			break;
		}
		}
		glm::vec4 textBox(textPosition.x, textPosition.y, textDimensions.x,
			textDimensions.y);
		App::Graphics.submit(m_labelText, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), m_horizontalLabelAlignment,
			m_verticalLabelAlignment, m_parent->getShader());
	}

	void Label::processInput() {

	}

	void Label::update(float dt) {

	}

	void Label::destroy() {
		m_background.destroy();
		m_labelText = "";
		m_labelAlignment = AC_ALIGN_NONE;
		m_horizontalLabelAlignment = AC_ALIGN_NONE;
		m_verticalLabelAlignment = AC_ALIGN_NONE;
	}

	/*
	Determine whether the mouse is inside the rectangle formed by a component's
	2D position and dimensions.
	@param UIComponent& component: The component to test.
	@return Whether the point p intersects the sprite s.
	*/
	bool MouseOver(UIComponent& component) {
		glm::vec2 mp = App::Input.getMousePosition(
			component.getParent().getShader().getCamera());
		glm::ivec2 wd = App::Window.getDimensions();
		glm::vec3 cp = component.getPosition();
		cp.x *= wd.x;
		cp.y *= wd.y;
		glm::vec2 cd = component.getDimensions();
		cd.x *= wd.x;
		cd.y *= wd.y;
		if (mp.x < cp.x || mp.x > cp.x + cd.x) {
			return false;
		}
		if (mp.y < cp.y || mp.y > cp.y + cd.y) {
			return false;
		}
		return true;
	}

	// Implement Button class functions.

	// Texture coordinates for a button's unselected texture (the left-most
	// third of its texture).
	glm::vec4 Button::UnselectedTextureCoordinates = glm::vec4(
		0.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	// Texture coordinates for a button's selected texture (the middle third of
	// its texture).
	glm::vec4 Button::SelectedTextureCoordinates = glm::vec4(
		1.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	// Texture coordinates for a button's clicked texture (the right-most third
	// of its texture).
	glm::vec4 Button::ClickedTextureCoordinates = glm::vec4(
		2.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);

	void Button::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment) {
		Label::init(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setSelected(false);
		setClicked(false);
	}

	bool Button::isSelected() const {
		return m_selected;
	}

	void Button::setSelected(bool selected) {
		if (selected) {
			m_background.texture.coordinates = SelectedTextureCoordinates;
		}
		else {
			m_background.texture.coordinates = UnselectedTextureCoordinates;
		}
		m_selected = selected;
	}

	bool Button::isClicked() const {
		return m_clicked;
	}

	void Button::setClicked(bool clicked) {
		if (clicked) {
			m_background.texture.coordinates = ClickedTextureCoordinates;
		}
		else {
			if (m_selected) {
				m_background.texture.coordinates = SelectedTextureCoordinates;
			}
			else {
				m_background.texture.coordinates
					= UnselectedTextureCoordinates;
			}
		}
		m_clicked = clicked;
	}

	void Button::setEnabled(bool enabled) {
		Label::setEnabled(enabled);
		if (!m_enabled) {
			setClicked(false);
			setSelected(false);
		}
	}

	void Button::processInput() {
		if (MouseOver(*this)) {
			if (!m_selected) {
				setSelected(true);
				m_parent->componentEvent(m_ID, SelectedEvent);
			}
		}
		else {
			if (m_selected) {
				setSelected(false);
				m_parent->componentEvent(m_ID, UnselectedEvent);
			}
		}
		if (m_selected) {
			if (m_clicked) {
				if (App::Input.isMouseButtonReleased(AC_MB_LEFT)) {
					m_parent->componentEvent(m_ID, ClickedEvent);
				}
			}
			if (App::Input.isMouseButtonDown(AC_MB_LEFT)) {
				setClicked(true);
			}
			else {
				setClicked(false);
			}
		}
		else {
			setClicked(false);
		}
	}

	void Button::destroy() {
		Label::destroy();
		m_selected = false;
		m_clicked = false;
	}

	// Implement Switch class functions.

	// Texture coordinates for a switch's turned off texture (the left half of
	// its texture).
	glm::vec4 Switch::OffTextureCoordinates = glm::vec4(
		0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	// Texture coordinates for a switch's turned on texture (the right half of
	// its texture).
	glm::vec4 Switch::OnTextureCoordinates = glm::vec4(
		1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	void Switch::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment) {
		Label::init(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setOn(false);
	}

	bool Switch::isOn() const {
		return m_on;
	}

	void Switch::setOn(bool on) {
		if (on) {
			m_background.texture.coordinates = OnTextureCoordinates;
		}
		else {
			m_background.texture.coordinates = OffTextureCoordinates;
		}
		m_on = on;
	}

	void Switch::processInput() {
		if (MouseOver(*this)) {
			if (App::Input.isMouseButtonPressed(AC_MB_LEFT)) {
				if (m_on) {
					setOn(false);
					m_parent->componentEvent(m_ID, OffEvent);
				}
				else {
					setOn(true);
					m_parent->componentEvent(m_ID, OnEvent);
				}
			}
		}
	}

	void Switch::destroy() {
		Label::destroy();
		m_on = false;
	}

	// Implement Slider class functions.

	// Texture coordinates for the slider's unselected texture (the left half
	// of its texture).
	glm::vec4 Slider::UnselectedTextureCoordinates = glm::vec4(0.0f, 0.0f,
		1.0f / 2.0f, 1.0f);
	// Texture coordinates for the slider's selected texture (the right half
	// of its texture).
	glm::vec4 Slider::SelectedTextureCoordinates = glm::vec4(1.0f / 2.0f, 0.0f,
		1.0f / 2.0f, 1.0f);
	
	void Slider::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName,
		const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& labelText,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment, Alignment cursorAlignment) {
		Label::init(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		m_cursorAlignment = cursorAlignment;
		glm::ivec2 wd = App::Window.getDimensions();
		m_cursor.init(glm::vec3(),
			glm::vec2(cursorDimensions.x, cursorDimensions.y * (float)wd.y),
			cursorTextureFileName);
		m_value = 0.0f;
		setSelected(false);
	}

	Alignment Slider::getCursorAlignment() const {
		return m_cursorAlignment;
	}

	void Slider::setCursorAlignment(Alignment cursorAlignment) {
		m_cursorAlignment = cursorAlignment;
	}

	float Slider::getValue() const {
		return m_value;
	}

	void Slider::setValue(float value) {
		m_value = value;
	}

	bool Slider::isSelected() const {
		return m_selected;
	}

	void Slider::setSelected(bool selected) {
		if (selected) {
			m_background.texture.coordinates = SelectedTextureCoordinates;
		}
		else {
			m_background.texture.coordinates = UnselectedTextureCoordinates;
		}
		m_selected = selected;
	}

	void Slider::setEnabled(bool enabled) {
		Label::setEnabled(enabled);
		if (!m_enabled) {
			setSelected(false);
		}
	}

	void Slider::setDimensions(const glm::vec2& dimensions) {
		glm::ivec2 wd = App::Window.getDimensions();
		float scale = dimensions.y / (m_background.dimensions.y / (float)wd.y);
		m_background.dimensions = glm::vec2(dimensions.x * wd.x,
			dimensions.y * wd.y);
		m_cursor.dimensions = glm::vec2(m_cursor.dimensions.x,
			m_cursor.dimensions.y * scale);
	}

	void Slider::draw() {
		Label::draw();
		App::Graphics.submit(m_cursor, m_parent->getShader());
	}

	void Slider::processInput() {
		if (MouseOver(*this)) {
			if (App::Input.isMouseButtonPressed(AC_MB_LEFT)) {
				setSelected(true);
			}
		}
		if (!App::Input.isMouseButtonDown(AC_MB_LEFT) && m_selected) {
			m_parent->componentEvent(m_ID, ValueSetEvent);
			setSelected(false);
		}
		if (m_selected) {
			glm::vec2 mp = App::Input.getMousePosition(
				m_parent->getShader().getCamera());
			if (mp.x >= m_background.position.x + m_background.dimensions.x
				- m_cursor.dimensions.x) {
				m_value = 1.0f;
			}
			else if (mp.x < m_background.position.x) {
				m_value = 0.0f;
			}
			else {
				m_value = (mp.x - m_background.position.x)
					/ (m_background.dimensions.x - m_cursor.dimensions.x);
			}
		}
	}

	void Slider::update(float dt) {
		float cy = 0.0f;
		switch (m_cursorAlignment) {
		case AC_ALIGN_BOTTOM: {
			cy = m_background.position.y;
			break;
		}
		case AC_ALIGN_CENTER: {
			cy = m_background.position.y + (m_background.dimensions.y
				- m_cursor.dimensions.y) / 2.0f;
			break;
		}
		case AC_ALIGN_TOP: {
			cy = m_background.position.y + m_background.dimensions.y
				- m_cursor.dimensions.y;
			break;
		}
		default: {
			cy = m_background.position.y + (m_background.dimensions.y
				- m_cursor.dimensions.y) / 2.0f;
			break;
		}
		}
		m_cursor.position = glm::vec3(m_background.position.x
			+ ((m_background.dimensions.x - m_cursor.dimensions.x) * m_value),
			cy, m_background.position.z + 0.1f);
	}

	void Slider::destroy() {
		Label::destroy();
		m_cursorAlignment = AC_ALIGN_NONE;
		m_cursor.destroy();
		m_value = 0.0f;
		m_selected = false;
	}

	// Implement TextBox class functions.

	// Texture coordinates for the text box's unselected texture (the left half
	// of its texture).
	glm::vec4 TextBox::UnselectedTextureCoordinates = glm::vec4(
		0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	// Texture coordinates for the text box's selected texture (the right half
	// of its texture).
	glm::vec4 TextBox::SelectedTextureCoordinates = glm::vec4(
		1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	void TextBox::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& text,
		int maxCharacters, const std::string& allowedCharacters,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment, Alignment cursorAlignment,
		Alignment verticalAlignment) {
		Label::init(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setSelected(false);
		m_text = text;
		m_textHorizontalPosition = 0.0f;
		m_cursorAlignment = cursorAlignment;
		glm::ivec2 wd = App::Window.getDimensions();
		m_cursor.init(glm::vec3(),
			glm::vec2(cursorDimensions.x, cursorDimensions.y * wd.y),
			cursorTextureFileName);
		m_cursorPosition = 0;
		m_timer = 0.0f;
		m_allowedCharacters = allowedCharacters;
		m_maxCharacters = maxCharacters;
		m_verticalAlignment = verticalAlignment;
	}

	bool TextBox::isSelected() const {
		return m_selected;
	}

	void TextBox::setSelected(bool selected) {
		if (selected) {
			m_background.texture.coordinates = SelectedTextureCoordinates;
		}
		else {
			m_background.texture.coordinates = UnselectedTextureCoordinates;
		}
		m_selected = selected;
	}

	void TextBox::setPosition(const glm::vec3& position) {
		Label::setPosition(position);
		m_textHorizontalPosition = m_background.position.x;
	}

	void TextBox::setDimensions(const glm::vec2& dimensions) {
		glm::ivec2 wd = App::Window.getDimensions();
		float scale = dimensions.y / (m_background.dimensions.y / (float)wd.y);
		m_background.dimensions = glm::vec2(dimensions.x * wd.x,
			dimensions.y * wd.y);
		m_cursor.dimensions = glm::vec2(m_cursor.dimensions.x,
			m_cursor.dimensions.y * scale);
	}

	const std::string& TextBox::getText() const {
		return m_text;
	}

	void TextBox::setText(const std::string& text) {
		if (text.length() > m_maxCharacters && m_maxCharacters > 0) {
			return;
		}
		m_textHorizontalPosition = m_background.position.x;
		m_cursorPosition = 0;
		m_text = text;
	}

	Alignment TextBox::getCursorAlignment() const {
		return m_cursorAlignment;
	}

	void TextBox::setCursorAlignment(Alignment cursorAlignment) {
		m_cursorAlignment = cursorAlignment;
	}

	unsigned int TextBox::getCursorPosition() const {
		return m_cursorPosition;
	}

	bool TextBox::setCursorPosition(unsigned int cursorPosition) {
		if (cursorPosition > m_text.length()) {
			return false;
		}
		m_cursorPosition = cursorPosition;
		return true;
	}

	int TextBox::getMaxCharacters() const {
		return m_maxCharacters;
	}

	void TextBox::setMaxCharacters(int maxCharacters) {
		if (maxCharacters < 0) {
			m_maxCharacters = -1;
		}
		else {
			m_maxCharacters = maxCharacters;
		}
	}

	const std::string& TextBox::getAllowedCharacters() const {
		return m_allowedCharacters;
	}

	void TextBox::setAllowedCharacters(const std::string& allowedCharacters) {
		m_allowedCharacters = allowedCharacters;
	}

	Alignment TextBox::getVerticalAlignment() const {
		return m_verticalAlignment;
	}

	void TextBox::setVerticalAlignment(Alignment verticalAlignment) {
		m_verticalAlignment = verticalAlignment;
	}

	void TextBox::charTyped(char c) {
		if (!m_selected) {
			return;
		}
		if (m_text.length() >= m_maxCharacters && m_maxCharacters > 0) {
			m_text.resize(m_maxCharacters);
			return;
		}
		bool f = false;
		for (char x : m_allowedCharacters) {
			if (x == c) {
				f = true;
				break;
			}
		}
		if (!(m_allowedCharacters.empty() || f)) {
			return;
		}
		std::string nt = "";
		bool s = false;
		for (unsigned int cp = 0; cp < m_text.length(); cp++) {
			if (cp == m_cursorPosition) {
				nt += c;
				s = true;
			}
			nt += m_text.at(cp);
		}
		if (s) {
			m_text = nt;
		}
		else {
			m_text += c;
		}
		m_cursorPosition++;
	}

	void TextBox::setEnabled(bool enabled) {
		Label::setEnabled(enabled);
		if (!m_enabled) {
			setSelected(false);
		}
	}

	void TextBox::draw() {
		Label::draw();
		glm::vec3 textPosition(m_textHorizontalPosition,
			m_background.position.y, m_background.position.z + 0.1f);
		glm::vec4 textBox(m_background.position.x, m_background.position.y,
			m_background.dimensions.x, m_background.dimensions.y);
		App::Graphics.submit(m_text, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), AC_ALIGN_NONE, m_verticalAlignment,
			m_parent->getShader());
		if (m_selected) {
			App::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void TextBox::processInput() {
		if (App::Input.isMouseButtonDown(AC_MB_LEFT)) {
			if (MouseOver(*this)) {
				if (!m_selected) {
					setSelected(true);
					m_parent->componentEvent(m_ID, SelectedEvent);
				}
				float x = m_textHorizontalPosition;
				unsigned int cp = 0;
				bool s = false;
				const Font* font = App::Graphics.getFont(
					m_parent->getFontFileName());
				glm::vec2 mp = App::Input.getMousePosition(
					m_parent->getShader().getCamera());
				for (unsigned int i = 0; i < m_text.length(); i++) {
					float d = font->glyphs.at(m_text.at(i)).offset
						* m_parent->getTextScale();
					if (x + d / 2.0f >= mp.x) {
						s = true;
						cp = i;
						break;
					}
					x += d;
				}
				if (s) {
					m_cursorPosition = cp;
				}
				else {
					m_cursorPosition = (unsigned int)m_text.length();
				}
			}
			else {
				if (m_selected
					&& App::Input.isMouseButtonPressed(AC_MB_LEFT)) {
					setSelected(false);
					m_parent->componentEvent(m_ID, UnselectedEvent);
				}
			}
		}
		if (m_selected) {
			if (App::Input.isKeyPressed(AC_CTL_ENTER)) {
				m_parent->componentEvent(m_ID, TextEnteredEvent);
			}
			if (m_cursorPosition > 0 && (App::Input.isKeyPressed(AC_CTL_LEFT)
				|| (App::Input.isKeyDown(AC_CTL_LEFT) && m_timer > 10.0f))) {
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (App::Input.isKeyPressed(AC_CTL_RIGHT)
					|| (App::Input.isKeyDown(AC_CTL_RIGHT)
						&& m_timer > 10.0f))) {
				m_cursorPosition++;
				m_timer = 0.0f;
			}
			if (m_cursorPosition > 0
				&& (App::Input.isKeyPressed(AC_CTL_BACKSPACE)
					|| (App::Input.isKeyDown(AC_CTL_BACKSPACE)
						&& m_timer > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp + 1) {
						nt += m_text.at(cp);
					}
				}
				m_text = nt;
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (App::Input.isKeyPressed(AC_CTL_DELETE)
					|| (App::Input.isKeyDown(AC_CTL_DELETE)
						&& m_timer > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp) {
						nt += m_text.at(cp);
					}
				}
				m_text = nt;
				m_timer = 0.0f;
			}
		}
	}

	void TextBox::update(float dt) {
		m_cursor.position = glm::vec3(m_textHorizontalPosition, 0.0f,
			m_background.position.z + 0.2f);
		const Font* font = App::Graphics.getFont(m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			m_cursor.position.x += (font->glyphs.at(m_text.at(cp)).offset
				- (cp == 0 ? font->glyphs.at(m_text.at(cp)).bearing.x : 0))
				* m_parent->getTextScale();
		}
		switch (m_cursorAlignment) {
		case AC_ALIGN_BOTTOM: {
			m_cursor.position.y = m_background.position.y;
			break;
		}
		case AC_ALIGN_CENTER: {
			m_cursor.position.y = m_background.position.y
				+ (m_background.dimensions.y - m_cursor.dimensions.y) / 2.0f;
			break;
		}
		case AC_ALIGN_TOP: {
			m_cursor.position.y = m_background.position.y
				+ m_background.dimensions.y - m_cursor.dimensions.y;
			break;
		}
		default: {
			m_cursor.position.y = m_background.position.y
				+ (m_background.dimensions.y - m_cursor.dimensions.y) / 2.0f;
			break;
		}
		}
		if (m_cursor.position.x < m_background.position.x) {
			m_textHorizontalPosition += m_background.dimensions.x / 5.0f;
			if (m_textHorizontalPosition > m_background.position.x) {
				m_textHorizontalPosition = m_background.position.x;
			}
		}
		else if (m_cursor.position.x + m_cursor.dimensions.x
				> m_background.position.x + m_background.dimensions.x) {
			m_textHorizontalPosition -= m_background.dimensions.x / 5.0f;
		}
		m_timer += dt;
		if (m_timer > 60.0f) {
			m_timer = 0.0f;
		}
	}

	void TextBox::destroy() {
		Label::destroy();
		m_selected = false;
		m_text = "";
		m_textHorizontalPosition = 0.0f;
		m_cursorAlignment = AC_ALIGN_NONE;
		m_cursor.destroy();
		m_cursorPosition = 0;
		m_timer = 0.0f;
		m_allowedCharacters = "";
		m_maxCharacters = -1;
		m_verticalAlignment = AC_ALIGN_NONE;
	}

	// Implement Carousel class functions.

	void Carousel::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		const std::vector<std::string>& values, const std::string& value,
		const std::string& buttonTextureFileName, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment horizontalAlignment, Alignment verticalAlignment,
		Alignment buttonAlignment) {
		Label::init(glm::vec3(), glm::vec2(), textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		for (const std::string& val : values) {
			addValue(val);
		}
		m_valueIndex = getValueIndex(value);
		if (m_valueIndex == -1) {
			m_valueIndex = 0;
		}
		m_horizontalAlignment = horizontalAlignment;
		m_verticalAlignment = verticalAlignment;
		m_buttonAlignment = buttonAlignment;
		m_prevButton.init(glm::vec3(), glm::vec2(), buttonTextureFileName, "",
			AC_ALIGN_NONE, AC_ALIGN_CENTER, AC_ALIGN_CENTER);
		m_prevButton.m_ID = 0;
		m_prevButton.m_parent = this;
		m_nextButton.init(glm::vec3(), glm::vec2(), buttonTextureFileName, "",
			AC_ALIGN_NONE, AC_ALIGN_CENTER, AC_ALIGN_CENTER);
		m_nextButton.m_ID = 1;
		m_nextButton.m_parent = this;
		setDimensions(dimensions);
		setPosition(position);
		setButtonAlignment(buttonAlignment);
	}

	const std::vector<std::string>& Carousel::getValues() const {
		return m_values;
	}

	const std::string& Carousel::getValue() const {
		return m_values.at(m_valueIndex);
	}

	void Carousel::setValue(const std::string& value) {
		int index = getValueIndex(value);
		if (index == -1) {
			addValue(value);
			index = getValueIndex(value);
		}
		setValueIndex(index);
	}

	unsigned int Carousel::getValueIndex() const {
		return m_valueIndex;
	}

	int Carousel::getValueIndex(const std::string& value) const {
		for (unsigned int i = 0; i < m_values.size(); i++) {
			if (m_values.at(i) == value) {
				return i;
			}
		}
		return -1;
	}

	void Carousel::setValueIndex(unsigned int index) {
		if (index >= m_values.size()) {
			m_valueIndex = 0;
		}
		m_valueIndex = index;
	}

	bool Carousel::addValue(const std::string& value, int index) {
		if (index < -1 || index > (int)m_values.size()) {
			return false;
		}
		if (index == -1) {
			m_values.push_back(value);
			return true;
		}
		std::vector<std::string>::iterator it = m_values.begin() + index;
		m_values.insert(it, value);
		return true;
	}

	bool Carousel::removeValue(const std::string& value) {
		std::vector<std::string>::iterator it = std::find(m_values.begin(),
			m_values.end(), value);
		if (it == m_values.end()) {
			return false;
		}
		m_values.erase(it);
		setValueIndex(m_valueIndex);
		return true;
	}

	bool Carousel::removeValue(unsigned int index) {
		if (index >= m_values.size()) {
			return false;
		}
		std::vector<std::string>::iterator it = m_values.begin() + index;
		m_values.erase(it);
		setValueIndex(m_valueIndex);
		return true;
	}

	Alignment Carousel::getHorizontalAlignment() const {
		return m_horizontalAlignment;
	}

	void Carousel::setHorizontalAlignment(Alignment horizontalAlignment) {
		m_horizontalAlignment = horizontalAlignment;
	}

	Alignment Carousel::getVerticalAlignment() const {
		return m_verticalAlignment;
	}

	void Carousel::setVerticalAlignment(Alignment verticalAlignment) {
		m_verticalAlignment = verticalAlignment;
	}

	Alignment Carousel::getButtonAlignment() const {
		return m_buttonAlignment;
	}

	void Carousel::setButtonAlignment(Alignment buttonAlignment) {
		glm::vec2 prevDimensions = getDimensions();
		glm::vec3 prevPosition = getPosition();
		switch (buttonAlignment) {
		case AC_ALIGN_LEFT: {
			m_prevButton.m_background.reflect.horizontal = false;
			m_prevButton.m_background.reflect.vertical = false;
			m_nextButton.m_background.reflect.horizontal = false;
			m_nextButton.m_background.reflect.vertical = true;
			break;
		}
		case AC_ALIGN_CENTER: {
			m_prevButton.m_background.reflect.horizontal = false;
			m_prevButton.m_background.reflect.vertical = false;
			m_nextButton.m_background.reflect.horizontal = true;
			m_nextButton.m_background.reflect.vertical = false;
			break;
		}
		case AC_ALIGN_RIGHT: {
			m_prevButton.m_background.reflect.horizontal = false;
			m_prevButton.m_background.reflect.vertical = false;
			m_nextButton.m_background.reflect.horizontal = false;
			m_nextButton.m_background.reflect.vertical = true;
			break;
		}
		default: {
			return;
		}
		}
		m_buttonAlignment = buttonAlignment;
		setDimensions(prevDimensions);
		setPosition(prevPosition);
	}

	glm::vec3 Carousel::getPosition() const {
		switch (m_buttonAlignment) {
		case AC_ALIGN_LEFT: {
			return m_nextButton.getPosition();
			break;
		}
		case AC_ALIGN_RIGHT: {
			return Label::getPosition();
			break;
		}
		case AC_ALIGN_CENTER: {
			return m_prevButton.getPosition();
			break;
		}
		}
		return Label::getPosition();
	}

	void Carousel::setPosition(const glm::vec3& position) {
		switch (m_buttonAlignment) {
		case AC_ALIGN_LEFT: {
			m_nextButton.setPosition(position);
			m_prevButton.setPosition(glm::vec3(position.x,
				position.y + m_nextButton.getDimensions().y, position.z));
			Label::setPosition(glm::vec3(
				position.x + m_nextButton.getDimensions().x, position.y,
				position.z));
			break;
		}
		case AC_ALIGN_RIGHT: {
			Label::setPosition(position);
			m_nextButton.setPosition(glm::vec3(
				position.x + Label::getDimensions().x, position.y,
				position.z));
			m_prevButton.setPosition(glm::vec3(
				position.x + Label::getDimensions().x,
				position.y + m_nextButton.getDimensions().y,
				position.z));
			break;
		}
		case AC_ALIGN_CENTER: {
			m_prevButton.setPosition(position);
			Label::setPosition(glm::vec3(
				position.x + m_prevButton.getDimensions().x, position.y,
				position.z));
			m_nextButton.setPosition(glm::vec3(
				position.x + m_prevButton.getDimensions().x
				+ Label::getDimensions().x, position.y, position.z));
			break;
		}
		}
	}

	glm::vec2 Carousel::getDimensions() const {
		glm::vec2 dimensions(0.0f, Label::getDimensions().y);
		switch (m_buttonAlignment) {
		case AC_ALIGN_LEFT: {
			dimensions.x = m_nextButton.getDimensions().x
				+ Label::getDimensions().x;
			break;
		}
		case AC_ALIGN_RIGHT: {
			dimensions.x = m_nextButton.getDimensions().x
				+ Label::getDimensions().x;
			break;
		}
		case AC_ALIGN_CENTER: {
			dimensions.x = m_nextButton.getDimensions().x
				+ Label::getDimensions().x
				+ m_prevButton.getDimensions().x;
			break;
		}
		}
		return dimensions;
	}

	void Carousel::setDimensions(const glm::vec2& dimensions) {
		switch (m_buttonAlignment) {
		case AC_ALIGN_LEFT: {
			Label::setDimensions(glm::vec2((4.0f * dimensions.x) / 5.0f,
				dimensions.y));
			m_prevButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y / 2.0f));
			m_nextButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y / 2.0f));
			break;
		}
		case AC_ALIGN_RIGHT: {
			Label::setDimensions(glm::vec2((4.0f * dimensions.x) / 5.0f,
				dimensions.y));
			m_prevButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y / 2.0f));
			m_nextButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y / 2.0f));
			break;
		}
		case AC_ALIGN_CENTER: {
			Label::setDimensions(glm::vec2((3.0f * dimensions.x) / 5.0f,
				dimensions.y));
			m_prevButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y));
			m_nextButton.setDimensions(glm::vec2(dimensions.x / 5.0f,
				dimensions.y));
			break;
		}
		}
	}

	void Carousel::componentEvent(int cID, int eID) {
		if (cID == m_prevButton.getID()) {
			if (eID == Button::ClickedEvent) {
				if (m_valueIndex >= 1) {
					setValueIndex((unsigned int)m_valueIndex - 1);
				}
				else {
					setValueIndex((unsigned int)m_values.size() - 1);
				}
				m_parent->componentEvent(m_ID, ValueSetEvent);
			}
		}
		else if (cID == m_nextButton.getID()) {
			if (eID == Button::ClickedEvent) {
				setValueIndex((m_valueIndex + 1) % m_values.size());
				m_parent->componentEvent(m_ID, ValueSetEvent);
			}
		}
	}

	Shader& Carousel::getShader() {
		return m_parent->getShader();
	}

	const std::string& Carousel::getFontFileName() const {
		return m_parent->getFontFileName();
	}

	float Carousel::getTextScale() const {
		return m_parent->getTextScale();
	}

	const glm::vec4& Carousel::getTextColor() const {
		return m_parent->getTextColor();
	}

	void Carousel::setEnabled(bool enabled) {
		Label::setEnabled(enabled);
		m_prevButton.setEnabled(enabled);
		m_nextButton.setEnabled(enabled);
	}

	void Carousel::setShader(Shader& shader) {

	}

	void Carousel::setFontFileName(const std::string& fontFileName) {

	}

	void Carousel::setTextScale(float textScale) {

	}

	void Carousel::setTextColor(const glm::vec4& textColor) {
	
	}

	void Carousel::draw() {
		Label::draw();
		if (m_values.size() > 0) {
			App::Graphics.submit(m_values.at(m_valueIndex), glm::vec3(
				m_background.position.x, m_background.position.y,
				m_background.position.z + 0.1f), glm::vec4(
					m_background.position.x, m_background.position.y,
					m_background.dimensions.x, m_background.dimensions.y),
				m_parent->getTextScale(), m_parent->getTextColor(),
				m_parent->getFontFileName(), m_horizontalAlignment,
				m_verticalAlignment, m_parent->getShader());
		}
		m_prevButton.draw();
		m_nextButton.draw();
	}

	void Carousel::processInput() {
		m_prevButton.processInput();
		m_nextButton.processInput();
	}

	void Carousel::update(float dt) {
		m_prevButton.update(dt);
		m_nextButton.update(dt);
	}

	void Carousel::destroy() {
		Label::destroy();
		m_values.clear();
		m_valueIndex = 0;
		m_prevButton.destroy();
		m_nextButton.destroy();
		m_horizontalAlignment = AC_ALIGN_NONE;
		m_verticalAlignment = AC_ALIGN_NONE;
		m_buttonAlignment = AC_ALIGN_NONE;
	}

	// Implement TextArea class functions.

	void TextArea::init(const glm::vec3& position, const glm::vec2& dimensions,
		unsigned int lineCount, const std::string& textureFileName,
		const std::string& labelText, const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& text,
		int maxCharacters, const std::string& allowedCharacters,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment, Alignment cursorAlignment) {
		TextBox::init(position, dimensions, textureFileName, labelText,
			cursorDimensions, cursorTextureFileName, text, maxCharacters,
			allowedCharacters, labelAlignment, horizontalLabelAlignment,
			verticalLabelAlignment, cursorAlignment, AC_ALIGN_CENTER);
		m_lineCount = lineCount;
		m_topLine = 0;
		m_textHorizontalPosition = m_background.position.x;
	}

	void TextArea::setPosition(const glm::vec3& position) {
		TextBox::setPosition(position);
		m_topLine = 0;
	}

	void TextArea::setText(const std::string& text) {
		TextBox::setText(text);
		m_topLine = 0;
	}

	int TextArea::getLineCount() const {
		return m_lineCount;
	}

	int TextArea::getTopLine() const {
		return m_topLine;
	}

	bool TextArea::setTopLine(int topLine) {
		if (topLine < 0) {
			return false;
		}
		int contentLineCount = (int)getContentLines().size();
		if (contentLineCount <= m_lineCount && topLine > 0) {
			return false;
		}
		if (topLine > contentLineCount - m_lineCount) {
			return false;
		}
		m_topLine = topLine;
		return true;
	}

	void TextArea::charTyped(char c) {
		TextBox::charTyped(c);
		putCursorInView();
	}

	void TextArea::draw() {
		Label::draw();
		std::vector<std::string> contentLines = getContentLines();
		float lineHeight = m_background.dimensions.y / m_lineCount;
		for (size_t i = 0; i < m_lineCount; i++) {
			glm::vec3 linePos(m_textHorizontalPosition,
				(m_background.position.y + m_background.dimensions.y)
					- (lineHeight * (i + 1)),
				m_background.position.z + 0.1f);
			glm::vec4 lineBox(m_background.position.x, linePos.y,
				m_background.dimensions.x, lineHeight);
			if (i + m_topLine >= contentLines.size()) {
				break;
			}
			App::Graphics.submit(contentLines.at(i + m_topLine), linePos,
				lineBox, m_parent->getTextScale(), m_parent->getTextColor(),
				m_parent->getFontFileName(), AC_ALIGN_NONE, AC_ALIGN_CENTER,
				m_parent->getShader());
		}
		if (m_selected && m_cursor.intersects(m_background)) {
			App::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void TextArea::processInput() {
		if (App::Input.isMouseButtonDown(AC_MB_LEFT)) {
			if (MouseOver(*this)) {
				if (!m_selected) {
					setSelected(true);
					m_parent->componentEvent(m_ID, SelectedEvent);
				}
				float x = m_textHorizontalPosition;
				float y = (m_background.position.y + m_background.dimensions.y)
					- (m_background.dimensions.y / m_lineCount);
				const Font* font = App::Graphics.getFont(
					m_parent->getFontFileName());
				glm::vec2 mp = App::Input.getMousePosition(
					m_parent->getShader().getCamera());
				m_cursorPosition = 0;
				std::vector<std::string> lines = getContentLines();
				if (!lines.empty()) {
					unsigned int line = 0;
					for (unsigned int l = 0; l < lines.size(); l++) {
						float yOffset = (m_background.dimensions.y
							/ m_lineCount) * m_topLine;
						if (y <= mp.y - yOffset) {
							line = l;
							break;
						}
						y -= (m_background.dimensions.y / m_lineCount);
						m_cursorPosition += (unsigned int)lines.at(l).length()
							+ 1;
					}
					float xd = 0.0f;
					for (unsigned int i = 0; i < lines.at(line).length(); i++) {
						xd = font->glyphs.at(lines.at(line).at(i)).offset
							* m_parent->getTextScale();
						if (x + xd / 2.0f > mp.x) {
							break;
						}
						m_cursorPosition++;
						x += xd;
					}
					if (m_cursorPosition > m_text.length()) {
						m_cursorPosition = (unsigned int)m_text.length();
					}
				}
			}
			else {
				if (m_selected && App::Input.isMouseButtonPressed(AC_MB_LEFT)) {
					setSelected(false);
					m_parent->componentEvent(m_ID, UnselectedEvent);
				}
			}
		}
		if (m_selected) {
			if (App::Input.isKeyPressed(AC_CTL_ENTER)) {
				charTyped('\n');
			}
			if (m_cursorPosition > 0 && (App::Input.isKeyPressed(AC_CTL_LEFT)
				|| (App::Input.isKeyDown(AC_CTL_LEFT) && m_timer > 10.0f))) {
				m_cursorPosition--;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (App::Input.isKeyPressed(AC_CTL_RIGHT)
					|| (App::Input.isKeyDown(AC_CTL_RIGHT)
						&& m_timer > 10.0f))) {
				m_cursorPosition++;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (App::Input.isKeyPressed(AC_CTL_UP)
				|| (App::Input.isKeyDown(AC_CTL_UP) && m_timer > 10.0f)) {
				std::vector<std::string> lines = getContentLines();
				unsigned int cLine = 0;
				for (unsigned int i = 0; i < m_cursorPosition; i++) {
					if (m_text.at(i) == '\n') {
						cLine++;
					}
				}
				if (cLine > 0) {
					unsigned int ncp = 0;
					for (unsigned int i = 0; i < cLine - 1; i++) {
						ncp += (unsigned int)lines.at(i).length() + 1;
					}
					float x = m_textHorizontalPosition;
					const Font* font = App::Graphics.getFont(
						m_parent->getFontFileName());
					for (unsigned int i = 0; i < lines.at(cLine - 1).length();
						i++) {
						x += font->glyphs.at(lines.at(cLine - 1).at(i)).offset
							* m_parent->getTextScale();
						ncp++;
						if (x + font->glyphs.at(
							lines.at(cLine - 1).at(i)).offset
								* m_parent->getTextScale()
							> m_cursor.position.x) {
							break;
						}
					}
					m_cursorPosition = ncp;
				}
				putCursorInView();
				m_timer = 0.0f;
			}
			if (App::Input.isKeyPressed(AC_CTL_DOWN)
				|| (App::Input.isKeyDown(AC_CTL_DOWN) && m_timer > 10.0f)) {
				std::vector<std::string> lines = getContentLines();
				unsigned int cLine = 0;
				for (unsigned int i = 0; i < m_cursorPosition; i++) {
					if (m_text.at(i) == '\n') {
						cLine++;
					}
				}
				if (cLine < lines.size() - 1) {
					unsigned int ncp = 0;
					for (unsigned int i = 0; i < cLine + 1; i++) {
						ncp += (unsigned int)lines.at(i).length() + 1;
					}
					float x = m_textHorizontalPosition;
					const Font* font = App::Graphics.getFont(
						m_parent->getFontFileName());
					for (unsigned int i = 0; i < lines.at(cLine + 1).length();
						i++) {
						x += font->glyphs.at(lines.at(cLine + 1).at(i)).offset
							* m_parent->getTextScale();
						ncp++;
						if (x + font->glyphs.at(
							lines.at(cLine + 1).at(i)).offset
								* m_parent->getTextScale()
							> m_cursor.position.x) {
							break;
						}
					}
					m_cursorPosition = ncp;
				}
				putCursorInView();
				m_timer = 0.0f;
			}
			if (m_cursorPosition > 0
				&& (App::Input.isKeyPressed(AC_CTL_BACKSPACE)
					|| (App::Input.isKeyDown(AC_CTL_BACKSPACE)
						&& m_timer > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp + 1) {
						nt += m_text.at(cp);
					}
				}
				m_text = nt;
				m_cursorPosition--;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (App::Input.isKeyPressed(AC_CTL_DELETE)
					|| (App::Input.isKeyDown(AC_CTL_DELETE)
						&& m_timer > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp) {
						nt += m_text.at(cp);
					}
				}
				m_text = nt;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (App::Input.isKeyPressed(AC_KEY_V)
				&& App::Input.isKeyDown(AC_CTL_L_CTRL)) {
				std::string paste = glfwGetClipboardString(
					App::Window.getHandle());
				for (size_t i = 0; i < paste.length(); i++) {
					charTyped(paste.at(i));
				}
			}
		}
		if (MouseOver(*this)) {
			float scroll = App::Input.getMouseScroll().y;
			if (scroll != 0.0f && m_timer > 10.0f) {
				if (scroll < 0.0f) {
					setTopLine(getTopLine() + 1);
				}
				else {
					setTopLine(getTopLine() - 1);
				}
				m_timer = 0.0f;
			}
		}
	}

	void TextArea::update(float dt) {
		m_cursor.position = glm::vec3(m_textHorizontalPosition,
			m_background.position.y + m_background.dimensions.y
				+ ((m_topLine - 1) * (m_background.dimensions.y / m_lineCount)),
			m_background.position.z + 0.2f);
		const Font* font = App::Graphics.getFont(m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			if (m_text.at(cp) == '\n') {
				m_cursor.position = glm::vec3(m_textHorizontalPosition,
					m_cursor.position.y
						- (m_background.dimensions.y / m_lineCount),
					m_cursor.position.z);
			}
			else {
				m_cursor.position.x += (font->glyphs.at(m_text.at(cp)).offset
					- (cp == 0 ? font->glyphs.at(m_text.at(cp)).bearing.x : 0))
					* m_parent->getTextScale();
			}
		}
		switch (m_cursorAlignment) {
		case AC_ALIGN_CENTER: {
			m_cursor.position.y = m_cursor.position.y
				+ ((m_background.dimensions.y / m_lineCount)
				- m_cursor.dimensions.y) / 2.0f;
			break;
		}
		case AC_ALIGN_TOP: {
			m_cursor.position.y = m_cursor.position.y
				+ (m_background.dimensions.y / m_lineCount)
				- m_cursor.dimensions.y;
			break;
		}
		default: {
			break;
		}
		}
		m_timer += dt;
		if (m_timer > 60.0f) {
			m_timer = 0.0f;
		}
	}

	void TextArea::destroy() {
		TextBox::destroy();
		m_lineCount = 0;
		m_topLine = 0;
	}

	void TextArea::putCursorInView() {
		update(0.0f);
		bool goodX = false;
		while (!goodX) {
			if (m_cursor.position.x < m_background.position.x) {
				m_textHorizontalPosition += m_background.dimensions.x / 5.0f;
				m_cursor.position.x += m_background.dimensions.x / 5.0f;
				if (m_textHorizontalPosition > m_background.position.x) {
					m_textHorizontalPosition = m_background.position.x;
					goodX = true;
				}
			}
			else if (m_cursor.position.x + m_cursor.dimensions.x
				> m_background.position.x + m_background.dimensions.x) {
				m_textHorizontalPosition -= m_background.dimensions.x / 5.0f;
				m_cursor.position.x -= m_background.dimensions.x / 5.0f;
			}
			else {
				goodX = true;
			}
		}
		bool goodY = false;
		while (!goodY) {
			if (m_cursor.position.y < m_background.position.y) {
				setTopLine(m_topLine + 1);
				m_cursor.position.y
					+= (m_background.dimensions.y / m_lineCount);
			}
			else if (m_cursor.position.y + m_cursor.dimensions.y
				> m_background.position.y + m_background.dimensions.y) {
				setTopLine(m_topLine - 1);
				m_cursor.position.y
					-= (m_background.dimensions.y / m_lineCount);
			}
			else {
				goodY = true;
			}
		}
	}

	std::vector<std::string> TextArea::getContentLines() const {
		if (m_text.empty()) {
			return std::vector<std::string>();
		}
		std::vector<std::string> contentLines;
		std::string contentLine = "";
		for (size_t i = 0; i < m_text.length(); i++) {
			if (m_text.at(i) == '\n') {
				contentLines.push_back(contentLine);
				contentLine = "";
			}
			else {
				contentLine += m_text.at(i);
			}
		}
		if (!contentLine.empty()) {
			contentLines.push_back(contentLine);
		}
		return contentLines;
	}

	// Implement PasswordBox class functions.

	void PasswordBox::init(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& text,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment, Alignment cursorAlignment,
		Alignment verticalAlignment) {
		TextBox::init(position, dimensions, textureFileName, labelText,
			cursorDimensions, cursorTextureFileName, text, -1, "",
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment,
			cursorAlignment, verticalAlignment);
	}

	void PasswordBox::draw() {
		Label::draw();
		glm::vec3 textPosition(m_textHorizontalPosition,
			m_background.position.y, m_background.position.z + 0.1f);
		glm::vec4 textBox(m_background.position.x, m_background.position.y,
			m_background.dimensions.x, m_background.dimensions.y);
		std::string maskText = "";
		for (size_t i = 0; i < m_text.length(); i++) {
			maskText += "*";
		}
		App::Graphics.submit(maskText, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), AC_ALIGN_NONE, m_verticalAlignment,
			m_parent->getShader());
		if (m_selected) {
			App::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void PasswordBox::update(float dt) {
		m_cursor.position = glm::vec3(m_textHorizontalPosition, 0.0f,
			m_background.position.z + 0.2f);
		const Font* font = App::Graphics.getFont(m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			char c = '*';
			m_cursor.position.x += (font->glyphs.at(c).offset
				- (cp == 0 ? font->glyphs.at(c).bearing.x : 0))
				* m_parent->getTextScale();
		}
		switch (m_cursorAlignment) {
		case AC_ALIGN_BOTTOM: {
			m_cursor.position.y = m_background.position.y;
			break;
		}
		case AC_ALIGN_CENTER: {
			m_cursor.position.y = m_background.position.y
				+ (m_background.dimensions.y - m_cursor.dimensions.y) / 2.0f;
			break;
		}
		case AC_ALIGN_TOP: {
			m_cursor.position.y = m_background.position.y
				+ m_background.dimensions.y - m_cursor.dimensions.y;
			break;
		}
		default: {
			m_cursor.position.y = m_background.position.y
				+ (m_background.dimensions.y - m_cursor.dimensions.y) / 2.0f;
			break;
		}
		}
		if (m_cursor.position.x < m_background.position.x) {
			m_textHorizontalPosition += m_background.dimensions.x / 5.0f;
			if (m_textHorizontalPosition > m_background.position.x) {
				m_textHorizontalPosition = m_background.position.x;
			}
		}
		else if (m_cursor.position.x + m_cursor.dimensions.x
		> m_background.position.x + m_background.dimensions.x) {
			m_textHorizontalPosition -= m_background.dimensions.x / 5.0f;
		}
		m_timer += dt;
		if (m_timer > 60.0f) {
			m_timer = 0.0f;
		}
	}

	void PasswordBox::destroy() {
		for (size_t i = 0; i < m_text.length(); i++) {
			m_text[i] = '0';
		}
		m_text.clear();
		TextBox::destroy();
	}

	// Implement ListBox class functions.

	void ListBox::init(const glm::vec3& position, const glm::vec2& dimensions,
		const std::string& textureFileName, const std::string& labelText,
		const std::vector<std::string>& values, unsigned int valueCount,
		const std::string& elementTextureFileName, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment horizontalAlignment, Alignment verticalAlignment) {
		Label::init(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		m_elementTextureFileName = elementTextureFileName;
		m_nextID = 0;
		m_valueCount = valueCount;
		m_topIndex = 0;
		for (const std::string& value : values) {
			addValue(value);
		}
		setPosition(position);
		setDimensions(dimensions);
		setHorizontalAlignment(horizontalAlignment);
		setVerticalAlignment(verticalAlignment);
	}

	void ListBox::setPosition(const glm::vec3& position) {
		Label::setPosition(position);
		float elementX = m_background.position.x
			/ (float)App::Window.getDimensions().x;
		float elementPixelHeight = m_background.dimensions.y
			/ (float)m_valueCount;
		float elementHeight = elementPixelHeight
			/ (float)App::Window.getDimensions().y;
		float elementY = ((m_background.position.y + m_background.dimensions.y
			- elementPixelHeight) + (elementPixelHeight * m_topIndex))
			/ (float)App::Window.getDimensions().y;
		for (int i = 0; i < m_elements.size(); i++) {
			m_elements[i].setPosition(glm::vec3(elementX, elementY,
				m_background.position.z + 0.1f));
			elementY -= elementHeight;
		}
	}

	void ListBox::setDimensions(const glm::vec2& dimensions) {
		Label::setDimensions(dimensions);
		float elementWidth = m_background.dimensions.x
			/ (float)App::Window.getDimensions().x;
		float elementHeight = (m_background.dimensions.y / (float)m_valueCount)
			/ (float)App::Window.getDimensions().y;
		for (Switch& element : m_elements) {
			element.setDimensions(glm::vec2(elementWidth, elementHeight));
		}
	}

	std::vector<std::string> ListBox::getValues() const {
		std::vector<std::string> values;
		for (const Switch& element : m_elements) {
			values.push_back(element.getLabelText());
		}
		return values;
	}

	const std::string ListBox::getValue() const {
		for (const Switch& element : m_elements) {
			if (element.isOn()) {
				return element.getLabelText();
			}
		}
		return "";
	}

	void ListBox::setValue(const std::string& value) {
		int index = getValueIndex(value);
		if (index == -1) {
			addValue(value);
			index = getValueIndex(value);
		}
		setValueIndex((unsigned int)index);
	}

	int ListBox::getValueIndex() const {
		for (int i = 0; i < m_elements.size(); i++) {
			if (m_elements.at(i).isOn()) {
				return i;
			}
		}
		return -1;
	}

	int ListBox::getValueIndex(const std::string& value) const {
		for (int i = 0; i < m_elements.size(); i++) {
			if (m_elements.at(i).getLabelText() == value) {
				return i;
			}
		}
		return -1;
	}

	bool ListBox::setValueIndex(unsigned int index) {
		if (index >= m_elements.size()) {
			return false;
		}
		for (Switch& element : m_elements) {
			element.setOn(false);
		}
		m_elements[index].setOn(true);
		return true;
	}

	void ListBox::addValue(const std::string& value, int index) {
		Switch* sw = 0;
		if (index < 0) {
			m_elements.push_back(Switch());
			sw = &m_elements.back();
		}
		else {
			m_elements.insert(m_elements.begin() + index, Switch());
			sw = &m_elements[index];
		}
		sw->init(glm::vec3(), glm::vec2(),
			m_elementTextureFileName, value, AC_ALIGN_CENTER,
			m_horizontalAlignment, m_verticalAlignment);
		sw->m_ID = m_nextID++;
		sw->m_parent = this;
		setPosition(getPosition());
	}

	bool ListBox::removeValue(const std::string& value) {
		bool f = false;
		std::vector<Switch>::iterator it = m_elements.begin();
		for (it; it != m_elements.end(); it++) {
			if (it->getLabelText() == value) {
				f = true;
				break;
			}
		}
		if (!f) {
			return false;
		}
		it->destroy();
		m_elements.erase(it);
		setPosition(getPosition());
		return true;
	}

	bool ListBox::removeValue(unsigned int index) {
		if (index >= m_elements.size()) {
			return false;
		}
		std::vector<Switch>::iterator it = m_elements.begin() + index;
		it->destroy();
		m_elements.erase(it);
		setPosition(getPosition());
		return true;
	}

	unsigned int ListBox::getValueCount() const {
		return m_valueCount;
	}

	unsigned int ListBox::getTopIndex() const {
		return m_topIndex;
	}

	bool ListBox::setTopIndex(unsigned int topIndex) {
		if (m_elements.size() <= m_valueCount && topIndex > 0) {
			return false;
		}
		if (topIndex > m_elements.size() - m_valueCount) {
			return false;
		}
		m_topIndex = topIndex;
		setPosition(getPosition());
		return true;
	}

	Alignment ListBox::getHorizontalAlignment() const {
		return m_horizontalAlignment;
	}

	void ListBox::setHorizontalAlignment(Alignment horizontalAlignment) {
		m_horizontalAlignment = horizontalAlignment;
		for (Switch& element : m_elements) {
			element.setHorizontalLabelAlignment(horizontalAlignment);
		}
	}

	Alignment ListBox::getVerticalAlignment() const {
		return m_verticalAlignment;
	}

	void ListBox::setVerticalAlignment(Alignment verticalAlignment) {
		m_verticalAlignment = verticalAlignment;
		for (Switch& element : m_elements) {
			element.setVerticalLabelAlignment(verticalAlignment);
		}
	}

	void ListBox::componentEvent(int cID, int eID) {
		for (Switch& element : m_elements) {
			if (cID == element.getID()) {
				element.setOn(true);
			}
			else {
				element.setOn(false);
			}
		}
		m_parent->componentEvent(m_ID, ValueSetEvent);
	}

	Shader& ListBox::getShader() {
		return m_parent->getShader();
	}

	const std::string& ListBox::getFontFileName() const {
		return m_parent->getFontFileName();
	}

	float ListBox::getTextScale() const {
		return m_parent->getTextScale();
	}

	const glm::vec4& ListBox::getTextColor() const {
		return m_parent->getTextColor();
	}

	void ListBox::setEnabled(bool enabled) {
		Label::setEnabled(enabled);
		for (Switch& element : m_elements) {
			element.setEnabled(enabled);
		}
	}

	void ListBox::setShader(Shader& shader) {

	}

	void ListBox::setFontFileName(const std::string& fontFileName) {

	}

	void ListBox::setTextScale(float textScale) {

	}

	void ListBox::setTextColor(const glm::vec4& textColor) {

	}

	void ListBox::draw() {
		Label::draw();
		for (unsigned int i = m_topIndex; i < m_valueCount + m_topIndex; i++) {
			if (i < m_elements.size()) {
				m_elements[i].draw();
			}
		}
	}

	void ListBox::processInput() {
		for (Switch& element : m_elements) {
			element.processInput();
		}
		if (MouseOver(*this)) {
			float scroll = App::Input.getMouseScroll().y;
			if (scroll != 0.0f && m_timer > 10.0f) {
				if (scroll < 0.0f) {
					setTopIndex(getTopIndex() + 1);
				}
				else {
					setTopIndex(getTopIndex() - 1);
				}
				m_timer = 0.0f;
			}
		}
	}

	void ListBox::update(float dt) {
		m_timer += dt;
		if (m_timer > 60.0f) {
			m_timer = 0.0f;
		}
	}

	void ListBox::destroy() {
		Label::destroy();
		m_elementTextureFileName = "";
		for (Switch& element : m_elements) {
			element.destroy();
		}
		m_elements.clear();
		m_nextID = 0;
		m_valueCount = 0;
		m_topIndex = 0;
		m_timer = 0.0f;
		m_horizontalAlignment = AC_ALIGN_NONE;
		m_verticalAlignment = AC_ALIGN_NONE;
	}
}