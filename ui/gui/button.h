#pragma once
#include <raylib.h>
#include <string>


class Button {
    public:
    Button() = default;
    ~Button() = default;

    Vector2 position;
    Vector2 size;
    Color bgColor;
    Color textColor;
    std::string text;
    Font font;
    Rectangle rectangle;
    Texture2D texture;
    float scale;
    bool hasTexture;

    void init(Vector2 position, Vector2 size, Color bgColor, Color textColor, const std::string& text, const Font& font);
    void init(Vector2 position, Vector2 size, Color bgColor, Color textColor, const Texture2D& texture, float scale);
    void draw() const;
    bool isPressed(Vector2 mousePos) const;

};

