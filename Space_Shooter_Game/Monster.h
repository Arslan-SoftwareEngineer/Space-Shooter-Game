#pragma once
#include "Enemy.h"

class Monster : public GameObject {
public:
    int health;
    int maxHealth;
    float speedX;
    sf::Clock beamTimer;
    bool movingRight;

    // Health Bar
    sf::RectangleShape barBG;
    sf::RectangleShape barFG;

    Monster(std::string png_path) : GameObject(png_path) {
        // Dynamic Scaling
        float targetWidth = 250.0f;
        float scaleFactor = targetWidth / sprite.getLocalBounds().width;
        sprite.setScale(scaleFactor, scaleFactor);

        // Center Start Position
        float xPos = (780.0f - targetWidth) / 2.0f;
        sprite.setPosition(xPos, 50); 
        
        maxHealth = 40;
        health = maxHealth;
        
        speedX = 3.0f;
        movingRight = true;
        beamTimer.restart();

        // Setup Health Bar
        barBG.setSize(sf::Vector2f(targetWidth, 10)); 
        barBG.setFillColor(sf::Color::Red);
        barBG.setPosition(xPos, 30);

        barFG.setSize(sf::Vector2f(targetWidth, 10));
        barFG.setFillColor(sf::Color::Green);
        barFG.setPosition(xPos, 30);
    }

    void move() override {
        // --- MOVEMENT LOGIC RESTORED ---
        if (movingRight) {
            sprite.move(speedX, 0);
            if (sprite.getPosition().x > 530) movingRight = false; // Boundary check (Right)
        } else {
            sprite.move(-speedX, 0);
            if (sprite.getPosition().x < 0) movingRight = true; // Boundary check (Left)
        }

        // Update Health Bar Position to follow Monster
        barBG.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20);
        barFG.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20);
        
        // Update Health Bar Width
        float hpPercent = (float)health / maxHealth;
        if (hpPercent < 0) hpPercent = 0;
        
        // Match bar size to current monster size
        float currentWidth = sprite.getGlobalBounds().width;
        barFG.setSize(sf::Vector2f(currentWidth * hpPercent, 10));
    }

    bool shouldFire() {
        if (beamTimer.getElapsedTime().asSeconds() > 0.1f) {
            beamTimer.restart();
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
        window.draw(barBG);
        window.draw(barFG);
    }
};