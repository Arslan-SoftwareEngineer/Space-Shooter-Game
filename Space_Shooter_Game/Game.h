#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <vector>
#include <iostream>
#include <fstream> 
#include <algorithm> 
#include <cmath> 
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Dragon.h"   
#include "Monster.h"  
#include "Menu.h" 
#include "AddOn.h" 
#include "Explosion.h" 

enum GameState { MENU, PLAY, PAUSE, INSTRUCTIONS, GAME_OVER_STATE, CONFIRM_QUIT, WAVE_TRANSITION, BOSS_DEFEATED_SCREEN };

class Game {
public:
    sf::RenderWindow window;
    Player* p;
    std::vector<Bullet*> bullets; 
    std::vector<Enemy*> enemies; 
    std::vector<AddOn*> addons;
    
    std::vector<Explosion*> explosions;
    std::vector<FloatingText*> floatTexts;

    Dragon* dragon = nullptr;
    Monster* monster = nullptr;
    Menu* menu; 

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Font font;
    sf::Text livesText, waveText, scoreText; 
    sf::Text instructionText; 

    // --- AUDIO SYSTEM ---
    sf::SoundBuffer shootBuff, destBuff, warnBuff, mSpawnBuff, mDieBuff, dSpawnBuff, dDieBuff;
    sf::SoundBuffer selectBuff, hitBuff, powerBuff, victoryBuff; 

    sf::Sound shootSnd, destSnd, warnSnd, mSpawnSnd, mDieSnd, dSpawnSnd, dDieSnd;
    sf::Sound selectSnd, hitSnd, powerSnd, victorySnd; 

    // --- UI VARIABLES ---
    sf::RectangleShape uiPanel; 
    sf::Text panelTitle;
    sf::Text panelSubText; 
    sf::RectangleShape buttonShape; 
    sf::Text buttonText;            

    // --- HUD VARIABLES ---
    sf::RectangleShape pHealthBarBG;
    sf::RectangleShape pHealthBarFG; 
    sf::Text pLivesText;

    // --- MENU NAVIGATION ---
    int pauseOption = 0; 
    int confirmOption = 1; 
    int gameOverOption = 0; 

    // --- TRANSITION VARIABLES ---
    sf::Text transitionMainText;
    sf::Text transitionSubText;
    sf::RectangleShape transitionOverlay;
    sf::Clock transitionTimer;
    int nextWaveNum = 1;
    float shakeTimer = 0.0f; 

    // --- BOSS DEFEATED ANIMATION ---
    sf::Clock bossDefeatedTimer;
    bool isDragonDefeated = false; 

    bool uiEnabled = false; 

    GameState currentState = MENU; 
    int wave = 1;
    long score = 0; 
    bool gameWon = false; 

    bool powerUpActive = false;
    bool fireModeActive = false;
    sf::Clock powerUpTimer;
    sf::Clock beamClock; 

    bool isRespawning = false;
    sf::Clock respawnTimer;
    sf::Vector2f lastDeathPos; 

    Game() {
        window.create(sf::VideoMode(780, 780), "Space Shooter");
        window.setFramerateLimit(60); 

        if (bgTexture.loadFromFile("img/background.jpg")) {
            bgSprite.setTexture(bgTexture);
            bgSprite.setScale(780.0f / bgTexture.getSize().x, 780.0f / bgTexture.getSize().y);
        }
        
        menu = new Menu(780, 780);
        p = new Player("img/player_ship.png");

        if (!font.loadFromFile("fonts/font.ttf")) {
            std::cout << "[[ WARNING ]]: Could not load 'fonts/font.ttf'" << std::endl;
            uiEnabled = false;
        } else {
            uiEnabled = true;
            setupUI();
        }

        loadSounds(); 
    }

