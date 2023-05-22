#pragma once
#include "line.h"
#include <cmath>
#include <functional>
#include <stdexcept>

const float PI = 3.141592f;

class Shape{
protected:
    std::vector<Point>points;
    std::vector<Line>lines;
    sf::VertexArray sfShape;
    unsigned long long num_points;
    Point center;
public:
    static int number_of_created_shapes;
    explicit Shape(const std::vector<sf::Vector2f>&v={}, int num=0): num_points(num){
        set_lines(v, num);
        calculate_center();
        number_of_created_shapes++;
        if (number_of_created_shapes > 10 ){
            throw std::runtime_error("Number of shapes cannot exceed 10! Sorry for the inconvenience!");
        }
    }

    void set_lines(const std::vector<sf::Vector2f>&v, unsigned long long num){
        points.clear();
        lines.clear();
        for ( unsigned long long i = 0; i < num; i++ ){
            points.emplace_back(v[i].x, v[i].y);
            lines.emplace_back(Point(v[i].x, v[i].y), Point(v[(i+1)%num].x, v[(i+1)%num].y));
        }
        calculate_center();
    }

    virtual std::unique_ptr<Shape> clone() const = 0;

    Shape& operator=(const Shape& other){
        if( this != &other){
            points = other.points;
            lines = other.lines;
            sfShape = other.sfShape;
            num_points = other.num_points;
        }
        return *this;
    }
    Shape(const Shape& other){
        this->lines = other.lines;
        this->points = other.points;
        this->sfShape = other.sfShape;
        this->num_points = other.num_points;
    }

    friend std::istream& operator>>(std::istream& input, Shape& mySh) {
        mySh.read();
        return input;
    }

    void get_sfml_shape(){
        sfShape = sf::VertexArray(sf::TriangleStrip, num_points);
        sfShape[0] = points[0].get_position();
        sfShape[0].color = points[0].get_color();
        unsigned long long i = 1, j = num_points - 1, p = 1;
        while( i <= j ){
            sfShape[p].color = points[j].get_color();
            sfShape[p++] = points[j--].get_position();
            if( i <= j ) {
                sfShape[p] = points[i].get_position();
                sfShape[p++].color = points[i++].get_color();
            }
        }
    }
    void calculate_center(){
        float x = 0, y = 0;
        for( unsigned long long i = 0; i < num_points; i++ ){
            x += points[i].get_position().x;
            y += points[i].get_position().y;
        }
        x /= float(num_points);
        y /= float(num_points);
        center.set_position(x, y);
    }
    void draw(sf::RenderWindow* window){  // note - it is preferred that the object is first updated, then drawn.
        (*window).draw(sfShape);
    }
    std::vector<Line>* get_lines(){
        return &(this->lines);
    }
    virtual void read() = 0;
    virtual void update() = 0;
    virtual void restore_to_initial_position() = 0;
    virtual void toggle_rotation() = 0;
    virtual void toggle_movement() = 0;
    virtual ~Shape(){
        number_of_created_shapes--;
    };
};
int Shape::number_of_created_shapes = 0;


class RotatingSh: virtual public Shape{
protected:
    float angle = 0;
    float angle_change;
    bool rotationState = true;
public:
    explicit RotatingSh( const std::vector<sf::Vector2f>&v={}, int num=0, float add_angle = 1.0f ) : Shape(v, num),
        angle_change(add_angle) {}

    std::unique_ptr<Shape> clone() const override{
        return std::make_unique<RotatingSh>(*this);
    };

    void read() override{
        std::vector<sf::Vector2f>v1;
        std::cin>>num_points>>this->angle_change;
        for(unsigned long long i = 0; i < num_points; i++ ){
            v1.emplace_back();
            std::cin>>v1[i].x>>v1[i].y;
        }
        set_lines(v1, num_points);
    }

