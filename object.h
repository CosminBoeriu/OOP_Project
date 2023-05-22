#pragma once
#include "shape.h"

class Object : public MovingSh, public RotatingSh{
public:
    explicit Object(const std::vector<sf::Vector2f>&v={}, int num=0, float add_angle = 1.0f, const std::vector<sf::Vector2f>&v2={}, int nr = 0,
           float time = 1, bool closed = true, int frames = 60):
            Shape(v, num), MovingSh(v, num, v2, nr, time, closed, frames), RotatingSh(v, num, add_angle){};

    void update() override{
        MovingSh::update();
        RotatingSh::update();
        get_sfml_shape();
    }

    void restore_to_initial_position() override{
        MovingSh::restore_to_initial_position();
        RotatingSh::restore_to_initial_position();
    }
    void toggle_rotation() override{
        RotatingSh::toggle_rotation();
    }
    void toggle_movement() override{
        MovingSh::toggle_movement();
    }
    [[nodiscard]] std::unique_ptr<Shape> clone() const override{
        return std::make_unique<Object>(*this);
    };

    void read() override{
        std::vector<sf::Vector2f>v1, v2;
        int frames;
        float time;
        std::cin>>num_points>>num_of_tr_points>>this->angle_change>>time>>this->closed>>frames;
        add_per_frame = 1 / (time * float(frames));
        for(unsigned long long i = 0; i < num_points; i++ ){
            v1.emplace_back();
            std::cin>>v1[i].x>>v1[i].y;
        }
        for(unsigned long long i = 0; i < num_of_tr_points; i++ ){
            v2.emplace_back();
            std::cin>>v2[i].x>>v2[i].y;
        }
        Shape::set_lines(v1, num_points);
        MovingSh::set_trajectory(v2, num_of_tr_points);
    }
};