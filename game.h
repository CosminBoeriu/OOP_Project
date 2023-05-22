#pragma once
#include <iostream>
#include "object.h"
#include <SFML/Graphics.hpp>
#include "Mouse.h"
#include <thread>

/* std::map<std::string, Shape*>m = std::map<std::string, Shape*>{
    {"Rotating", new RotatingSh},
    {"Moving", new MovingSh},
    {"Object", new Object} };  as vrea sa mapez valorile in loc sa fac switch case de initializare*/
const std::vector<sf::Keyboard::Key>num_keys = {sf::Keyboard::Num0, sf::Keyboard::Num1,sf::Keyboard::Num2,sf::Keyboard::Num3,sf::Keyboard::Num4,
                                                sf::Keyboard::Num5,sf::Keyboard::Num6,sf::Keyboard::Num7,sf::Keyboard::Num8,sf::Keyboard::Num9};
class Game{
    sf::RenderWindow* window;
    std::vector<std::unique_ptr<Shape>>objects;
    MouseHandler mouse_point;
    int frameRate = 60;
    unsigned long long selected_shape = 10;
public:
    Game(sf::RenderWindow* wind, float max_len): window(wind){
        Sunray::set_max_length(max_len);
        (*window).setFramerateLimit(frameRate);
        sf::ContextSettings settings;
        settings.antialiasingLevel = 2;
        this->mouse_point = MouseHandler(window, sf::Color(255, 255, 0), 2.0f);
    }
    void main_func(){
        while( window->isOpen() ) {
            sf::Event event{};
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {window->close();}
                if (event.type == sf::Event::KeyReleased){
                    if (event.key.code == sf::Keyboard::C){
                        std::string type;
                        std::cin>>type;
                        if( type == "Rotating" ){
                            auto* obj = new RotatingSh();
                            create_new_shape(obj);
                        }
                        if( type == "Moving" ){
                            auto* obj = new MovingSh();
                            create_new_shape(obj);
                        }
                        if( type == "Object" ){
                            auto* obj = new Object();
                            create_new_shape(obj);
                        }
                    }
                    else{
                        int index = 0;
                        for( sf::Keyboard::Key key : num_keys ){
                            if( key == event.key.code ){
                                selected_shape = index;
                                break;
                            }
                            index++;
                        }
                    }
                    if( event.key.code == sf::Keyboard::M && selected_shape < objects.size()) {
                        try {
                            objects[selected_shape]->toggle_movement();
                            std::cout << "stopped the movement of " << selected_shape << '\n';
                        }
                        catch(std::exception& e){
                            std::cout<<e.what()<<'\n';
                        }
                    }
                    if( event.key.code == sf::Keyboard::R && selected_shape < objects.size()){
                        try{
                            objects[selected_shape]->toggle_rotation();
                            std::cout<<"stopped the rotation of "<<selected_shape<<'\n';

                        }
                        catch(std::string& s){
                            std::cout<<s<<'\n';
                        }
                    }
                }

            }
            /// Updating collisions between all shapes and the light source, while also updating the state of the objects
            mouse_point.update_position();
            for( std::unique_ptr<Shape> & obj : objects ){
                obj->update();
                mouse_point.update_collisions( *(obj->get_lines()), obj->get_lines()->size());
            }

            mouse_point.draw(); /// Drawing the sunrays on screen

            /// Drawing all shapes on screen
            for( auto & shape : objects ) {
                shape->draw(window);
            }

            window->display();
            window->clear();
        }
    }

    void create_new_shape(Shape* s){
        objects.emplace_back(s->clone());
        std::cin>>*objects[objects.size()-1];
    };
};
