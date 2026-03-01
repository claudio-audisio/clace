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

void Button::draw2() const {
    // Hover detection
    bool hovered = CheckCollisionPointRec(GetMousePosition(), {position.x, position.y, size.x, size.y});

    Color currColor = hovered ? bgColor : BLANK;
    Color borderColor = bgColor;
    float borderThick = hovered ? 0 : 2;

    DrawRectangleRounded({position.x, position.y, size.x, size.y}, 0.2, 8, currColor);
    DrawRectangleRoundedLines({position.x, position.y, size.x, size.y}, 0.2, 8, borderColor);

    // Center text
    Vector2 textSize = MeasureTextEx(font, text.c_str(), BUTTON_FONT_SIZE, 1);
    Vector2 textPos = {
        position.x + (size.x - textSize.x) / 2,
        position.y + (size.y - textSize.y) / 2
    };

    Color currTextColor = hovered ? textColor : bgColor;

    if (hasTexture) {
        DrawTextureEx(texture, textPos, 0, scale, currTextColor);
    } else {
        DrawTextEx(font, text.c_str(), textPos, BUTTON_FONT_SIZE, 1, currTextColor);
    }
}

bool Button::isPressed(const Vector2 mousePos) const {
    return CheckCollisionPointRec(mousePos, rectangle);
}
