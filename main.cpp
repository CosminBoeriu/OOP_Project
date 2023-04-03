#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

class Point {
protected:
    sf::Vector2f position;
    sf::Color color;
public:
    explicit Point(float x = 0, float y = 0, sf::Color color = sf::Color(255, 255, 255)) :
        position(sf::Vector2f(x, y)), color(color){}
    Point([[maybe_unused]] const Point& other){
        this->position = other.position;
        this->color = other.color;
    }
    Point(const Point& p, float angle, float lenght){ //angle is in radians;
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
    void set_color( sf::Color col ){
        this->color = col;
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

class Line{
    Point *endpoints;
public:
    explicit Line(Point *v=nullptr): endpoints(v){}
    Line(const Point& a, const Point& b){
        endpoints = new Point;
        *endpoints = a;
        *(endpoints+1) = b;
    }
    Line(const Line& other){
        this->endpoints = new Point;
        this->endpoints[0] = other.endpoints[0];
        this->endpoints[1] = other.endpoints[1];
    }
    Line& operator=(const Line& other){
        if(this != &other) {
            this->endpoints = new Point;
            this->endpoints[0] = other.endpoints[0];
            this->endpoints[1] = other.endpoints[1];
        }
        return *this;
    }
    void drawLine(sf::RenderWindow& wind){
        sf::Vertex line[] = {endpoints[0].get_position(), endpoints[1].get_position()};
        wind.draw(line, 2, sf::Lines);
    }
    void set_color(sf::Color color) {
        (*endpoints).set_color(color);
        (*(endpoints+1)).set_color(color);
    }

};

/*class Sunray : public Line {
    float lenght;
public:
    Sunray(Point *x, float angle, float lenght): Line(*x, Point(*x, angle, lenght)), lenght(lenght) {}
};*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Light Simulator!");
    // const float EPSILON = 0.01;
    const float PI = 3.141592f;

    SourcePoint sourcep;
    while( window.isOpen() ) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {window.close();}

        }
        sourcep.update_position(window);
        for( int i = 0; i <= 360; i++ ){
            Point p(sourcep, float(float(i) * PI / 180), 1000);
            Line l(sourcep, p);
            l.set_color(sf::Color(255, 255, 0));
            l.drawLine(window);
        }

        window.display();
        window.clear();
    }

    return 0;
}