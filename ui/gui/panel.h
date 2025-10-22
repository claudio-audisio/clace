#pragma once
#include <raylib.h>
#include <string>
#include <vector>


class Panel {
public:
    Panel() = default;
    ~Panel() = default;

    Vector2 position;
    Vector2 size;
    Font font;
    int lineSpacing = 20;
    std::vector<std::string> text;

    void init(Vector2 position, Vector2 size, const Font& font);
    void draw();
    void write(const std::string& text);
    void clean();
};