    void loadSounds() {
        if (shootBuff.loadFromFile("sounds/shoot.wav")) shootSnd.setBuffer(shootBuff);
        if (destBuff.loadFromFile("sounds/explosion.wav")) destSnd.setBuffer(destBuff);
        if (warnBuff.loadFromFile("sounds/warning.wav")) warnSnd.setBuffer(warnBuff);
        
        if (mSpawnBuff.loadFromFile("sounds/monster_spawn.wav")) mSpawnSnd.setBuffer(mSpawnBuff);
        if (mDieBuff.loadFromFile("sounds/monster_death.wav")) mDieSnd.setBuffer(mDieBuff);
        if (dSpawnBuff.loadFromFile("sounds/dragon_spawn.wav")) dSpawnSnd.setBuffer(dSpawnBuff);
        if (dDieBuff.loadFromFile("sounds/dragon_death.wav")) dDieSnd.setBuffer(dDieBuff);

        if (selectBuff.loadFromFile("sounds/select.wav")) selectSnd.setBuffer(selectBuff);
        if (hitBuff.loadFromFile("sounds/hit.wav")) hitSnd.setBuffer(hitBuff);
        if (powerBuff.loadFromFile("sounds/powerup.wav")) powerSnd.setBuffer(powerBuff);
        
        if (victoryBuff.loadFromFile("sounds/victory.wav")) victorySnd.setBuffer(victoryBuff);

        mSpawnSnd.setLoop(true); 
        dSpawnSnd.setLoop(true); 

        shootSnd.setVolume(50);
        destSnd.setVolume(60);
        warnSnd.setVolume(80);
        selectSnd.setVolume(70);
        victorySnd.setVolume(100);
    }

    // --- AUDIO CONTROL FUNCTIONS ---
    void stopGameAudio() {
        warnSnd.stop();
        mSpawnSnd.stop();
        dSpawnSnd.stop();
    }

    void pauseGameAudio() {
        if (warnSnd.getStatus() == sf::Sound::Playing) warnSnd.pause();
        if (mSpawnSnd.getStatus() == sf::Sound::Playing) mSpawnSnd.pause();
        if (dSpawnSnd.getStatus() == sf::Sound::Playing) dSpawnSnd.pause();
    }

    void resumeGameAudio() {
        // Only resume warning if we are in transition AND it's a boss wave
        if (currentState == WAVE_TRANSITION && (nextWaveNum == 4 || nextWaveNum == 8)) warnSnd.play();
        
        // Only resume Boss music if they are alive
        if (monster != nullptr) mSpawnSnd.play();
        if (dragon != nullptr) dSpawnSnd.play();
    }
    // ------------------------------------------------

    void setupUI() {
        scoreText.setFont(font); scoreText.setCharacterSize(24); scoreText.setPosition(20, 20); scoreText.setFillColor(sf::Color::Cyan); scoreText.setString("Score: 0");
        waveText.setFont(font); waveText.setCharacterSize(24); waveText.setPosition(650, 20); waveText.setFillColor(sf::Color::Yellow); waveText.setString("Wave: 1");
        
        pHealthBarBG.setSize(sf::Vector2f(200, 20)); pHealthBarBG.setFillColor(sf::Color(50, 50, 50)); pHealthBarBG.setOutlineThickness(2); pHealthBarBG.setOutlineColor(sf::Color::White); pHealthBarBG.setPosition(550, 740);
        pHealthBarFG.setSize(sf::Vector2f(200, 20)); pHealthBarFG.setFillColor(sf::Color::Green); pHealthBarFG.setPosition(550, 740);
        pLivesText.setFont(font); pLivesText.setCharacterSize(24); pLivesText.setFillColor(sf::Color::White); pLivesText.setPosition(550, 710); pLivesText.setString("Lives: 3");

        instructionText.setFont(font); 
        instructionText.setCharacterSize(22);
        instructionText.setString(
            "       MISSION BRIEFING\n\n"
            " CONTROLS:\n"
            "  [W/A/S/D]   : Navigation\n"
            "  [SPACE]     : Fire Weapons\n"
            "  [P]         : Pause System\n"
            "  [Q]         : Abort / Back\n\n"
            " INTEL:\n"
            "  * Survive 8 Waves of enemies.\n"
            "  * Bosses appear at Wave 4 & 8.\n\n"
            " POWER-UPS:\n"
            "  [Yellow] : Scatter Shot\n"
            "  [Red]    : Rapid Fire\n"
            "  [Green]  : +1 Life\n"
            "  [Purple] : DANGER! (-50 HP)"
        );
        instructionText.setPosition(180, 150); 
        instructionText.setFillColor(sf::Color::White);

        uiPanel.setSize(sf::Vector2f(600, 400)); uiPanel.setOrigin(300, 200); uiPanel.setPosition(390, 390); uiPanel.setFillColor(sf::Color(0, 0, 0, 240)); uiPanel.setOutlineThickness(4);
        panelTitle.setFont(font); panelTitle.setCharacterSize(60); panelTitle.setStyle(sf::Text::Bold);
        panelSubText.setFont(font); panelSubText.setCharacterSize(30);
        buttonText.setFont(font); buttonText.setCharacterSize(35);

        transitionMainText.setFont(font); transitionMainText.setCharacterSize(70); transitionMainText.setStyle(sf::Text::Bold);
        transitionSubText.setFont(font); transitionSubText.setCharacterSize(30);
        transitionOverlay.setSize(sf::Vector2f(780, 780)); transitionOverlay.setFillColor(sf::Color(0, 0, 0, 150)); 
    }

