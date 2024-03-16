// Acetylene ~ UI Header
// Record 01-000E
// 2024.03.14 @ 17.09

// File:	UI.h
// Author:	VirtuteVerbis (virtuteverbis@gmail.com)
// Created:	2024.03.14 @ 17.09
// Purpose:	Declare user-interface classes and components to be used in
//			Acetylene apps.

#ifndef AC_UI_H
#define AC_UI_H

#include "Entity.h"
#include "Input.h"

namespace Acetylene {
	// Forward declare the Scene, Shader, and UIGroup classes.
	class Scene;
	class Shader;
	class UIGroup;

	// Abstract interface for any controller of multiple UI components.
	class UIListener {
	public:
		/*
		A child UI component has triggered an event.
		@param int: The ID number of the component.
		@param int: The ID number of the event type.
		*/
		virtual void componentEvent(int, int) = 0;
		/*
		Get the shader used to draw children of this UI listener.
		@return Shader&: A reference to the shader.
		*/
		virtual Shader& getShader() = 0;
		/*
		Set the shader used to draw children of this UI listener.
		@param Shader&: A reference to the new shader.
		*/
		virtual void setShader(Shader&) = 0;
		/*
		Get the font used for drawing children of this UI listener.
		@return const std::string&: The file name of the font.
		*/
		virtual const std::string& getFontFileName() const = 0;
		/*
		Set the font used for drawing children of this UI listener.
		@param const std::string& fontFileName: The file name of the new font.
		*/
		virtual void setFontFileName(const std::string&) = 0;
		/*
		Get the scale used for drawing text on children of this UI listener.
		@return float: The text scale.
		*/
		virtual float getTextScale() const = 0;
		/*
		Set the scale used for drawing text on children of this UI listener.
		@param float: The new text scale.
		*/
		virtual void setTextScale(float) = 0;
		/*
		Get the color used for drawing text on children of this UI listener.
		@return const glm::vec4&: The text color in RGBA 0 to 1.
		*/
		virtual const glm::vec4& getTextColor() const = 0;
		/*
		Set the color used for drawing text on children of this UI listener.
		@param const glm::vec4&: The new text color in RGBA 0 to 1.
		*/
		virtual void setTextColor(const glm::vec4&) = 0;
	};

	// An abstract user interface component.
	class UIComponent : public InputListener {
	public:
		/*
		Get the parent UI listener of this component.
		@return UIListener&: A reference to the parent.
		*/
		virtual UIListener& getParent();
		/*
		Get the ID number of this component.
		@return unsigned int: The ID number.
		*/
		virtual unsigned int getID() const;
		/*
		Test whether this component is enabled for user input.
		@return bool: Whether the component is enabled.
		*/
		virtual bool isEnabled() const;
		/*
		Set whether this component is enabled for user input.
		@param bool enabled: Whether this component should be enabled.
		*/
		virtual void setEnabled(bool);
		/*
		Test whether this component is visible on the window.
		@return bool: Whether the component is visible.
		*/
		virtual bool isVisible() const;
		/*
		Set whether this component is visible on the window.
		@param bool visible: Whether this component should be visible.
		*/
		virtual void setVisible(bool);
		/*
		Get the normalized position of this component.
		@return glm::vec3: The normalized 2D position and depth.
		*/
		virtual glm::vec3 getPosition() const = 0;
		/*
		Set the normalized position of this component.
		@param const glm::vec3&: The new normalized 2D position and depth.
		*/
		virtual void setPosition(const glm::vec3&) = 0;
		/*
		Get the normalized dimensions of this component.
		@return glm::vec2: The normalized dimensions.
		*/
		virtual glm::vec2 getDimensions() const = 0;
		/*
		Set the normalized dimensions of this component.
		@param const glm::vec2&: The new normalized dimensions.
		*/
		virtual void setDimensions(const glm::vec2&) = 0;

	protected:
		// Allow the UIGroup class access to component memory.
		friend class UIGroup;

		// The UI listener parent of this component.
		UIListener* m_parent = 0;
		// The ID number of this component.
		int m_ID = 0;
		// Whether this component is currently enabled for user input.
		bool m_enabled = true;
		// Whether this component is visible on the window.
		bool m_visible = true;

		/*
		Draw this component's graphics using the App's renderer.
		*/
		virtual void draw() = 0;
		/*
		Process user input to this component.
		*/
		virtual void processInput() = 0;
		/*
		Update this component's logic.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) = 0;
		/*
		Free this component's memory.
		*/
		virtual void destroy() = 0;
	};

