#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Simple class for expanding fire rings
class Explosion {
public:
    sf::CircleShape shape;
    float life; // 1.0 (full) to 0.0 (gone)

    Explosion(float x, float y) {
        shape.setPosition(x, y);
        shape.setRadius(5.0f);
        shape.setFillColor(sf::Color(255, 165, 0, 255)); // Orange
        shape.setOrigin(5, 5); // Center origin
        life = 1.0f;
    }

    void update() {
        life -= 0.05f; // Fade out speed
        float r = shape.getRadius() + 2.0f; // Expand speed
        shape.setRadius(r);
        shape.setOrigin(r, r); // Keep centered
        
        // Fade alpha
        sf::Color c = shape.getFillColor();
        c.a = static_cast<sf::Uint8>(255 * life);
        shape.setFillColor(c);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    bool isFinished() { return life <= 0; }
};

// Simple class for floating combat text (e.g. "+1 LIFE")
class FloatingText {
public:
    sf::Text text;
    float life;

    FloatingText(std::string str, float x, float y, sf::Font& font, sf::Color color) {
        text.setFont(font);
        text.setString(str);
        text.setPosition(x, y);
        text.setCharacterSize(20);
        text.setFillColor(color);
        life = 1.0f;
    }

    void update() {
        life -= 0.02f;
        text.move(0, -1.0f); // Float up
        
        // Fade
        sf::Color c = text.getFillColor();
        c.a = static_cast<sf::Uint8>(255 * life);
        text.setFillColor(c);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(text);
    }

    bool isFinished() { return life <= 0; }
};