    void saveScore() {
        std::ofstream file("scores.txt", std::ios::app); if (file.is_open()) { file << "Player " << score << "\n"; file.close(); }
    }

    void drawButton(float x, float y, float w, float h, std::string text, sf::Color baseColor, bool isSelected) {
        buttonShape.setSize(sf::Vector2f(w, h)); buttonShape.setOrigin(w/2, h/2); buttonShape.setPosition(x, y); buttonShape.setOutlineThickness(isSelected ? 3 : 1);
        if (isSelected) { buttonShape.setFillColor(baseColor); buttonShape.setOutlineColor(sf::Color::White); buttonText.setFillColor(sf::Color::White); } 
        else { sf::Color dim = baseColor; dim.a = 100; buttonShape.setFillColor(dim); buttonShape.setOutlineColor(baseColor); buttonText.setFillColor(sf::Color(200, 200, 200)); }
        window.draw(buttonShape);
        buttonText.setString(text); sf::FloatRect b = buttonText.getLocalBounds(); buttonText.setOrigin(b.left + b.width/2.0f, b.top + b.height/2.0f); buttonText.setPosition(x, y); window.draw(buttonText);
    }

    void drawPremiumPanel(std::string title, sf::Color titleColor, std::string sub, sf::Color borderColor) {
        window.draw(transitionOverlay);
        uiPanel.setOutlineColor(borderColor);
        window.draw(uiPanel);

        panelTitle.setString(title); panelTitle.setFillColor(titleColor);
        panelTitle.setScale(1,1);
        sf::FloatRect tb = panelTitle.getLocalBounds(); panelTitle.setOrigin(tb.left + tb.width/2.0f, tb.top + tb.height/2.0f);
        
        sf::Text shadow = panelTitle; shadow.setFillColor(sf::Color::Black); shadow.setPosition(390 + 3, 300 + 3); 
        window.draw(shadow); panelTitle.setPosition(390, 300); window.draw(panelTitle);

        panelSubText.setString(sub); panelSubText.setFillColor(sf::Color::White);
        sf::FloatRect sb = panelSubText.getLocalBounds(); panelSubText.setOrigin(sb.left + sb.width/2.0f, sb.top + sb.height/2.0f);
        panelSubText.setPosition(390, 360); window.draw(panelSubText);
    }

    void triggerWaveTransition(int nextW) {
        nextWaveNum = nextW; currentState = WAVE_TRANSITION; transitionTimer.restart();
        shakeTimer = 0.0f;
        
        if (nextWaveNum == 4 || nextWaveNum == 8) {
            warnSnd.setLoop(true); 
            warnSnd.play();
        }
    }

    void triggerBossDefeated(bool isDragon) {
        isDragonDefeated = isDragon; currentState = BOSS_DEFEATED_SCREEN; bossDefeatedTimer.restart();
        if (isDragon) { dSpawnSnd.stop(); dDieSnd.play(); } 
        else { mSpawnSnd.stop(); mDieSnd.play(); }
    }

    void startWave() {
        stopGameAudio(); // Safety stop

        if (p) p->resetPosition(); 
        for (auto b : bullets) delete b; bullets.clear();
        for (auto e : enemies) delete e; enemies.clear();
        for (auto a : addons) delete a; addons.clear();
        if (dragon) { delete dragon; dragon = nullptr; }
        if (monster) { delete monster; monster = nullptr; }

        if (wave == 1) spawnShape("rectangle"); 
        else if (wave == 2) spawnShape("triangle"); 
        else if (wave == 3) spawnShape("cross"); 
        else if (wave == 4) { monster = new Monster("img/monster.png"); mSpawnSnd.play(); }
        else if (wave == 5) spawnShape("circle"); 
        else if (wave == 6) spawnShape("diamond"); 
        else if (wave == 7) spawnShape("heart"); 
        else if (wave == 8) { dragon = new Dragon("img/dragon.png"); dSpawnSnd.play(); }
        else { gameWon = true; currentState = GAME_OVER_STATE; }
    }

