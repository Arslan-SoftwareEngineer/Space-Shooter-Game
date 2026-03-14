#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float speedY;
    float speedX; 

    // Constructor
    Bullet(std::string png_path, float startX, float startY, float sY, float sX = 0) {
        if (!texture.loadFromFile(png_path)) {
            std::cout << "Error loading bullet: " << png_path << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(startX, startY);
        
        speedY = sY;
        speedX = sX;
    }

    void move() {
        sprite.move(speedX, speedY); 
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }

    bool isOffScreen() {
        return sprite.getPosition().y < -50 || sprite.getPosition().y > 850 || 
               sprite.getPosition().x < -50 || sprite.getPosition().x > 850;
    }
};