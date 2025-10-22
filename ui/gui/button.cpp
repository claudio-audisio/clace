#include "button.h"

#include "guiConstants.h"

void Button::init(const Vector2 position, const Vector2 size, const Color bgColor, const Color textColor, const std::string& text, const Font& font) {
    this->position = position;
    this->size = size;
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->text = text;
    this->font = font;
    this->rectangle = {position.x, position.y, size.x, size.y};
    this->hasTexture = false;
}

void Button::init(const Vector2 position, const Vector2 size, const Color bgColor, const Color textColor, const Texture2D& texture, const float scale) {
    this->position = position;
    this->size = size;
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->textColor = textColor;
    this->rectangle = {position.x, position.y, size.x, size.y};
    this->texture = texture;
    this->hasTexture = true;
    this->scale = scale;
}

void Button::draw() const {
    DrawRectangle(position.x, position.y, size.x, size.y, bgColor);

    if (hasTexture) {
        DrawTextureEx(texture, position, 0, scale, textColor);
    } else {
        DrawTextEx(font, text.c_str(), {position.x + 10, position.y}, BUTTON_FONT_SIZE, 1, textColor);
    }
}

bool Button::isPressed(const Vector2 mousePos) const {
    return CheckCollisionPointRec(mousePos, rectangle);
}