	// A group of user interface components drawn with the same shader and
	// font with a parent scene.
	class UIGroup : public UIListener {
	public:
		/*
		Initialize this UI group's memory.
		@param Scene& parent: A reference to the parent scene of this UI group.
		@param unsigned int ID: The ID number to assign to this UI group.
		@param const std::string& fontFileName: The file name of the font to
		use when drawing text on child components of this UI group.
		@param float textScale: The scale to draw text at on child components
		of this UI group.
		@param const glm::vec4& textColor: The RGBA color to draw text in on
		child components of this UI group.
		*/
		void init(Scene&, unsigned int, const std::string&, float,
			const glm::vec4&);
		/*
		Add a component as a child of this UI group.
		@param UIComponent& component: A reference to the component to add.
		@return bool: Whether the component was successfully added.
		*/
		bool addComponent(UIComponent&);
		/*
		Remove a component as a child of this UI group.
		@param UIComponent& component: A reference to the component to remove.
		@return bool: Whether the component was removed successfully.
		*/
		bool removeComponent(UIComponent&);
		/*
		Call the draw functions of all this UI group's child components.
		*/
		void draw();
		/*
		Call the processInput function of all this UI group's child components.
		*/
		void processInput();
		/*
		Process an event triggered by a child of this group.
		@param int cID: The ID number of the component that triggered the
		event.
		@param int eID: The ID number of the event type.
		*/
		void componentEvent(int, int) override;
		/*
		Call the update function of all this UI group's child components.
		@param float dt: The number of frames elapsed since the last update.
		*/
		void update(float);
		/*
		Free this group's memory and that of all its child components.
		*/
		void destroy();
		/*
		Get this group's parent scene.
		@return Scene&: A reference to the parent scene.
		*/
		Scene& getParent();
		/*
		Get this group's ID number assigned by its parent scene.
		@return unsigned int: The ID number.
		*/
		unsigned int getID() const;
		/*
		Get the shader used to draw all this UI group's child components.
		@return Shader&: A reference to the shader.
		*/
		Shader& getShader() override;
		/*
		Set the shader used to draw all this UI group's child components.
		@param Shader& shader: A reference to the new shader.
		*/
		void setShader(Shader&) override;
		/*
		Get the file name of the font used to draw text on the child components
		of this UI group.
		@return const std::string&: The font file name.
		*/
		const std::string& getFontFileName() const override;
		/*
		Set the file name of the font used to draw text on the child components
		of this UI group.
		@param const std::string& fontFileName: The new font file name.
		*/
		void setFontFileName(const std::string&) override;
		/*
		Get the scale used to draw text at on this UI group's child components.
		@return float: The text scale.
		*/
		float getTextScale() const override;
		/*
		Set the scale used to draw text at on this UI group's child components.
		@param float textScale: The new text scale.
		*/
		void setTextScale(float) override;
		/*
		Get the RGBA color used to draw text in on this UI group's child
		components.
		@return const glm::vec4&: The text color.
		*/
		const glm::vec4& getTextColor() const override;
		/*
		Set the RGBA color used to draw text in on this UI group's child
		components.
		@param const glm::vec4& textColor: The new text color.
		*/
		void setTextColor(const glm::vec4&) override;
		/*
		Test whether this UI group's components are enabled.
		@return bool: Whether this UI group is enabled.
		*/
		bool isEnabled() const;
		/*
		Set whether this UI group's components are enabled.
		@param bool enabled: Whether to set this UI group to enabled or
		disabled.
		*/
		void setEnabled(bool);
		/*
		Test whether this UI group's components are visible.
		@return bool: Whether this UI group is visible.
		*/
		bool isVisible() const;
		/*
		Set whether this UI group's components are visible.
		@param bool visible: Whether to set this UI group to visible or
		invisible.
		*/
		void setVisible(bool);

	private:
		// This group's parent scene.
		Scene* m_parent = 0;
		// This group's ID number.
		int m_ID = 0;
		// Pointer to the components of this group.
		std::vector<UIComponent*> m_components;
		// The next ID number to assign to a component.
		int m_nextID = 0;
		// The shader used to draw this group's components.
		Shader* m_shader = 0;
		// The font used to draw this group's components.
		std::string m_fontFileName = "";
		// The scale to display text at on this group's components.
		float m_textScale = 1.0f;
		// The color to display text in on this group's components.
		glm::vec4 m_textColor = glm::vec4();
		// Whether this group's components are enabled.
		bool m_enabled = true;
		// Whether this group's components are visible.
		bool m_visible = true;
		// The dimensions of the window in the last update.
		glm::ivec2 m_prevWindowDimensions = glm::ivec2();
	};

	// Text label UI component.
	class Label : public UIComponent {
	public:
		/*
		Initialize this label's memory.
		@param const glm::vec3& position: The normalized position of this
		label.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		label.
		@param const std::string& textureFileName: The file name of the texture
		to use as this label's background.
		@param const std::string& labelText: The text to appear on this label.
		@param Alignment labelAlignment: The alignment of this label's text
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this label's text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		label's text in its bounding box.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_LEFT,
			Alignment = AC_ALIGN_CENTER);
		/*
		Get the normalized position of this label.
		@return glm::vec3: The position of this label.
		*/
		virtual glm::vec3 getPosition() const override;
		/*
		Set the normalized position of this label.
		@param const glm::vec3& position: The new position for this label.
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Get the normalized dimensions of this label.
		@return glm::vec2: The dimensions of this label.
		*/
		virtual glm::vec2 getDimensions() const override;
		/*
		Set the normalized dimensions of this label.
		@param const glm::vec2& dimensions: The new dimensions for this label.
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Get the text appearing in this label.
		@return const std::string&: The label text.
		*/
		virtual const std::string& getLabelText() const;
		/*
		Set the text appearing in this label.
		@param const std::string& labelText: The new label text.
		*/
		virtual void setLabelText(const std::string&);
		/*
		Get the alignment of this label's text about its background.
		@return Alignment: The label alignment.
		*/
		virtual Alignment getLabelAlignment() const;
		/*
		Set the alignment of this label's text about its background.
		@param Alignment labelAlignment: The new label alignment.
		*/
		virtual void setLabelAlignment(Alignment);
		/*
		Get the horizontal alignment of this label's text within its bounding
		box.
		@return Alignment: The horizontal alignment of this label.
		*/
		virtual Alignment getHorizontalLabelAlignment() const;
		/*
		Set the horizontal alignment of this label's text within its bounding
		box.
		@param Alignment horizontalLabelAlignment: The new horizontal alignment
		of this label.
		*/
		virtual void setHorizontalLabelAlignment(Alignment);
		/*
		Get the vertical alignment of this label's text within its bounding
		box.
		@return Alignment: The vertical alignment of this label.
		*/
		virtual Alignment getVerticalLabelAlignment() const;
		/*
		Set the vertical alignment of this label's text within its bounding
		box.
		@param Alignment verticalLabelAlignment: The new vertical alignment of
		this label.
		*/
		virtual void setVerticalLabelAlignment(Alignment);

