#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameObject {
public:
    sf::Texture texture;
    sf::Sprite sprite;

    GameObject(std::string png_path) {
        texture.loadFromFile(png_path);
        sprite.setTexture(texture);
    }

    // Virtual function: children classes (Player/Enemy) will implement their own logic
    virtual void move() = 0; 

    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    // --- ADDED THIS FUNCTION ---
    virtual sf::FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }
    
};