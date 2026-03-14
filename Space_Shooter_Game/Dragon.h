#pragma once
#include "Enemy.h"
#include "Bullet.h"
#include <vector>

class Dragon : public GameObject {
public:
    int health;
    int maxHealth;
    sf::Clock fireTimer;

    // Movement
    float speedX;
    bool movingRight;

    // Health Bar
    sf::RectangleShape barBG;
    sf::RectangleShape barFG;

    Dragon(std::string png_path) : GameObject(png_path) {
        sprite.setPosition(300, 50); 
        sprite.setScale(1.2f, 1.2f); 
        
        maxHealth = 100; 
        health = maxHealth;
        
        speedX = 3.0f;
        movingRight = true;
        fireTimer.restart();

        barBG.setSize(sf::Vector2f(150, 15)); 
        barBG.setFillColor(sf::Color::Red);
        barFG.setSize(sf::Vector2f(150, 15));
        barFG.setFillColor(sf::Color::Green);
    }

    void move() override {
        if (movingRight) {
            sprite.move(speedX, 0);
            if (sprite.getPosition().x > 600) movingRight = false;
        } else {
            sprite.move(-speedX, 0);
            if (sprite.getPosition().x < 50) movingRight = true;
        }

        // Update Bar
        barBG.setPosition(sprite.getPosition().x, sprite.getPosition().y - 30);
        barFG.setPosition(sprite.getPosition().x, sprite.getPosition().y - 30);

        float hpPercent = (float)health / maxHealth;
        if (hpPercent < 0) hpPercent = 0;
        barFG.setSize(sf::Vector2f(150 * hpPercent, 15));
    }

    bool shouldFire() {
        if (fireTimer.getElapsedTime().asSeconds() > 1.5f) {
            fireTimer.restart();
            return true;
        }
        return false;
    }

    std::vector<Bullet*> specialFire() {
        std::vector<Bullet*> shots;
        float x = sprite.getPosition().x + 50;
        float y = sprite.getPosition().y + 100;

        // --- 5-WAY SHOOTING PATTERN ---
        // 1. Center (Down)
        shots.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", x, y, 8.0f, 0));       
        
        // 2. Inner Left (Slight Angle)
        shots.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", x, y, 7.0f, -2.5f));   
        
        // 3. Inner Right (Slight Angle)
        shots.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", x, y, 7.0f, 2.5f));    

        // 4. Outer Left (Wide Angle)
        shots.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", x, y, 6.0f, -5.0f));

        // 5. Outer Right (Wide Angle)
        shots.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", x, y, 6.0f, 5.0f));

        return shots;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
        window.draw(barBG);
        window.draw(barFG);
    }
};