	protected:
		// The sprite used for texturing the background of this label.
		Sprite m_background;
		// The text to display in this label.
		std::string m_labelText = "";
		// The alignment of this label's text about its background.
		Alignment m_labelAlignment = AC_ALIGN_NONE;
		// The horizontal alignment of the text within this label.
		Alignment m_horizontalLabelAlignment = AC_ALIGN_NONE;
		// The vertical alignment of the text within this label.
		Alignment m_verticalLabelAlignment = AC_ALIGN_NONE;

		/*
		Draw this label's background sprite and text.
		*/
		virtual void draw() override;
		/*
		Do not process user input to labels.
		*/
		virtual void processInput() override;
		/*
		Do not update logic for labels.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this label's memory.
		*/
		virtual void destroy() override;
	};

	// Button UI element.
	class Button : public Label {
	public:
		// Event ID for unselected (moused off of) button.
		const static unsigned int UnselectedEvent = 0;
		// Event ID for selected (moused over) button.
		const static unsigned int SelectedEvent = 1;
		// Event ID for clicked button.
		const static unsigned int ClickedEvent = 2;

		/*
		Initialize this button's memory.
		@param const glm::vec3& position: The normalized position of this
		button.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		button.
		@param const std::string& textureFileName: The file name of the texture
		to use as this button's background.
		@param const std::string& labelText: The text to appear on this button.
		@param Alignment labelAlignment: The alignment of this button's text
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this button's text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		button's text in its bounding box.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_CENTER,
			Alignment = AC_ALIGN_CENTER) override;
		/*
		Test whether this button is currently selected (moused over).
		@return bool: Whether this button is selected.
		*/
		virtual bool isSelected() const;
		/*
		Set whether this button is currently selected.
		@param bool selected: Whether to set this button set to selected or
		unselected.
		*/
		virtual void setSelected(bool);
		/*
		Test whether this button has been clicked (it is selected and the left
		mouse button is down).
		@return bool: Whether this button has been clicked.
		*/
		virtual bool isClicked() const;
		/*
		Set whether this button is in its clicked state.
		@param bool clicked: Whether this button should be set to clicked or
		not.
		*/
		virtual void setClicked(bool);
		/*
		Set whether this button is enabled for user input.
		@param bool enabled: Whether this button should be enabled.
		*/
		virtual void setEnabled(bool) override;

	protected:
		// Allow the carousel class to access buttons' memory.
		friend class Carousel;

		// The texture coordinates for unselected button image.
		static glm::vec4 UnselectedTextureCoordinates;
		// The texture coordinates for selected button image.
		static glm::vec4 SelectedTextureCoordinates;
		// The texture coordinates for clicked button image.
		static glm::vec4 ClickedTextureCoordinates;
		// Whether this button is currently selected.
		bool m_selected = false;
		// Whether this button has been clicked.
		bool m_clicked = false;

		/*
		Process input to this button to set its selected and clicked states.
		*/
		virtual void processInput() override;
		/*
		Free this button's memory.
		*/
		virtual void destroy() override;
	};

	// Switch UI element.
	class Switch : public Button {
	public:
		// Event ID for switch turned off.
		const static unsigned int OffEvent = 0;
		// Event ID for switch turned on.
		const static unsigned int OnEvent = 1;

		/*
		Initialize this switch's memory.
		@param const glm::vec3& position: The normalized position of this
		switch.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		button.
		@param const std::string& textureFileName: The file name of the texture
		to use as this switch's background.
		@param const std::string& labelText: The text to appear on this switch.
		@param Alignment labelAlignment: The alignment of this switch's text
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this switch's text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		switch's text in its bounding box.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			Alignment = AC_ALIGN_TOP, Alignment = AC_ALIGN_LEFT,
			Alignment = AC_ALIGN_BOTTOM) override;
		/*
		Test whether this switch is on.
		@return bool: Whether this switch is on.
		*/
		virtual bool isOn() const;
		/*
		Set whether this switch is on.
		@param bool on: Whether this switch should be set to its on or off
		state.
		*/
		virtual void setOn(bool);

	protected:
		// Allow the ListBox class to access the switch's draw function.
		friend class ListBox;

		// The texture coordinates for turned off switch.
		static glm::vec4 OffTextureCoordinates;
		// The texture coordinates for turned on switch.
		static glm::vec4 OnTextureCoordinates;
		// Whether this switch is currently turned on.
		bool m_on = false;

		/*
		Process input to this switch.
		*/
		virtual void processInput() override;
		/*
		Free this switch's memory.
		*/
		virtual void destroy() override;
	};

	// Slider UI component.
	class Slider : public Label {
	public:
		// Event ID for updated slider value.
		const static unsigned int ValueSetEvent = 0;

