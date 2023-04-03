#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
const float EPSILON = 0.01;
const float PI = 3.14159265;

class Point : sf::Vertex {
public:
    explicit Point(float x = 0, float y = 0, sf::Color color = sf::Color(255, 255, 255)) :
        sf::Vertex( sf::Vector2f(x, y), color ){}
    Point(const Point& other) : Vertex(other) {
        this->position = other.position;
        this->color = other.color;
    }
    Point(const Point& p, float angle, float lenght): Vertex(){ //angle is in radians;
        float x = p.position.x + cos(angle) * lenght;
        float y = p.position.y + sin(angle) * lenght;
        this->position = sf::Vector2f(x, y);
    }
    Point& operator=(const Point&other){
        if(this != &other) {
            this->color = other.color;
            this->position = other.position;
        }
        return *this;
    }
    bool operator==(const Point& other) {
        return (other.position == this->position);
    }
    friend istream& operator>>(istream& input, Point& myPoint) {
        float x, y;
        cin >> x >> y;
        myPoint.position = sf::Vector2f(x, y);
        return input;
    }
    void set_color( sf::Color color ){
        this->color = color;
    }

};

class Line : sf::VertexArray{
    Point *endpoints;
public:
    explicit Line(Point *v=nullptr): endpoints(v){}
    Line(const Point& a, const Point& b){
        endpoints = new Point;
        *endpoints = a;
        *(endpoints+1) = b;
    }
    void draw(sf::RenderWindow& wind){
        window.draw(sf::LineStrip, *endpoints);
    }
    void set_color(sf::Color color) {
        (*endpoints).set_color(color);
        (*(endpoints+1)).set_color(color);
    }

};

class Sunray : public Line {
    float lenght;
public:
    Sunray(Point *x, float angle, float lenght): Line(*x, Point(*x, angle, lenght)), lenght(lenght) {}
};

int main()
{

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100, 50));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(10, 20);

    window.setFramerateLimit(60);
    sf::Color color(255, 0, 0);
    while( window.isOpen() ) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {window.close();}

        }
        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}