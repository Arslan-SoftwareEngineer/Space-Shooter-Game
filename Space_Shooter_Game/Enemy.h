#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
public:
    sf::Clock shootTimer;
    float fireInterval;
    std::string type; // "alpha", "beta", "gamma"

    // Constructor accepts type to determine fire rate
    Enemy(std::string png_path, float startX, float startY, std::string enemyType) 
        : GameObject(png_path) 
    {
        sprite.setPosition(startX, startY);
        sprite.setScale(0.5f, 0.5f); 
        type = enemyType;

        // PDF Requirements for Fire Intervals:
        if (type == "alpha") fireInterval = 5.0f;
        else if (type == "beta") fireInterval = 3.0f;
        else if (type == "gamma") fireInterval = 2.0f;
        else fireInterval = 3.0f; // Default

        // Randomize start so they don't shoot in sync
        float randomOffset = (rand() % 100) / 100.0f; 
        shootTimer.restart(); 
    }

    void move() override {
        // Simple downward movement
        sprite.move(0, 0.5f); 
    }

    bool shouldFire() {
        if (shootTimer.getElapsedTime().asSeconds() > fireInterval) {
            shootTimer.restart();
            return true;
        }
        return false;
    }

    bool isOffScreen() {
        return sprite.getPosition().y > 800; 
    }
};