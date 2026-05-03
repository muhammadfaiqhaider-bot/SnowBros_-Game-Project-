#pragma once
#include "PowerUps.h"
#include <SFML/Graphics.hpp>

class Star : public PowerUps
{
public:
    Star(float posX, float posY) : PowerUps(posX, posY, "Star")
    {
        isActive = true;
    }

    void updatePowerUp() override
    {
        PowerUps::updatePowerUp();
        if (y >= 560.f - 20.f)
        {
            y = 560.f - 20.f;
            velocityY = 0.f;
        }
    }

    void applyEffect(Player& player) override
    {
        // Grant score for collecting a star and show a pickup message
        player.addScore(200);
        player.showPowerupMessage(std::string("Star +200"), 120);
    }

    void DrawPowerUp(sf::RenderWindow& window) override
    {
        if (isActive && !isCollected)
        {
            sf::ConvexShape star(10);
            float sx = 14.f; // size
            star.setPoint(0, sf::Vector2f(0.f * sx, 0.38f * sx));
            star.setPoint(1, sf::Vector2f(0.35f * sx, 0.35f * sx));
            star.setPoint(2, sf::Vector2f(0.5f * sx, 0.f * sx));
            star.setPoint(3, sf::Vector2f(0.65f * sx, 0.35f * sx));
            star.setPoint(4, sf::Vector2f(1.f * sx, 0.38f * sx));
            star.setPoint(5, sf::Vector2f(0.75f * sx, 0.6f * sx));
            star.setPoint(6, sf::Vector2f(0.8f * sx, 1.f * sx));
            star.setPoint(7, sf::Vector2f(0.5f * sx, 0.78f * sx));
            star.setPoint(8, sf::Vector2f(0.2f * sx, 1.f * sx));
            star.setPoint(9, sf::Vector2f(0.25f * sx, 0.6f * sx));
            star.setFillColor(sf::Color::Yellow);
            star.setPosition(x, y);
            window.draw(star);
        }
    }
};