		/*
		Initialize this slider's memory.
		@param const glm::vec3& position: The normalized position of this
		slider.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		slider.
		@param const std::string& textureFileName: The file name of the texture
		for the background of this slider.
		@param const glm::vec2& cursorDimensions: The dimensions of the cursor
		for this slider in pixels.
		@param const std::string& cursorTextureFileName: The file name of the
		texture for the cursor on this slider.
		@param const std::string& labelText: The text to appear in the label
		for this slider.
		@param Alignment labelAlignment: The alignment of this slider's label
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this slider's label text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		slider's label text in its bounding box.
		@param Alignment cursorAlignment: The vertical alignment of this
		slider's cursor in its background.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const glm::vec2&, const std::string&,
			const std::string&, Alignment = AC_ALIGN_TOP,
			Alignment = AC_ALIGN_LEFT, Alignment = AC_ALIGN_BOTTOM,
			Alignment = AC_ALIGN_CENTER);
		/*
		Get the alignment of this slider's cursor.
		@return Alignment: The vertical alignment of the cursor on this
		slider's background.
		*/
		virtual Alignment getCursorAlignment() const;
		/*
		Set the alignment of this slider's cursor.
		@param Alignment cursorAlignment: The new vertical alignment of the
		cursor on this slider's background.
		*/
		virtual void setCursorAlignment(Alignment);
		/*
		Get the current value of this slider.
		@return float: The current value of this slider (0.0 - 1.0).
		*/
		virtual float getValue() const;
		/*
		Set the value of this slider.
		@param float value: The new value for this slider (0.0 - 1.0).
		*/
		virtual void setValue(float);
		/*
		Test whether this slider is currently selected (moused over).
		@return bool: Whether this slider is selected.
		*/
		virtual bool isSelected() const;
		/*
		Set whether this slider is in its selected state.
		@param bool selected: The new selected state for this slider.
		*/
		virtual void setSelected(bool);
		/*
		Set whether this slider is enabled for user input.
		@param bool enabled: Whether this slider should be enabled.
		*/
		virtual void setEnabled(bool) override;

	protected:
		// The texture coordinates for an unselected slider.
		static glm::vec4 UnselectedTextureCoordinates;
		// The texture coordinates for a selected slider.
		static glm::vec4 SelectedTextureCoordinates;
		// The vertical alignment of this slider's cursor within its
		// background.
		Alignment m_cursorAlignment = AC_ALIGN_NONE;
		// The cursor sprite for this slider.
		Sprite m_cursor;
		// The current value of this sprite.
		float m_value = 0.0f;
		// Whether this slider is currently selected.
		bool m_selected = false;

		/*
		Draw this slider.
		*/
		virtual void draw() override;
		/*
		Process user input to this slider.
		*/
		virtual void processInput() override;
		/*
		Update this slider's logic.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this slider's memory.
		*/
		virtual void destroy() override;
	};

	// Text input field UI component.
	class TextBox : public Label {
	public:
		// Event ID for unselected (clicked out of) text box.
		const static unsigned int UnselectedEvent = 0;
		// Event ID for selected (clicked into) text box.
		const static unsigned int SelectedEvent = 1;
		// Event ID for text entered into text box.
		const static unsigned int TextEnteredEvent = 2;

		/*
		Initialize this text box's memory.
		@param const glm::vec3& position: The normalized position of this text
		box.
		@param const glm::vec2& dimensions: The normalized position of this
		text box.
		@param const std::string& textureFileName: The file name for the
		texture of this text box's background.
		@param const std::string& labelText: The text to appear in the label
		for this text box.
		@param const glm::vec2& cursorDimensions: The dimensions of the cursor
		for this text box in pixels.
		@param const std::string& cursorTextureFileName: The file name for the
		texture of this text box's cursor.
		@param const std::string& text: The initial text to appear in this text
		box.
		@param int maxCharacters: The maximum number of characters allowed in
		this text box or -1 for infinite.
		@param const std::string& allowedCharacters: The set of characters
		allowed in this text box or empty string for all characters allowed.
		@param Alignment labelAlignment: The alignment of this text box's label
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this text box's label text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		text box's label text in its bounding box.
		@param Alignment cursorAlignment: The vertical alignment of this text
		box's cursor in its background.
		@param Alignment verticalAlignment: The vertical alignment of this text
		box's text in its background.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&, const glm::vec2&,
			const std::string&, const std::string&, int = -1,
			const std::string& = "", Alignment = AC_ALIGN_TOP,
			Alignment = AC_ALIGN_LEFT, Alignment = AC_ALIGN_BOTTOM,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_CENTER);
		/*
		Test whether this text box is currently selected (clicked into).
		@return bool: This text box's selected state.
		*/
		virtual bool isSelected() const;
		/*
		Set whether this text box is currently selected.
		@param bool selected: This text box's new selected state.
		*/
		virtual void setSelected(bool);
		/*
		Set the normalized position of this text box.
		@param const glm::vec3& The new normalized position for this text box.
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Get this text box's content text.
		@return const std::string&: The content of this text box.
		*/
		virtual const std::string& getText() const;
		/*
		Set this text box's content text.
		@param const std::string& text: The new content for this text box.
		*/
		virtual void setText(const std::string&);
		/*
		Get the vertical alignment of this text box's cursor in its background.
		@return Alignment: The cursor alignment.
		*/
		virtual Alignment getCursorAlignment() const;
		/*
		Set the vertical alignment of this text box's cursor in its background.
		@param Alignment cursorAlignment: The new cursor alignment.
		*/
		virtual void setCursorAlignment(Alignment);
		/*
		Get the position of this text box's cursor in its content string.
		@return unsigned int: The cursor position.
		*/
		virtual unsigned int getCursorPosition() const;
		/*
		Set the position of this text box's cursor in its content string.
		@param unsigned int cursorPosition: The new cursor position.
		*/
		virtual bool setCursorPosition(unsigned int);
		/*
		Get the maximum number of characters allowed in this text box.
		@return int: The maximum number of characters allowed.
		*/
		virtual int getMaxCharacters() const;
		/*
		Set the maximum number of characters allowed in this text box.
		@param int maxCharacters: The new maximum number of characters allowed.
		*/
		virtual void setMaxCharacters(int);
		/*
		Get the set of characters allowed in this text box.
		@return const std::string&: The set of allowed characters.
		*/
		virtual const std::string& getAllowedCharacters() const;
		/*
		Set the set of characters allowed in this text box.
		@param const std::string& allowedCharacters: The new set of allowed
		characters.
		*/
		virtual void setAllowedCharacters(const std::string&);
		/*
		Get the vertical alignment of this text box's content in its
		background.
		@return Alignment: This text box's vertical alignment.
		*/
		virtual Alignment getVerticalAlignment() const;
		/*
		Set the vertical alignment of this text box's content in its
		background.
		@param Alignment verticalAlignment: This text box's new vertical
		alignment.
		*/
		virtual void setVerticalAlignment(Alignment);
		/*
		A character has been typed on this text box.
		@param char c: The typed character.
		*/
		virtual void charTyped(char) override;
		/*
		Set whether this text box is enabled for user input.
		@param bool enabled: Whether this text box should be enabled.
		*/
		virtual void setEnabled(bool) override;

