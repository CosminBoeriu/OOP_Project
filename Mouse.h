#pragma once
#include "SFML/Graphics.hpp"
#include "line.h"

class MouseHandler{
    SourcePoint p;
    float rayFrequency;
    sf::RenderWindow* window;
    sf::Color color;
    std::vector<Sunray>sunrays;
public:
    explicit MouseHandler(sf::RenderWindow* window= nullptr, sf::Color col = sf::Color(255, 255, 0), float rayFrequency = 2 ):
        rayFrequency(rayFrequency), window(window), color(col){
        float i = 0;
        while( i < 360 ){
            sunrays.emplace_back();
            i += rayFrequency;
        }
    };
    void update_position(){
        p.update_position(*window);
        reset_rays();
    }
    void reset_rays(){
        float i = 0.1f;
        int index = 0;
        while( i <= 360 ){
            sunrays[index++] = Sunray(p, float(float(i) * PI / 180), Sunray::maximum_lenght, color);
            i += rayFrequency;
        }
    }
    void update_collisions(std::vector<Line>&obstacles, unsigned long long n) {
        Sunray* temp;
        for(Sunray & obj : sunrays){
            for( unsigned long long j = 0; j < n; j++ ) {
                Point v = obstacles[j].get_collision(obj);
                if (v.get_position().x != -1) {
                    temp = new Sunray(p, Point(v.get_position().x, v.get_position().y), color);
                    if( obj.get_length() > (*temp).get_length())
                        obj = *temp;
                    delete temp;
                }
            }
        }
    }
    void draw(){
        for( auto & obj : sunrays ){
            obj.drawLine(*window);
        }
    }
};