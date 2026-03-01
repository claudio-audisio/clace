#include <sstream>

#include "panel.h"
#include "guiConstants.h"

void Panel::write(const std::string& text) {
    std::stringstream ss(text);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        this->text.push_back(line);

        if (this->text.size() > PANEL_MAX_ROWS) {
            this->text.erase(this->text.begin());
        }
    }
}

void Panel::init(const Vector2 position, const Vector2 size, const Font& font) {
    this->position = position;
    this->size = size;
    this->font = font;

}

void Panel::draw() {
    Vector2 row = {position.x + 10, position.y + 10};
    DrawRectangleV(position, size, RAYWHITE);

    for (const std::string& line : text) {
        DrawTextEx(font, line.c_str(), row, 16, 1, BG_COLOR);
        row.y += lineSpacing;
    }
}

void Panel::clean() {
    text.clear();
}
