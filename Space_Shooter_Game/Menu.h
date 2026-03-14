#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Menu {
public:
    sf::Font font;
    std::vector<sf::Text> menuTexts;
    std::vector<sf::RectangleShape> menuButtons; // The button shapes
    int selectedItemIndex = 0;
    bool isReady = false;
    
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    Menu(float width, float height) {
        if (!font.loadFromFile("fonts/font.ttf")) {
            // Handle error
        } else {
            isReady = true;
        }

        // Load Background
        if (bgTexture.loadFromFile("img/background.jpg")) {
            bgSprite.setTexture(bgTexture);
            bgSprite.setScale(width / bgTexture.getSize().x, height / bgTexture.getSize().y);
            bgSprite.setColor(sf::Color(100, 100, 100)); // Darken background for menu
        }

        std::string options[] = { "PLAY GAME", "INSTRUCTIONS", "QUIT" };
        
        float startY = height / 2 - 50;
        float buttonWidth = 300;
        float buttonHeight = 60;

        for (int i = 0; i < 3; i++) {
            // 1. Create Button Shape
            sf::RectangleShape btn(sf::Vector2f(buttonWidth, buttonHeight));
            btn.setOrigin(buttonWidth / 2, buttonHeight / 2); // Center origin
            btn.setPosition(width / 2, startY + (i * 90)); // Stack vertically
            btn.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
            btn.setOutlineThickness(2);
            btn.setOutlineColor(sf::Color::Cyan);
            menuButtons.push_back(btn);

            // 2. Create Text
            sf::Text text;
            text.setFont(font);
            text.setString(options[i]);
            text.setCharacterSize(30);
            
            // Center text inside button
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);
            text.setPosition(btn.getPosition());
            
            menuTexts.push_back(text);
        }
        
        updateVisuals(); 
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bgSprite); 
        
        // Draw Title
        sf::Text title;
        title.setFont(font);
        title.setString("SPACE SHOOTER");
        title.setCharacterSize(60);
        
        // Center Title
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
        title.setPosition(window.getSize().x / 2.0f, 100);
        
        // Glow effect for title
        sf::Text shadow = title;
        shadow.setPosition(title.getPosition().x + 3, title.getPosition().y + 3);
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        
        title.setFillColor(sf::Color::Cyan);
        
        window.draw(shadow);
        window.draw(title);

        // Draw Buttons and Text
        for (int i = 0; i < menuButtons.size(); i++) {
            window.draw(menuButtons[i]);
            window.draw(menuTexts[i]);
        }
    }

    void MoveUp() {
        if (selectedItemIndex - 1 >= 0) {
            selectedItemIndex--;
            updateVisuals();
        }
    }

    void MoveDown() {
        if (selectedItemIndex + 1 < menuButtons.size()) {
            selectedItemIndex++;
            updateVisuals();
        }
    }

    // Handles highlighting logic
    void updateVisuals() {
        for (int i = 0; i < menuButtons.size(); i++) {
            if (i == selectedItemIndex) {
                // SELECTED: Brighter button, Yellow text
                menuButtons[i].setFillColor(sf::Color(50, 50, 50, 200)); 
                menuButtons[i].setOutlineColor(sf::Color::Yellow);
                menuTexts[i].setFillColor(sf::Color::Yellow);
            } else {
                // UNSELECTED: Darker button, White text
                menuButtons[i].setFillColor(sf::Color(0, 0, 0, 150));
                menuButtons[i].setOutlineColor(sf::Color::Cyan);
                menuTexts[i].setFillColor(sf::Color::White);
            }
        }
    }

    int handleInput(sf::RenderWindow& window, sf::Event& event) {
        if (!isReady) return 0;

        // 1. Mouse Hover
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        bool mouseHovering = false;
        for (int i = 0; i < menuButtons.size(); i++) {
            if (menuButtons[i].getGlobalBounds().contains(mousePosF)) {
                selectedItemIndex = i;
                mouseHovering = true;
                updateVisuals();
                
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    return i + 1; 
                }
            }
        }

        // 2. Keyboard
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) MoveUp();
            if (event.key.code == sf::Keyboard::Down) MoveDown();
            if (event.key.code == sf::Keyboard::Return) return selectedItemIndex + 1;
        }

        return 0;
    }
};