	protected:
		// Texture coordinates for the unselected text box.
		static glm::vec4 UnselectedTextureCoordinates;
		// Texture coordinates for the selected text box.
		static glm::vec4 SelectedTextureCoordinates;
		// Whether this text box is currently selected.
		bool m_selected = false;
		// The text to display in this text box.
		std::string m_text = "";
		// The position of the text within its bounds.
		float m_textHorizontalPosition = 0.0f;
		// The alignment of this text box's cursor within its background.
		Alignment m_cursorAlignment = AC_ALIGN_NONE;
		// The cursor to display on this text box.
		Sprite m_cursor;
		// The position of this text box's cursor in its content.
		unsigned int m_cursorPosition = 0;
		// Timer for tracking repeated control inputs.
		float m_timer = 0.0f;
		// The set of characters allowed in this text box.
		std::string m_allowedCharacters = "";
		// The maximum number of characters allowed in this text box.
		int m_maxCharacters = -1;
		// The vertical alignment of this text box's content in its bounds.
		Alignment m_verticalAlignment = AC_ALIGN_NONE;

		/*
		Draw this text box's graphics.
		*/
		virtual void draw() override;
		/*
		Process user input to this text box.
		*/
		virtual void processInput() override;
		/*
		Update this text box's physics.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this text box's memory.
		*/
		virtual void destroy() override;
	};

	// Carousel UI component.
	class Carousel : public Label, UIListener {
	public:
		// Event ID for carousel value changed.
		const static unsigned int ValueSetEvent = 0;

