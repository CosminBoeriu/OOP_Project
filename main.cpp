#include <iostream>
#include "shape.h"
#include "game.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(2000, 2000), "Light Simulator!");
    Game g = Game(&window, 2500);
    g.main_func();
    return 0;
}