#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
const float PI = 3.141592f;

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
    friend ostream& operator<<(ostream& output, const Point& myPoint) {
        cout << myPoint.position.x << ' ' << myPoint.position.y;
        return output;
    }
    void set_color( sf::Color col ){
        this->color = col;
    }
    sf::Color get_color(){
        return this->color;
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
    explicit Line(const Point& a = Point(), const Point& b = Point()){
        endpoints = new Point[2];
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
    friend istream& operator>>(istream& input, const Line& myLine) {
        cin>>myLine.endpoints[0]>>myLine.endpoints[1];
        return input;
    }
    friend ostream& operator<<(ostream& output, Line& myLine) {
        cout << myLine.endpoints[0] <<' '<< myLine.endpoints[1];
        return output;
    }
    void drawLine(sf::RenderWindow& wind){
        sf::Vertex line[] = {endpoints[0].get_position(), endpoints[1].get_position()};
        line[0].color = endpoints[0].get_color();
        line[1].color = endpoints[1].get_color();
        wind.draw(line, 2, sf::Lines);
    }
    void set_color(sf::Color color) {
        (*endpoints).set_color(color);
        (*(endpoints+1)).set_color(color);
    }
    float get_tangent(){
        sf::Vector2f p1 = endpoints[0].get_position();
        sf::Vector2f p2 = endpoints[1].get_position();
        if( p1.x == p2.x ){
            return 0;
        }
        else{
            return (p2.y-p1.y)/(p2.x-p1.x);
        }
    }
    float get_reminder(){
        float m = this->get_tangent();
        return endpoints[0].get_position().y - endpoints[0].get_position().x * m;
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
        if( abs(this->get_tangent() - l.get_tangent() ) < 0.001f ){
            return Point(-1, -1);
        }
        else {
            float x = (this->get_reminder() - l.get_reminder()) / (l.get_tangent() - this->get_tangent());
            float y = this->get_tangent() * x + this->get_reminder();
            Point a(x, y);
            if (l.test_point_inclusion(a) && this->test_point_inclusion(a)) {
                return Point(x, y);
            } else {
                return Point(-1, -1);
            }
        }
    }
    ~Line(){
        delete[] endpoints;
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
    window.setFramerateLimit(60);
    // const float EPSILON = 0.01;
    int numberOfObstacles = 4;
    Line *obstacles = new Line[numberOfObstacles];
    for( int i = 0; i < numberOfObstacles; i++ ){
        cin>>obstacles[i];
    }
    /*
    1 1 800 800
    1 800 800 1
    1890 300 439 567
    600 900 300 1200
    */
    //cout<<obstacles[1];
    SourcePoint sourcep;
    while( window.isOpen() ) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {window.close();}
        }
        sourcep.update_position(window);
        for( int i = 1; i <= 360; i += 2 ){
            Point p(sourcep, float(float(i) * PI / 180), 10000);
            Line *l = new Line(sourcep, p);
            for( int j = 0; j < numberOfObstacles; j++ ) {
                Point v = obstacles[j].get_collision(*l);
                //cout<<j<<' '<<v<<endl;
                if (v.get_position().x != -1) {
                    delete l;
                    l = new Line(sourcep, Point(v.get_position().x, v.get_position().y));
                }
            }
            (*l).set_color(sf::Color(255, 255, 0));
            (*l).drawLine(window);
        }
        for( int i = 0; i < numberOfObstacles; i++ ) {
            obstacles[i].drawLine(window);
        }
        window.display();
        window.clear();
    }

    return 0;
}