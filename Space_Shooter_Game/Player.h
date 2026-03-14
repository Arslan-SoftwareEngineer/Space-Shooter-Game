#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Player : public GameObject {
public:
    int lives;
    int health;
    int maxHealth;
    float speed = 5.0f;

    Player(std::string png_path) : GameObject(png_path) {
        sprite.setScale(0.8f, 0.8f);
        lives = 3;
        maxHealth = 100;
        health = maxHealth;
        resetPosition();
    }

    void resetPosition() {
        sprite.setPosition(390 - (sprite.getGlobalBounds().width / 2), 650);
    }

    void move() override {
        // --- 1. FREE MOVEMENT ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            sprite.move(-speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            sprite.move(speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            sprite.move(0, -speed); 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            sprite.move(0, speed);
        }

        // --- 2. WARP LOGIC ---
        // Horizontal
        if (sprite.getPosition().x < -50) sprite.setPosition(780, sprite.getPosition().y);
        else if (sprite.getPosition().x > 780) sprite.setPosition(-50, sprite.getPosition().y);

        // Vertical
        if (sprite.getPosition().y < -50) sprite.setPosition(sprite.getPosition().x, 780);
        else if (sprite.getPosition().y > 780) sprite.setPosition(sprite.getPosition().x, -50);
    }

    Bullet* fire() {
        float x = sprite.getPosition().x + (sprite.getGlobalBounds().width / 2) - 5;
        float y = sprite.getPosition().y + sprite.getGlobalBounds().height; 
        return new Bullet("img/PNG/Lasers/laserBlue01.png", x, y, -10.0f, 0); 
    }
};