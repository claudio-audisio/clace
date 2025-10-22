//
// Created by claudio on 9/23/25.
//

#include "panel.h"

#include "guiConstants.h"

void Panel::write(const std::string& text) {
    this->text.push_back(text);
}

void Panel::init(const Vector2 position, const Vector2 size, const Font& font) {
    this->position = position;
    this->size = size;
    this->font = font;

}

void Panel::draw() {
    const int maxRows = 19;   // depends on font size, line spacing and panel height
    Vector2 row = {position.x + 10, position.y + 10};
    DrawRectangleV(position, size, RAYWHITE);
    int rowsToSkip = text.size() - maxRows;

    for (const std::string& line : text) {
        if (--rowsToSkip >= 0) {
            continue;
        }
        DrawTextEx(font, line.c_str(), row, 20, 1, BG_COLOR);
        row.y += lineSpacing;
    }
}

void Panel::clean() {
    text.clear();
}