		/*
		Initialize this carousel's memory.
		@param const glm::vec3& position: The normalized position of this
		carousel.
		@param const glm::vec2& dimensions: The normalized position of this
		carousel.
		@param const std::string& textureFileName: The file name of the texture
		for the background of this carousel.
		@param const std::string& labelText: The text to appear in the label
		for this carousel.
		@param const std::vector<std::string>& values: The set of possible
		values for this carousel.
		@param const std::string& value: The initial value of this carousel
		(does not have to be in the initial possible values).
		@param const std::string& buttonTextureFileName: The file name of the
		texture for the previous and next buttons of this carousel.
		@param Alignment labelAlignment: The alignment of this carousel's
		label about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this carousel's label text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		carousel's label text in its bounding box.
		@param Alignment horizontalAlignment: The horizontal alignment of this
		carousel's value text.
		@param Alignment verticalAlignment: The vertical alignment of this
		carousel's value text.
		@param Alignment buttonAlignment: The alignment of this carousel's
		buttons about its background, may be left, center, or right.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			const std::vector<std::string>&, const std::string&,
			const std::string&, Alignment = AC_ALIGN_TOP,
			Alignment = AC_ALIGN_LEFT, Alignment = AC_ALIGN_BOTTOM,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_CENTER,
			Alignment = AC_ALIGN_RIGHT);
		/*
		Get the set of possible values for this carousel.
		@return const std::vector<std::string>&: The set of possible values.
		*/
		virtual const std::vector<std::string>& getValues() const;
		/*
		Get the current value for this carousel.
		@return const std::string&: The current value.
		*/
		virtual const std::string& getValue() const;
		/*
		Set the current value for this carousel, will add to the set of
		possible values if not already present.
		@param const std::string& value: The new value.
		*/
		virtual void setValue(const std::string&);
		/*
		Get the current index of this carousel.
		@return unsigned int: The index of the current value in the list of
		possible values.
		*/
		virtual unsigned int getValueIndex() const;
		/*
		Get the index of a given value in this carousel.
		@param const std::string& value: The value to test the index of.
		@return int: The index of the value in the list of possible values or
		-1 if the value was not found.
		*/
		virtual int getValueIndex(const std::string&) const;
		/*
		Set the current index of this carousel.
		@param unsigned int index: The new index.
		*/
		virtual void setValueIndex(unsigned int);
		/*
		Add a possible value to this carousel.
		@param const std::string& value: The new value to add.
		@param int index: The index to add the new value at or -1 for the end
		of the set of possible values.
		@return bool: Whether the value could be added (the index was in
		range).
		*/
		virtual bool addValue(const std::string&, int = -1);
		/*
		Remove the first instance of a value from this carousel.
		@param const std::string& value: The value to remove.
		@return bool: Whether the value was found and successfully removed.
		*/
		virtual bool removeValue(const std::string&);
		/*
		Remove a value by index from this carousel.
		@param unsigned int index: The index in the list of possible values to
		remove.
		@return bool: Whether the index was in range and the value was removed.
		*/
		virtual bool removeValue(unsigned int);
		/*
		Get the horizontal alignment of this carousel's value.
		@return Alignment: The horizontal alignment of this carousel's value.
		*/
		virtual Alignment getHorizontalAlignment() const;
		/*
		Set the horizontal alignment of this carousel's value.
		@param Alignment horizontalAlignment: The new horizontal alignment of
		this carousel's value.
		*/
		virtual void setHorizontalAlignment(Alignment);
		/*
		Get the vertical alignment of this carousel's value.
		@return Alignment: The vertical alignment of this carousel's value.
		*/
		virtual Alignment getVerticalAlignment() const;
		/*
		Set the vertical alignment of this carousel's value.
		@param Alignment verticalAlignment: The new vertical alignment of this
		carousel's value.
		*/
		virtual void setVerticalAlignment(Alignment);
		/*
		Get the alignment of this carousel's buttons about its background.
		@return Alignment: The button alignment.
		*/
		virtual Alignment getButtonAlignment() const;
		/*
		Set the alignment of this carousel's buttons about its background.
		@param Alignment buttonAlignment: The new button alignment (must be
		either AC_ALIGN_LEFT, AC_ALIGN_CENTER, or AC_ALIGN_RIGHT).
		*/
		virtual void setButtonAlignment(Alignment);
		/*
		Get the normalized position of this carousel.
		@return glm::vec3: The normalized position of this carousel.
		*/
		virtual glm::vec3 getPosition() const override;
		/*
		Set the normalized position of this carousel.
		@param const glm::vec3& position: The new normalized position of this
		carousel.
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Get the normalized dimensions of this carousel.
		@return glm::vec2: The normalized dimensions of this carousel.
		*/
		virtual glm::vec2 getDimensions() const override;
		/*
		Set the normalized dimensions of this carousel.
		@param const glm::vec2& dimensions: The new normalized dimensions of 
		this carousel.
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		A child button has triggered an event.
		@param int cID: The ID number of the button which triggered the event.
		@param int eID: The ID number of the event type.
		*/
		virtual void componentEvent(int, int) override;
		/*
		Get the shader used to draw child buttons of this carousel.
		@return Shader&: A reference to the shader.
		*/
		virtual Shader& getShader() override;
		/*
		Get the font used to draw child buttons of this carousel.
		@return const std::string&: The file name of the font.
		*/
		virtual const std::string& getFontFileName() const override;
		/*
		Get the scale used for drawing text on child buttons of this carousel.
		@return float: The text scale.
		*/
		virtual float getTextScale() const override;
		/*
		Get the color used for drawing text on child buttons of this carousel.
		@return const glm::vec4&: The text color.
		*/
		virtual const glm::vec4& getTextColor() const override;
		/*
		Set whether this carousel is enabled for user input.
		@param bool enabled: Whether this carousel should be enabled.
		*/
		virtual void setEnabled(bool) override;

	protected:
		// The set of values of this carousel.
		std::vector<std::string> m_values;
		// The index of the current value of this carousel.
		unsigned int m_valueIndex = 0;
		// The button to change this carousel's value to the previous one.
		Button m_prevButton;
		// The button to change this carousel's value to the text one.
		Button m_nextButton;
		// The horizontal alignment of this carousel's value.
		Alignment m_horizontalAlignment = AC_ALIGN_NONE;
		// The vertical alignment of this carousel's value.
		Alignment m_verticalAlignment = AC_ALIGN_NONE;
		// The alignments of the buttons about this carousel's background.
		Alignment m_buttonAlignment = AC_ALIGN_NONE;

		/*
		Do not set the shader for this carousel (required override).
		@param Shader& shader: A reference to the shader not to set.
		*/
		virtual void setShader(Shader&) override;
		/*
		Do not set the font for this carousel (required override).
		@param const std::string& fontFileName: The file name of the font not
		to set.
		*/
		virtual void setFontFileName(const std::string&) override;
		/*
		Do not set the text scale for this carousel (required override).
		@param float textScale: The text scale not to set.
		*/
		virtual void setTextScale(float) override;
		/*
		Do not set the text color for this carousel (required override).
		@param const glm::vec4& textColor: The text color not to set.
		*/
		virtual void setTextColor(const glm::vec4&) override;
		/*
		Draw this carousel's graphics.
		*/
		virtual void draw() override;
		/*
		Process user input to this carousel.
		*/
		virtual void processInput() override;
		/*
		Update this carousel's logic.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this carousel's memory.
		*/
		virtual void destroy() override;
	};

