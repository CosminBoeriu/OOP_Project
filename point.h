#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Point {
protected:
    sf::Vector2f position;
    sf::Color color;
public:
    explicit Point(float x = 0, float y = 0, sf::Color color = sf::Color(255, 255, 255)) :
            position(sf::Vector2f(x, y)), color(color){}
    Point( const Point& other){
        this->position = other.position;
        this->color = other.color;
    }
    Point(const Point& p, float angle, float length, sf::Color color = sf::Color(255, 255, 255)){ //angle is in radians;
        this->color = color;
        float x = p.position.x + std::cos(angle) * length;
        float y = p.position.y + std::sin(angle) * length;
        this->position = sf::Vector2f(x, y);
    }
    Point& operator=(const Point& other){
        if(this != &other) {
            this->color = other.color;
            this->position = other.position;
        }
        return *this;
    }
    Point operator+(const Point& other) const{
        return Point(this->position.x+other.position.x, this->position.y+other.position.y);
    }
    Point operator-(const Point& other) const{
        return Point(this->position.x-other.position.x, this->position.y-other.position.y);
    }
    Point operator*(const float x) const {
        return Point(this->position.x * x, this->position.y * x);
    }
    bool operator==(const Point& other) {
        return (other.position == this->position);
    }
    float get_distance(const Point& other) const{
        return float(std::sqrt(std::pow(other.position.x - this->position.x, 2) + std::pow(other.position.y - this->position.y, 2)));
    }
    friend std::istream& operator>>(std::istream& input, Point& myPoint) {
        //float x, y;
        std::cin >> myPoint.position.x >> myPoint.position.y;
        //myPoint.position = sf::Vector2f(x, y);
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const Point& myPoint) {
        std::cout << myPoint.position.x << ' ' << myPoint.position.y;
        return output;
    }
    void set_color( sf::Color col ){
        this->color = col;
    }
    sf::Color get_color(){
        return this->color;
    }
    void set_position(float x, float y){
        position.x = x;
        position.y = y;
    }
    void set_position(sf::Vector2f v){
        position = v;
    }
    sf::Vector2f get_position(){
        return position;
    }

};

class SourcePoint: public Point{
public:
    SourcePoint(): Point(){};
    void update_position(sf::RenderWindow& wind) {
        int x = sf::Mouse::getPosition(wind).x;
        int y = sf::Mouse::getPosition(wind).y;
        position = sf::Vector2f(float(x), float(y));
    }
};
