#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
float EPSILON = 0.01;

class Point : sf::Vertex {
public:
    explicit Point(float x, float y, sf::Color color = sf::Color(255, 255, 255)) :
        sf::Vertex( sf::Vector2f(x, y), color ){}
    Point(const Point& other) : Vertex(other) {
        this->position = other.position;
        this->color = other.color;
    }
    Point(const Point& p, float angle, float lenght){

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

class Line{
    Point *endpoints;
public:
    explicit Line(Point *v=nullptr): endpoints(v){}
    void set_color(sf::Color color) {
        (*endpoints).set_color(color);
        (*(endpoints+1)).set_color(color);
    }

};


class Sunray : public Line {
    float lenght;
public:
    Sunray(Point *x, float angle, int lenght): Line(), length(lenght) {}
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