	// Text area UI component.
	class TextArea : public TextBox {
	public:
		/*
		Initialize this text area's memory.
		@param const glm::vec3& position: The normalized position of this text
		area.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		text area.
		@param unsigned int lineCount: The number of lines to appear in this
		text area.
		@param const std::string& textureFileName: The file name of the texture
		of this text area's background.
		@param const std::string& labelText: The text to appear in the label for
		this text area.
		@param const glm::vec2& cursorDimensions: The dimensions of the cursor
		for this text area in pixels.
		@param const std::string& cursorTextureFileName: The file name of the
		texture of this text area's cursor.
		@param const std::string& text: The initial text to appear in this text
		area.
		@param int maxCharacters: The maximum number of cahracters allowed in
		this text area or -1 for infinite.
		@param const std::string& allowedCharacters: The set of characters
		allowed in this text area or empty string for all characters allowed.
		@param Alignment labelAlignment: The alignment of this text area's label
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this text area's label text in its bounds.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		text area's label text in its bounds.
		@param Alignment cursorAlignment: The vertical alignment of the cursor
		on its line.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, unsigned int,
			const std::string&, const std::string&, const glm::vec2&,
			const std::string&, const std::string&, int = -1,
			const std::string& = "", Alignment = AC_ALIGN_TOP,
			Alignment = AC_ALIGN_LEFT, Alignment = AC_ALIGN_BOTTOM,
			Alignment = AC_ALIGN_CENTER);
		/*
		Set the normalized position of this text area.
		@param const glm::vec3& position: The new normalized position.
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Set the text to appear in this text area.
		@param const std::string& text: The new text for this text area.
		*/
		virtual void setText(const std::string&) override;
		/*
		Get the number of lines appearing in this text area.
		@return int: The line count.
		*/
		virtual int getLineCount() const;
		/*
		Get the current line of this text area's content to appear at the top of
		the text area.
		@return int: The current top line index.
		*/
		virtual int getTopLine() const;
		/*
		Set the current line of this text area's content to appear at the top of
		the text area.
		@param int topLine: The new top line index.
		@return bool: Whether the new top line was set (valid).
		*/
		virtual bool setTopLine(int);
		/*
		* A text character has been typed.
		*/
		void charTyped(char) override;

	protected:
		// The number of lines in this text area.
		int m_lineCount = 0;
		// The line index to appear at the top of this text area.
		int m_topLine = 0;

		/*
		Draw this text area's graphics.
		*/
		virtual void draw() override;
		/*
		Process user input to this text area.
		*/
		virtual void processInput() override;
		/*
		Update this text area's physics.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this text area's memory.
		*/
		virtual void destroy() override;
		/*
		* Set the top line and horizontal text position so that the cursor
		* appears inside the background of this text area.
		*/
		void putCursorInView();

	private:
		/*
		Get the lines in this text area's content.
		@return unsigned int: The content lines.
		*/
		std::vector<std::string> getContentLines() const;
	};

	// Password box UI component.
	class PasswordBox : public TextBox {
	public:
		/*
		Initialize this text box's memory.
		@param const glm::vec3& position: The normalized position of this text
		box.
		@param const glm::vec2& dimensions: The normalized position of this
		text box.
		@param const std::string& textureFileName: The file name for the
		texture of this text box's background.
		@param const std::string& labelText: The text to appear in the label
		for this text box.
		@param const glm::vec2& cursorDimensions: The dimensions of the cursor
		for this text box in pixels.
		@param const std::string& cursorTextureFileName: The file name for the
		texture of this text box's cursor.
		@param const std::string& text: The initial text to appear in this text
		box.
		@param Alignment labelAlignment: The alignment of this text box's label
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this text box's label text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		text box's label text in its bounding box.
		@param Alignment cursorAlignment: The vertical alignment of this text
		box's cursor in its background.
		@param Alignment verticalAlignment: The vertical alignment of this text
		box's text in its background.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&, const glm::vec2&,
			const std::string&, const std::string&, Alignment = AC_ALIGN_TOP,
			Alignment = AC_ALIGN_LEFT, Alignment = AC_ALIGN_BOTTOM,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_CENTER);

	protected:
		/*
		Draw this password box's graphics.
		*/
		virtual void draw() override;
		/*
		Updates this password box's physics.
		*/
		virtual void update(float) override;
		/*
		Free this password box's memory.
		*/
		virtual void destroy() override;
	};

	// List box UI component.
	class ListBox : public Label, UIListener {
	public:
		// Event ID for list box value changed.
		const static unsigned int ValueSetEvent = 0;

