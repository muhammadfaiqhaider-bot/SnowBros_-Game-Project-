
#pragma once
#include <SFML/Window/Event.hpp>
#include "GamePlay.h"

class LevelNavigator
{
public:
   
    static void handleEvent(const sf::Event& event, GamePlay& gp)
    {
        if (event.type != sf::Event::KeyPressed) return;

        using K = sf::Keyboard;
        int target = -1;
        switch (event.key.code)
        {
        case K::Num1: case K::Numpad1: target = 1; break;
        case K::Num2: case K::Numpad2: target = 2; break;
        case K::Num3: case K::Numpad3: target = 3; break;
        case K::Num4: case K::Numpad4: target = 4; break;
        case K::Num5: case K::Numpad5: target = 5; break;
        case K::Num6: case K::Numpad6: target = 6; break;
        case K::Num7: case K::Numpad7: target = 7; break;
        case K::Num8: case K::Numpad8: target = 8; break;
        case K::Num9: case K::Numpad9: target = 9; break; //press i bcz 0 is already used for player 2 thrwoable
        case K::I: target = 10; break;
        default: break;
        }

        if (target >= 1 && target <= 10)
        {
            gp.jumpToLevel(target);
        }
    }
};

