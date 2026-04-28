#pragma once
#include <SFML/Graphics.hpp>
#include "Botom.h"
#include "FlyingFoogo.h"
#include "Tornado.h"
#include "PLayer.h"
#include "Nick.h"

class CollisionManager
{
public:

    // ---- Botom platform collision ----
    static void handleBotom(Botom& botom,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        botom.setOnGround(false);

        for (int i = 0; i < platformCount; i++)
        {
            sf::FloatRect botomBounds(
                botom.getPositionX(),
                botom.getPositionY(),
                40.f, 40.f
            );
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (botomBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;
                float botomBottom = botom.getPositionY() + 40.f;

                if (botomBottom >= platformTop &&
                    botom.getPositionY() < platformTop &&
                    botom.getVelocityY() >= 0)
                {
                    botom.setOnGround(true);
                    botom.snapToGround(platformTop - 39.99f);
                }
            }
        }

        // Wall check
        if (botom.getPositionX() <= 0 || botom.getPositionX() >= 560)
        {
            botom.setHitWall(true);
        }
    }

    // ---- FlyingFoogo platform collision ----
    static void handleFlyingFoogo(FlyingFoogaFoog& enemy,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        enemy.setOnGround(false);

        if (enemy.getIsFlying())
        {
            // Only ground platform during flight
            sf::FloatRect enemyBounds(
                enemy.getPositionX(),
                enemy.getPositionY(),
                40.f, 40.f
            );
            sf::FloatRect groundBounds = platforms[0].getGlobalBounds();

            if (enemyBounds.intersects(groundBounds))
            {
                float groundTop = platforms[0].getPosition().y;

                if (enemy.getPositionY() + 40.f <= groundTop + 10.f)
                {
                    enemy.setOnGround(true);
                    enemy.snapToGround(groundTop - 40.f);
                }
            }
        }
        else
        {
            // All platforms when walking
            for (int i = 0; i < platformCount; i++)
            {
                sf::FloatRect enemyBounds(
                    enemy.getPositionX(),
                    enemy.getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

                if (enemyBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[i].getPosition().y;

                    if (enemy.getPositionY() + 40.f <= platformTop + 10.f)
                    {
                        enemy.setOnGround(true);
                        enemy.snapToGround(platformTop - 40.f);
                    }
                }
            }
        }
    }

    // ---- Tornado platform collision ----
    static void handleTornado(Tornado& tornado,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        tornado.setOnGround(false);

        if (tornado.getIsFlying())
        {
            // Only ground platform during flight
            sf::FloatRect tornadoBounds(
                tornado.getPositionX(),
                tornado.getPositionY(),
                40.f, 40.f
            );
            sf::FloatRect groundBounds = platforms[0].getGlobalBounds();

            if (tornadoBounds.intersects(groundBounds))
            {
                float groundTop = platforms[0].getPosition().y;

                if (tornado.getPositionY() + 40.f <= groundTop + 10.f)
                {
                    tornado.setOnGround(true);
                    tornado.snapToGround(groundTop - 40.f);
                }
            }
        }
        else
        {
            for (int i = 0; i < platformCount; i++)
            {
                sf::FloatRect tornadoBounds(
                    tornado.getPositionX(),
                    tornado.getPositionY(),
                    40.f, 40.f
                );
                sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

                if (tornadoBounds.intersects(platformBounds))
                {
                    float platformTop = platforms[i].getPosition().y;
                    float tornadoBottom = tornado.getPositionY() + 40.f;

                    if (tornadoBottom >= platformTop &&
                        tornado.getPositionY() < platformTop &&
                        tornado.getVelocityY() >= 0)
                    {
                        tornado.setOnGround(true);
                        tornado.snapToGround(platformTop - 39.99f);
                    }
                }
            }
        }
    }

    // ---- Player platform collision ----
    static void handlePlayer(Nick& player,
        sf::RectangleShape platforms[],
        int platformCount)
    {
        player.setOnGround(false);

        for (int i = 0; i < platformCount; i++)
        {
            sf::FloatRect playerBounds(
                player.getPositionX(),
                player.getPositionY(),
                40.f, 40.f
            );
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            if (playerBounds.intersects(platformBounds))
            {
                float platformTop = platforms[i].getPosition().y;

                if (player.getPositionY() + 40.f <= platformTop + 10.f)
                {
                    player.setOnGround(true);
                    player.setJump(0);
                    player.snapToGround(platformTop - 40.f);
                }
            }
        }
    }
};