#pragma once
#include <array>
#include "point.h"
#include <cmath>

class Line{
protected:
    std::array<Point, 2>endpoints;
    float gradient = 0, intercept = 0;
    int flag = 1;
public:
    explicit Line(const Point& a = Point(), const Point& b = Point()){
        endpoints[0] = a;
        endpoints[1] = b;
        this->get_gradient();
        this->get_intercept();
    }
    Line(const Line& other){
        this->endpoints[0] = other.endpoints[0];
        this->endpoints[1] = other.endpoints[1];
        this->flag = other.flag;
        this->gradient = other.gradient;
        this->intercept = other.intercept;
    }
    Line& operator=(const Line& other){
        if(this != &other) {
            this->endpoints[0] = other.endpoints[0];
            this->endpoints[1] = other.endpoints[1];
            this->flag = other.flag;
            this->gradient = other.gradient;
            this->intercept = other.intercept;
        }
        return *this;
    }
    friend std::istream& operator>>(std::istream& input, Line& myLine) {
        std::cin>>myLine.endpoints[0]>>myLine.endpoints[1];
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const Line& myLine) {
        std::cout << myLine.endpoints[0] <<' '<< myLine.endpoints[1];
        return output;
    }
    void drawLine(sf::RenderWindow& wind){
        sf::Vertex line[] = {endpoints[0].get_position(), endpoints[1].get_position()};
        line[0].color = endpoints[0].get_color();
        line[1].color = endpoints[1].get_color();
        wind.draw(line, 2, sf::Lines);
    }
    std::array<Point, 2>* get_points(){
        return &endpoints;
    }
    void set_color(sf::Color color) {
        endpoints[0].set_color(color);
        endpoints[1].set_color(color);
    }
    void set_color(sf::Color color1, sf::Color color2) {
        endpoints[0].set_color(color1);
        endpoints[1].set_color(color2);
    }
    void scale_Length(float lenght){
        float scalar = lenght / calculate_length();
        endpoints[1] = endpoints[0] * scalar + endpoints[1] * scalar;
    }
    float calculate_length(){
        float x, y;
        x = (endpoints[0].get_position().x - endpoints[1].get_position().x);
        y = (endpoints[0].get_position().y - endpoints[1].get_position().y);
        return std::sqrt( x * x + y * y  );
    }
    void get_gradient(){
        sf::Vector2f p1 = endpoints[0].get_position();
        sf::Vector2f p2 = endpoints[1].get_position();
        if( p1.x == p2.x ){
            this->gradient = 0;
            this->flag = 0;
        }
        else{
            this->gradient = (p2.y-p1.y)/(p2.x-p1.x);
        }
    }
    void get_intercept(){
        if( flag == 1 )
            intercept = endpoints[0].get_position().y - endpoints[0].get_position().x * gradient;
        else
            intercept = -endpoints[0].get_position().x;
    }

    bool test_point_inclusion(Point p){
        if(((endpoints[0].get_position().x <= p.get_position().x && p.get_position().x <= endpoints[1].get_position().x) ||
            (endpoints[1].get_position().x <= p.get_position().x && p.get_position().x <= endpoints[0].get_position().x))&&
           ((endpoints[0].get_position().y <= p.get_position().y && p.get_position().y <= endpoints[1].get_position().y) ||
            (endpoints[1].get_position().y <= p.get_position().y && p.get_position().y <= endpoints[0].get_position().y))){
            return true;
        }
        return false;
    }
    Point get_collision(Line& l){
        // n1 - n2 = m2 x - m1 x = ( m2 - m1 ) x
        if( std::abs(float(flag) * gradient - float(l.flag) * l.gradient ) < 0.001f ){
            return Point(-1, -1);
        }
        else {
            float x, y;
            if( flag == 0 || l.flag == 0 ){
                if( flag == 0 ) {
                    x = -intercept;
                    y = l.gradient * x - l.intercept;
                }
                else{
                    x = -l.intercept;
                    y = gradient * x - intercept;
                }
            }
            else {
                x = (intercept - l.intercept) / (l.gradient - gradient);
                y = gradient * x + intercept;
            }
            Point a(x, y);
            if (l.test_point_inclusion(a) && this->test_point_inclusion(a)) {
                return Point(x, y);
            } else {
                return Point(-1, -1);
            }
        }
    }
};


class Sunray : public Line {
    float length;
    sf::Color color;
public:
    inline static float maximum_lenght;
    explicit Sunray(const Point& x = Point(), float angle = -1, float length = 0, sf::Color color = sf::Color(255, 255, 0)):
            Line(x, Point(x, angle, length)), length(length), color(color) {
        this->set_color(color, sf::Color(0, 0, 0));
        this->decoloring();
    }
    Sunray(const Point &x, const Point &y, sf::Color color = sf::Color(255, 255, 0)): Line( x, y ){
        length = this->calculate_length();
        this->set_color(color, sf::Color(0, 0, 0));
        this->color = color;
        this->decoloring();
    }
    static void set_max_length(float length){
        maximum_lenght = length;
    }
    void decoloring(){
        sf::Color col;
        col.r = std::max(color.r - (unsigned char)(( length / Sunray::maximum_lenght ) * 255), 0);
        col.g = std::max(color.g - (unsigned char)(( length / Sunray::maximum_lenght ) * 255), 0);
        col.b = std::max(color.b - (unsigned char)(( length / Sunray::maximum_lenght ) * 255), 0);
        endpoints[1].set_color(col);
    }
    float get_length() const{
        return length;
    }
};