    void spawnShape(std::string shape) {
        std::string img = "img/PNG/Enemies/enemyBlack1.png";
        
        if (shape == "rectangle") for(int col=0; col<6; col++) enemies.push_back(new Enemy(img, 180 + col*70, 50, "alpha"));
        if (shape == "triangle") {

            for(int i=0; i<5; i++) enemies.push_back(new Enemy(img, 230 + i*70, 50, "beta"));
            for(int i=0; i<3; i++) enemies.push_back(new Enemy(img, 300 + i*70, 110, "beta"));
            for(int i=0; i<1; i++) enemies.push_back(new Enemy(img, 370 + i*70, 170, "beta"));
        }
        else if (shape == "cross") { for(int i=0; i<5; i++) enemies.push_back(new Enemy(img, 360, 50 + i*60, "gamma")); for(int i=0; i<2; i++) enemies.push_back(new Enemy(img, 220 + i*70, 170, "gamma")); for(int i=0; i<2; i++) enemies.push_back(new Enemy(img, 430 + i*70, 170, "gamma")); }
        else if (shape == "circle") { int count = 10; float centerX = 360, centerY = 200, radius = 150; for (int i = 0; i < count; i++) enemies.push_back(new Enemy(img, centerX + radius * std::cos(i * 0.628f), centerY + radius * std::sin(i * 0.628f), "alpha")); }
        else if (shape == "diamond") { enemies.push_back(new Enemy(img, 360, 50, "beta")); enemies.push_back(new Enemy(img, 280, 120, "beta")); enemies.push_back(new Enemy(img, 440, 120, "beta")); enemies.push_back(new Enemy(img, 200, 190, "beta")); enemies.push_back(new Enemy(img, 520, 190, "beta")); enemies.push_back(new Enemy(img, 280, 260, "beta")); enemies.push_back(new Enemy(img, 440, 260, "beta")); enemies.push_back(new Enemy(img, 360, 330, "beta")); }
        else if (shape == "heart") { std::vector<sf::Vector2f> points = {{300, 80}, {420, 80}, {240, 130}, {480, 130}, {240, 200}, {480, 200}, {300, 270}, {420, 270}, {360, 340}, {360, 150}}; for(auto p : points) enemies.push_back(new Enemy(img, p.x, p.y, "gamma")); }
    }

    void start() {
        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) window.close();
                
                if (currentState == MENU) {
                     if (e.type == sf::Event::KeyPressed) {
                         if (e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down) {
                             selectSnd.play();
                         }
                     }
                     
                     int c = menu->handleInput(window, e);
                     if (c == 1) { 
                        selectSnd.play(); 
                        wave=1; score=0; 
                        p->lives=3; 
                        p->health = p->maxHealth; 
                        p->resetPosition(); 
                        gameWon = false; isRespawning = false; triggerWaveTransition(1); 
                     }
                     else if (c == 2) { selectSnd.play(); currentState = INSTRUCTIONS; }
                     else if (c == 3) { selectSnd.play(); window.close(); }
                }
                