		/*
		Initialize this list box's memory.
		@param const glm::vec3& position: The normalized position of this list
		box.
		@param const glm::vec2& dimensions: The normalized dimensions of this
		list box.
		@param const std::string& textureFileName: The file name for the texture
		of this list box's background.
		@param const std::string& labelText: The text to appear in the label for
		this list box.
		@param const std::vector<std::string>& values: The values to appear in
		this list box.
		@param unsigned int valueCount: The maximum number of values to appear
		in this list box at a time.
		@param const std::string& elementTextureFileName: The file name for the
		texture to appear as the background of values of this list box.
		@param Alignment labelAlignment: The alignment of this list box's label
		about its background.
		@param Alignment horizontalLabelAlignment: The horizontal alignment of
		this list box's label text in its bounding box.
		@param Alignment verticalLabelAlignment: The vertical alignment of this
		list box's label text in its bounding box.
		@param Alignment horizontalAlignment: The horizontal alignment of the
		text appearing in this list box's elements.
		@param Alignment verticalAlignment: The vertical alignment of the text
		appearing in this list box's elements.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			const std::vector<std::string>&, unsigned int, const std::string&,
			Alignment = AC_ALIGN_TOP, Alignment = AC_ALIGN_LEFT,
			Alignment = AC_ALIGN_CENTER, Alignment = AC_ALIGN_LEFT,
			Alignment = AC_ALIGN_CENTER);
		/*
		Set this list box's normalized positon.
		@param const glm::vec3& position: The new normalized position and depth
		for this list box.
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Set this list box's normalized dimensions.
		@param const glm::vec2& dimensions: The new normalized dimensions for
		this list box.
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Get the set of possible values for this list box.
		@return std::vector<std::string>: The set of possible values.
		*/
		virtual std::vector<std::string> getValues() const;
		/*
		Get the current value for this list box.
		@return const std::string: The current value.
		*/
		virtual const std::string getValue() const;
		/*
		Set the current value for this list box, will add to the set of
		possible values if not already present.
		@param const std::string& value: The new value.
		*/
		virtual void setValue(const std::string&);
		/*
		Get the current index of this list box.
		@return int: The index of the current value in the list of possible
		values or -1 for none.
		*/
		virtual int getValueIndex() const;
		/*
		Get the index of a given value in this list box.
		@param const std::string& value: The value to test the index of.
		@return int: The index of the value in the list of possible values or
		-1 if the value was not found.
		*/
		virtual int getValueIndex(const std::string&) const;
		/*
		Set the current index of this list box.
		@param unsigned int index: The new index.
		@return bool: Whether the new index was in range.
		*/
		virtual bool setValueIndex(unsigned int);
		/*
		Add a value to this list box.
		@param const std::string& valueText: The text to appear on this new
		value.
		@param int index: The index in the list to add this value at (-1 for
		end).
		*/
		virtual void addValue(const std::string&, int = -1);
		/*
		Remove the first instance of an value from this list box.
		@param const std::string& value: The text appearing on the value
		to remove.
		@return bool: Whether the value was found and removed.
		*/
		virtual bool removeValue(const std::string&);
		/*
		Remove a value by index from this list box.
		@param unsigned int index: The index in the list of possible values to
		remove.
		@return bool: Whether the index was in range and the value was removed.
		*/
		virtual bool removeValue(unsigned int);
		/*
		Get the maximum number of values which can appear in this list box.
		@return unsigned int: The number of values.
		*/
		virtual unsigned int getValueCount() const;
		/*
		Get the index of the first value appearing in this list box.
		@return unsigned int: The top index.
		*/
		virtual unsigned int getTopIndex() const;
		/*
		Set the index of the first value to appear in this list box.
		@param unsigned int topIndex: The new top index.
		@return bool: Whether the new top index was in range.
		*/
		virtual bool setTopIndex(unsigned int);
		/*
		Get the horizontal alignment of this list box's values.
		@return Alignment: The horizontal alignment of this list box's values.
		*/
		virtual Alignment getHorizontalAlignment() const;
		/*
		Set the horizontal alignment of this list box's values.
		@param Alignment horizontalAlignment: The new horizontal alignment of
		this list box's values.
		*/
		virtual void setHorizontalAlignment(Alignment);
		/*
		Get the vertical alignment of this list box's values.
		@return Alignment: The vertical alignment of this list box's values.
		*/
		virtual Alignment getVerticalAlignment() const;
		/*
		Set the vertical alignment of this list box's values.
		@param Alignment verticalAlignment: The new vertical alignment of this
		list box's values.
		*/
		virtual void setVerticalAlignment(Alignment);
		/*
		A child switch has triggered an event.
		@param int cID: The ID number of the button which triggered the event.
		@param int eID: The ID number of the event type.
		*/
		virtual void componentEvent(int, int) override;
		/*
		Get the shader used to draw child switches of this list box.
		@return Shader&: A reference to the shader.
		*/
		virtual Shader& getShader() override;
		/*
		Get the font used to draw child switches of this list box.
		@return const std::string&: The file name of the font.
		*/
		virtual const std::string& getFontFileName() const override;
		/*
		Get the scale used for drawing text on child switches of this list box.
		@return float: The text scale.
		*/
		virtual float getTextScale() const override;
		/*
		Get the color used for drawing text on child switches of this list box.
		@return const glm::vec4&: The text color.
		*/
		virtual const glm::vec4& getTextColor() const override;
		/*
		Set whether this list box is enabled for user input.
		@param bool enabled: Whether this list box should be enabled.
		*/
		virtual void setEnabled(bool) override;

	protected:
		// The file name of the texture for the elements in this list box.
		std::string m_elementTextureFileName = "";
		// The elements of this list box.
		std::vector<Switch> m_elements;
		// The next ID number to assign to a new element of this list box.
		int m_nextID = 0;
		// The maximum number of values to appear in this list at a time.
		unsigned int m_valueCount = 0;
		// The index of the first value to appear in this list.
		unsigned int m_topIndex = 0;
		// Timer for scrolling.
		float m_timer = 0.0f;
		// The horizontal alignment of this list box's values.
		Alignment m_horizontalAlignment = AC_ALIGN_NONE;
		// The vertical alignment of this list box's values.
		Alignment m_verticalAlignment = AC_ALIGN_NONE;

		/*
		Do not set the shader for this carousel (required override).
		@param Shader& shader: A reference to the shader not to set.
		*/
		virtual void setShader(Shader&) override;
		/*
		Do not set the font for this carousel (required override).
		@param const std::string& fontFileName: The file name of the font not
		to set.
		*/
		virtual void setFontFileName(const std::string&) override;
		/*
		Do not set the text scale for this carousel (required override).
		@param float textScale: The text scale not to set.
		*/
		virtual void setTextScale(float) override;
		/*
		Do not set the text color for this carousel (required override).
		@param const glm::vec4& textColor: The text color not to set.
		*/
		virtual void setTextColor(const glm::vec4&) override;
		/*
		Draw this list box's graphics.
		*/
		virtual void draw() override;
		/*
		Process user input to this list box.
		*/
		virtual void processInput() override;
		/*
		Update this list box's physics.
		@param float dt: The number of frames elapsed since the last update.
		*/
		virtual void update(float) override;
		/*
		Free this list box's memory.
		*/
		virtual void destroy() override;
	};
}

#endif