    void rotate(float add_angle) {
        angle = (angle + add_angle);
        sf::Vector2f newPos;
        add_angle = (float(add_angle) * PI / 180);
        for( unsigned long long i = 0; i < num_points; i++ ){
            sf::Vector2f oldPos = points[i].get_position();
            newPos.x = oldPos.x * std::cos(add_angle) - oldPos.y * std::sin(add_angle) + center.get_position().x -
                    center.get_position().x * std::cos(add_angle) + center.get_position().y * std::sin(add_angle);
            newPos.y = oldPos.x * std::sin(add_angle) + oldPos.y * std::cos(add_angle) + center.get_position().y -
                    center.get_position().x * std::sin(add_angle) - center.get_position().y * std::cos(add_angle);;
            points[i].set_position(newPos);
        }
        for ( unsigned long long i = 0; i < num_points; i++ ){
            lines[i] = Line(points[i], points[(i+1)%num_points]);
        }

    }
    void restore_to_initial_position() override{
        this->rotate(-angle);
        angle = 0;
    }
    void toggle_rotation() override{
        rotationState = !rotationState;
    }
    void update() override{
        if(rotationState)
            rotate(this->angle_change);
        get_sfml_shape();
    }
    void toggle_movement() override{
        throw std::invalid_argument("Rotating Shapes do not move by default - try creating an Object instead");
    }
};



class MovingSh: virtual public Shape{
protected:
    std::vector<Point>trajectory;
    unsigned long long num_of_tr_points{};
    float add_per_frame, current_pos = 0;
    bool closed, movementState = true;
public:
    explicit MovingSh( const std::vector<sf::Vector2f>&v={}, int num=0, const std::vector<sf::Vector2f>&v2={}, int nr = 0,
                       float time = 1, bool closed = true, int frames = 60 ) : Shape(v, num), closed(closed){
        set_trajectory(v2, nr);
        add_per_frame = 1 / ( time * float(frames) );
    }
    void set_trajectory(const std::vector<sf::Vector2f>&v2, unsigned long long nr){
        num_of_tr_points = nr+1;
        trajectory.clear();
        trajectory.push_back(center);
        for( unsigned long long i = 1; i < num_of_tr_points; i++ ){
            trajectory.emplace_back(v2[i-1].x, v2[i-1].y);
        }
        if(closed) {
            num_of_tr_points++;
            trajectory.push_back(center);
        }
    }

    std::unique_ptr<Shape> clone() const override{
        return std::make_unique<MovingSh>(*this);
    };

    void read() override{
        std::vector<sf::Vector2f>v1, v2;
        int frames;
        float time;
        std::cin>>num_points>>num_of_tr_points>>time>>this->closed>>frames;
        add_per_frame = 1 / (time * float(frames));
        for(unsigned long long i = 0; i < num_points; i++ ){
            v1.emplace_back();
            std::cin>>v1[i].x>>v1[i].y;
        }
        for(unsigned long long i = 0; i < num_of_tr_points; i++ ){
            v2.emplace_back();
            std::cin>>v2[i].x>>v2[i].y;
        }
        set_lines(v1, num_points);
        set_trajectory(v2, num_of_tr_points);
    }

    Point curveIndex( float t ){  // returns point on curve that has t value given as parameter.
        Point p(trajectory[0] * float(std::pow((1-t), num_of_tr_points-1)) +
                trajectory[num_of_tr_points-1] * float(std::pow(t, num_of_tr_points-1)));
        auto exponent = float(num_of_tr_points-1);
        for( int i = 1; i < num_of_tr_points-1; i++ ){
            p = p + trajectory[i] * exponent * float(std::pow(1-t, i) * std::pow(t, num_of_tr_points-1-i));
            exponent = exponent * float( num_of_tr_points - 1 - i ) / float( i + 1 );
        }
        return p;
    }

    void move(const Point& x){
        points[0] = points[0] + x;
        for ( unsigned long long i = 1; i < num_points; i++ ){
            points[i] = points[i] + x;
            lines[i-1] = Line(points[i-1], points[i]);
        }
        lines[num_points-1] = Line(points[num_points-1], points[0]);
        calculate_center();
    }
    void toggle_movement() override{
        movementState = !movementState;
    }
    void restore_to_initial_position() override{
        move(trajectory[0]);
    }

    void update() override{
        if (movementState) {
            current_pos += add_per_frame;
            if (current_pos <= 0 || current_pos >= 1) {
                if (!closed) {
                    add_per_frame = -add_per_frame;
                    current_pos += add_per_frame;
                }
                else {
                    current_pos = current_pos-1;
                }
            }
            Point nextCenter = curveIndex(current_pos);
            move(nextCenter - center);
        }
        get_sfml_shape();
    }
    void toggle_rotation() override{
        throw std::runtime_error("Moving Shapes do not rotate by default - try creating an Object instead");
    }
};