                else if (e.type == sf::Event::KeyPressed) {
                    if (currentState == PLAY) {
                        if (e.key.code == sf::Keyboard::P) { 
                            selectSnd.play(); 
                            currentState = PAUSE; 
                            pauseOption = 0; 
                            pauseGameAudio(); // --- PAUSE AUDIO ---
                        }
                        if (e.key.code == sf::Keyboard::Q) { 
                            selectSnd.play(); 
                            currentState = CONFIRM_QUIT; 
                            confirmOption = 1; 
                            pauseGameAudio(); // --- PAUSE AUDIO ---
                        }
                    }
                    else if (currentState == PAUSE) {
                        if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right) { selectSnd.play(); pauseOption = (pauseOption == 0) ? 1 : 0; }
                        if (e.key.code == sf::Keyboard::Return) {
                            selectSnd.play();
                            if (pauseOption == 0) { 
                                currentState = PLAY; 
                                resumeGameAudio(); // --- RESUME AUDIO ---
                            } 
                            else { 
                                currentState = CONFIRM_QUIT; 
                                confirmOption = 1; 
                            } 
                        }
                        if (e.key.code == sf::Keyboard::P) { 
                            selectSnd.play(); 
                            currentState = PLAY; 
                            resumeGameAudio(); // --- RESUME AUDIO ---
                        }
                    }
                    else if (currentState == CONFIRM_QUIT) {
                        if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right) { selectSnd.play(); confirmOption = (confirmOption == 0) ? 1 : 0; }
                        if (e.key.code == sf::Keyboard::Return) {
                            selectSnd.play();
                            if (confirmOption == 0) { 
                                saveScore(); 
                                stopGameAudio(); // --- STOP AUDIO ---
                                currentState = MENU; 
                            } 
                            else { 
                                currentState = PAUSE; 
                                // Remain paused (do not resume yet)
                            }
                        }
                    }
                    else if (currentState == INSTRUCTIONS) {
                        if (e.key.code == sf::Keyboard::Escape || e.key.code == sf::Keyboard::Q) { selectSnd.play(); currentState = MENU; }
                    }
                    else if (currentState == GAME_OVER_STATE) {
                         if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right) { selectSnd.play(); gameOverOption = (gameOverOption == 0) ? 1 : 0; }
                         if (e.key.code == sf::Keyboard::Return) {
                             selectSnd.play();
                             if (gameOverOption == 0) { 
                                 wave=1; score=0; p->lives=3; 
                                 p->health = p->maxHealth; 
                                 p->resetPosition(); 
                                 gameWon = false; isRespawning = false; triggerWaveTransition(1); 
                             } 
                             else { currentState = MENU; }
                         }
                    }
                }
            }

            window.clear();
            if (currentState == MENU) menu->draw(window);
            else if (currentState == INSTRUCTIONS) { window.draw(bgSprite); window.draw(transitionOverlay); if(uiEnabled) window.draw(instructionText); }
            else if (currentState == PLAY) { updateGame(); drawGame(); }
            
            else if (currentState == WAVE_TRANSITION) {
                drawGame(); window.draw(transitionOverlay);
                float time = transitionTimer.getElapsedTime().asSeconds();
                bool isBoss = (nextWaveNum == 4 || nextWaveNum == 8);

                if (nextWaveNum == 4) { transitionMainText.setString("BOSS DETECTED"); transitionMainText.setFillColor(sf::Color::Red); transitionSubText.setString("Warning: Monster Incoming!"); }
                else if (nextWaveNum == 8) { transitionMainText.setString("FINAL BOSS"); transitionMainText.setFillColor(sf::Color(200, 0, 0)); transitionSubText.setString("Survive the Dragon!"); }
                else { transitionMainText.setString("WAVE " + std::to_string(nextWaveNum)); transitionMainText.setFillColor(sf::Color::Cyan); transitionSubText.setString("Get Ready"); }

                float xOffset = 0, yOffset = 0;
                if (isBoss) {
                    shakeTimer += 0.5f;
                    xOffset = std::sin(shakeTimer * 50) * 5; 
                    yOffset = std::cos(shakeTimer * 45) * 5;
                    if ((int)(time * 10) % 2 == 0) transitionOverlay.setFillColor(sf::Color(100, 0, 0, 150)); 
                    else transitionOverlay.setFillColor(sf::Color(0, 0, 0, 150));
                } else {
                    transitionOverlay.setFillColor(sf::Color(0, 0, 0, 150)); 
                }

                sf::FloatRect b1 = transitionMainText.getLocalBounds(); transitionMainText.setOrigin(b1.left + b1.width/2.0f, b1.top + b1.height/2.0f); transitionMainText.setPosition(390 + xOffset, 300 + yOffset); window.draw(transitionMainText);
                sf::FloatRect b2 = transitionSubText.getLocalBounds(); transitionSubText.setOrigin(b2.left + b2.width/2.0f, b2.top + b2.height/2.0f); transitionSubText.setPosition(390 + xOffset, 380 + yOffset); window.draw(transitionSubText);

                if (time > 3.0f) { 
                    warnSnd.stop(); 
                    wave = nextWaveNum; startWave(); currentState = PLAY; 
                }
            }
            
            else if (currentState == BOSS_DEFEATED_SCREEN) {
                drawGame(); 
                float time = bossDefeatedTimer.getElapsedTime().asSeconds();
                int alpha = 100 + (std::sin(time * 3.0f) * 50); 
                transitionOverlay.setFillColor(sf::Color(0, 100, 0, alpha)); 
                window.draw(transitionOverlay);

                std::string titleStr = isDragonDefeated ? "DRAGON DEFEATED" : "MONSTER DEFEATED";
                std::string subStr = isDragonDefeated ? "Galaxy Saved!" : "Sector Secure";

                panelTitle.setCharacterSize(45); 
                drawPremiumPanel(titleStr, sf::Color::Yellow, subStr, sf::Color::Green);
                panelTitle.setCharacterSize(60); 

                if (bossDefeatedTimer.getElapsedTime().asSeconds() > 3.0f) {
                    transitionOverlay.setFillColor(sf::Color(0, 0, 0, 150)); 
                    if (isDragonDefeated) { 
                        gameWon = true; saveScore(); currentState = GAME_OVER_STATE; 
                        dDieSnd.stop();
                        victorySnd.play(); 
                    } 
                    else { triggerWaveTransition(5); }
                }
            }

            else if (currentState == PAUSE) { 
                drawGame(); drawPremiumPanel("PAUSED", sf::Color::Cyan, "System Halted", sf::Color::Cyan);
                drawButton(290, 420, 150, 60, "RESUME", sf::Color::Green, (pauseOption == 0));
                drawButton(490, 420, 150, 60, "QUIT", sf::Color::Red, (pauseOption == 1));
            }
            else if (currentState == CONFIRM_QUIT) { 
                drawGame(); drawPremiumPanel("QUIT GAME?", sf::Color::Red, "Are you sure?", sf::Color::Red);
                drawButton(290, 420, 150, 60, "YES", sf::Color::Red, (confirmOption == 0));
                drawButton(490, 420, 150, 60, "NO", sf::Color::Green, (confirmOption == 1));
            }
            else if (currentState == GAME_OVER_STATE) { 
                if(uiEnabled) {
                    window.draw(bgSprite); window.draw(transitionOverlay);
                    sf::Color themeColor = gameWon ? sf::Color::Green : sf::Color::Red; std::string title = gameWon ? "VICTORY!" : "GAME OVER";
                    uiPanel.setOutlineColor(themeColor); window.draw(uiPanel);
                    panelTitle.setString(title); panelTitle.setFillColor(themeColor); panelTitle.setCharacterSize(60); panelTitle.setScale(1,1); 
                    sf::FloatRect tb = panelTitle.getLocalBounds(); panelTitle.setOrigin(tb.left + tb.width/2.0f, tb.top + tb.height/2.0f); panelTitle.setPosition(390, 280); window.draw(panelTitle);
                    sf::Text info; info.setFont(font); info.setString("Final Score: " + std::to_string(score)); info.setCharacterSize(30); info.setFillColor(sf::Color::White);
                    sf::FloatRect ib = info.getLocalBounds(); info.setOrigin(ib.left + ib.width/2.0f, ib.top + ib.height/2.0f); info.setPosition(390, 340); window.draw(info);
                    drawButton(290, 420, 180, 60, "RESTART", sf::Color::Green, (gameOverOption == 0));
                    drawButton(490, 420, 180, 60, "QUIT", sf::Color::Red, (gameOverOption == 1));
                }
            }
            window.display();
        }
    }

    void updateGame() {
        if (!p) return;
        for(int i=0; i<explosions.size(); i++) { explosions[i]->update(); if(explosions[i]->isFinished()) { delete explosions[i]; explosions.erase(explosions.begin()+i); i--; } }
        for(int i=0; i<floatTexts.size(); i++) { floatTexts[i]->update(); if(floatTexts[i]->isFinished()) { delete floatTexts[i]; floatTexts.erase(floatTexts.begin()+i); i--; } }

        if (isRespawning) {
            if (respawnTimer.getElapsedTime().asSeconds() > 1.0f) { isRespawning = false; p->sprite.setPosition(lastDeathPos); p->sprite.setColor(sf::Color::White); } else return; 
        }

        if (!dragon && !monster && enemies.empty()) { if (wave != 4 && wave != 8) { triggerWaveTransition(wave + 1); } return; }
        if ((powerUpActive || fireModeActive) && powerUpTimer.getElapsedTime().asSeconds() > 5.0f) { powerUpActive = false; fireModeActive = false; p->sprite.setColor(sf::Color::White); }

        if(uiEnabled) {
            pLivesText.setString("Lives: " + std::to_string(p->lives)); 
            scoreText.setString("Score: " + std::to_string(score)); 
            waveText.setString("Wave: " + std::to_string(wave));
            float hpPercent = (float)p->health / p->maxHealth; if(hpPercent < 0) hpPercent = 0; pHealthBarFG.setSize(sf::Vector2f(200 * hpPercent, 20));
        }

        p->move();
        
        float shipCenter = p->sprite.getPosition().x + (p->sprite.getGlobalBounds().width / 2) - 5; 
        float shipTop = p->sprite.getPosition().y;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (powerUpActive) { 
                if (beamClock.getElapsedTime().asSeconds() > 0.1f) { 
                    float angles[] = {0, -1.5f, 1.5f, -3.0f, 3.0f, -5.0f, 5.0f};
                    for(float a : angles) bullets.push_back(new Bullet("img/PNG/Lasers/laserBlue01.png", shipCenter, shipTop, -10.0f, a));
                    beamClock.restart();
                    shootSnd.play(); 
                } 
            }
            else { 
                static sf::Clock shootTimer; 
                float cooldown = fireModeActive ? 0.15f : 0.2f; 
                if (shootTimer.getElapsedTime().asSeconds() > cooldown) { 
                    float speed = fireModeActive ? -12.0f : -10.0f; 
                    Bullet* b = new Bullet("img/PNG/Lasers/laserBlue01.png", shipCenter, shipTop, speed, 0); 
                    if (fireModeActive) b->sprite.setColor(sf::Color::Red); 
                    bullets.push_back(b); 
                    shootTimer.restart();
                    shootSnd.play(); 
                } 
            }
        }

        if (dragon) { dragon->move(); if (dragon->shouldFire()) { std::vector<Bullet*> shots = dragon->specialFire(); for (auto s : shots) bullets.push_back(s); } }
        if (monster) { monster->move(); if (monster->shouldFire()) { float mCenter = monster->sprite.getPosition().x + (monster->sprite.getGlobalBounds().width / 2); bullets.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", mCenter, monster->sprite.getPosition().y + 80, 10.0f, 0)); } }

        if (rand() % 600 == 0) { std::string types[] = {"powerup", "fire", "danger", "life"}; int idx = rand() % 4; AddOn* a = new AddOn("img/PNG/Power-ups/star_gold.png", rand()%700 + 40, -50, types[idx]); if (types[idx] == "powerup") a->sprite.setColor(sf::Color::Yellow); else if (types[idx] == "fire") a->sprite.setColor(sf::Color::Red); else if (types[idx] == "danger") a->sprite.setColor(sf::Color::Magenta); else if (types[idx] == "life") a->sprite.setColor(sf::Color::Green); addons.push_back(a); }

        for (int i=0; i<bullets.size(); i++) { bullets[i]->move(); if (bullets[i]->isOffScreen()) { delete bullets[i]; bullets.erase(bullets.begin()+i); i--; } }
        for (int i=0; i<enemies.size(); i++) { enemies[i]->move(); if (enemies[i]->shouldFire()) bullets.push_back(new Bullet("img/PNG/Lasers/laserRed01.png", enemies[i]->sprite.getPosition().x+20, enemies[i]->sprite.getPosition().y+50, 5.0f, 0)); if (enemies[i]->isOffScreen()) { delete enemies[i]; enemies.erase(enemies.begin()+i); i--; } }
        for (int i=0; i<addons.size(); i++) { addons[i]->move(); if (addons[i]->isOffScreen()) { delete addons[i]; addons.erase(addons.begin()+i); i--; } }

        checkCollisions();
    }

    void checkCollisions() {
        for(int i=0; i<addons.size(); i++) {
            if (p->getBounds().intersects(addons[i]->getBounds())) {
                std::string t = addons[i]->type; powerUpTimer.restart(); 
                
                if (t == "powerup") { powerSnd.play(); powerUpActive = true; p->sprite.setColor(sf::Color::Yellow); floatTexts.push_back(new FloatingText("BEAM!", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Yellow)); }
                else if (t == "fire") { powerSnd.play(); fireModeActive = true; floatTexts.push_back(new FloatingText("FIRE!", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Red)); }
                
                else if (t == "life") { 
                    if (p->lives < 3) {
                        powerSnd.play(); 
                        p->lives++; 
                        floatTexts.push_back(new FloatingText("+1 LIFE", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Green)); 
                    }
                } 
                
                else if (t == "danger") { 
                    if (!powerUpActive) { 
                        hitSnd.play(); 
                        p->health -= 50; 
                        floatTexts.push_back(new FloatingText("-50 HP", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Magenta)); 
                    } 
                }
                delete addons[i]; addons.erase(addons.begin()+i); i--;
            }
        }

        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i]->speedY < 0) { 
                bool bulletDestroyed = false; 
                if (dragon && bullets[i]->getBounds().intersects(dragon->getBounds())) { dragon->health--; explosions.push_back(new Explosion(bullets[i]->sprite.getPosition().x, bullets[i]->sprite.getPosition().y)); if (!fireModeActive) { delete bullets[i]; bullets.erase(bullets.begin()+i); i--; bulletDestroyed=true; } if (dragon->health <= 0) { delete dragon; dragon=nullptr; score+=1000; triggerBossDefeated(true); } if(bulletDestroyed) continue; }
                if (monster && bullets[i]->getBounds().intersects(monster->getBounds())) { monster->health--; explosions.push_back(new Explosion(bullets[i]->sprite.getPosition().x, bullets[i]->sprite.getPosition().y)); if (!fireModeActive) { delete bullets[i]; bullets.erase(bullets.begin()+i); i--; bulletDestroyed=true; } if (monster->health <= 0) { delete monster; monster=nullptr; score+=500; triggerBossDefeated(false); } if(bulletDestroyed) continue; }
                for (int j = 0; j < enemies.size(); j++) { 
                    if (bullets[i]->getBounds().intersects(enemies[j]->getBounds())) { 
                        explosions.push_back(new Explosion(enemies[j]->sprite.getPosition().x + 20, enemies[j]->sprite.getPosition().y + 20)); 
                        delete enemies[j]; enemies.erase(enemies.begin() + j); j--; 
                        score += 10; 
                        destSnd.play(); 
                        if (!fireModeActive) { delete bullets[i]; bullets.erase(bullets.begin() + i); i--; break; } 
                    } 
                }
            }
        }

        if (p != nullptr && !isRespawning) { 
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i]->speedY > 0) { 
                     if (bullets[i]->getBounds().intersects(p->getBounds())) {
                         delete bullets[i]; bullets.erase(bullets.begin() + i); i--;
                         if (!powerUpActive) {
                             hitSnd.play(); 
                             p->health -= 20; 
                             floatTexts.push_back(new FloatingText("-20 HP", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Red));
                             if (p->health <= 0) {
                                 p->lives--;
                                 if (p->lives <= 0) { 
                                     currentState = GAME_OVER_STATE; 
                                     saveScore(); 
                                     stopGameAudio(); // --- STOP AUDIO ON DEATH ---
                                 }
                                 else { isRespawning = true; respawnTimer.restart(); lastDeathPos = p->sprite.getPosition(); p->sprite.setPosition(-1000, -1000); p->health = 100; }
                             }
                         }
                     }
                }
            }
            for (int i = 0; i < enemies.size(); i++) {
                if (p->getBounds().intersects(enemies[i]->getBounds())) {
                    delete enemies[i]; enemies.erase(enemies.begin() + i); i--;
                    if (!powerUpActive) {
                         hitSnd.play(); 
                         p->health -= 30; 
                         floatTexts.push_back(new FloatingText("-30 HP", p->sprite.getPosition().x, p->sprite.getPosition().y, font, sf::Color::Red));
                         if (p->health <= 0) {
                             p->lives--;
                             if (p->lives <= 0) { 
                                 currentState = GAME_OVER_STATE; 
                                 saveScore(); 
                                 stopGameAudio(); // --- STOP AUDIO ON DEATH ---
                             }
                             else { isRespawning = true; respawnTimer.restart(); lastDeathPos = p->sprite.getPosition(); p->sprite.setPosition(-1000, -1000); p->health = 100; }
                         }
                    }
                }
            }
        }
    }

    void drawGame() {
        window.draw(bgSprite); 
        for (auto& b : bullets) b->draw(window);
        if (p && !isRespawning) p->draw(window);
        for (auto& e : enemies) e->draw(window);
        for (auto& a : addons) a->draw(window);
        for (auto& ex : explosions) ex->draw(window);
        for (auto& ft : floatTexts) ft->draw(window);
        if (dragon) dragon->draw(window); 
        if (monster) monster->draw(window); 
        
        if (uiEnabled) { 
            window.draw(waveText); 
            window.draw(scoreText);
            window.draw(pHealthBarBG);
            window.draw(pHealthBarFG);
            window.draw(pLivesText);
        }
    }
};