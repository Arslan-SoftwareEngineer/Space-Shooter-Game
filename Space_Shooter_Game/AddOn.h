#pragma once
#include "GameObject.h"

class AddOn : public GameObject {
public:
    std::string type; // "powerup", "fire", "danger", "life"

    AddOn(std::string png_path, float startX, float startY, std::string addOnType) 
        : GameObject(png_path) 
    {
        sprite.setPosition(startX, startY);
        type = addOnType;
        sprite.setScale(0.7f, 0.7f);
    }

    void move() override {
        sprite.move(0, 3.0f); // Floats down
    }

    bool isOffScreen() {
        return sprite.getPosition().y > 800; 
    }
};