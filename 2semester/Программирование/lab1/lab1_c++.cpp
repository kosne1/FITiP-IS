#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

class Point{
private:
    float x_;
    float y_;

public:
    //сеттер
    void setX(float x) {
        x_=x;
    };
    void setY(float y) {
        y_=y;
    };
    //геттер
    float getX() const {
        return x_;
    };
    float getY() const {
        return y_;
    };

    //конструктор по умолчанию
    Point(float x = 0, float y = 0)
        : x_(x)
        , y_(y)
    {
        std::cout << "set Point" << '\n';
    }

    //конструктор копирования
    Point(const Point &point)
        : x_(point.x_)
        , y_(point.y_)
    {
        std::cout << "Point Copied" << '\n';
    }

    //перегрузка оператора присваивания
    Point& operator= (const Point &point) {
        x_ = point.x_;
        y_ = point.y_;
        std::cout << "Assignment Operator (point)" << '\n';
        return *this;
    }

    //деструктор
    ~Point() {
        std::cout << "Destructing Point" << '\n';
    }
};

class Polyline{
protected:
    std::vector<Point> polyline_;

public:
    //сеттер
    void setPointsToPolyline(const std::vector<Point> &points) {
        for (int i = 0; i < points.size(); ++i) {
            polyline_.push_back(points[i]);
        }
    };

    //геттер
    void getPolyline() const {
        for (int i = 0; i < polyline_.size(); ++i) {
            std::cout << "(" << polyline_[i].getX() << ";" << polyline_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    Polyline(){
        std::cout << "Set Empty Polyline" << '\n';
    }

    //конструктор
    Polyline(const std::vector<Point> &points) {
        assert(points.size() >= 2);
        for (int i = 0; i < points.size(); ++i) {
            polyline_.push_back(points[i]);
        }
        std::cout << "Set Polyline" << '\n';
    }

    //конструктор копирования
    Polyline(const Polyline &polyline)
        : polyline_(polyline.polyline_)
    {
        std::cout << "Polyline Copied" << '\n';
    }

    //перегрузка оператора присваивания
    Polyline& operator= (const Polyline &polyline) {
        polyline_ = polyline.polyline_;
        std::cout << "Assignment Operator (polyline)" << '\n';
        return *this;
    }

    //расстояние между двумя точками
    float pointToPoint(const Point &point1, const Point &point2) {
        return sqrt(pow(point1.getX() - point2.getX(), 2)
               + pow(point1.getY() - point2.getY(), 2));
    }

    //длина ломаной
    virtual float length() {
        float length = 0;
        for (int i = 0; i < polyline_.size() - 1; ++i) {
            length += pointToPoint(polyline_[i], polyline_[i+1]);
        }
        return length;
    }

    //деструктор
    ~Polyline() {
        std::cout << "Destructing Polyline" << '\n';
    }
};

class ClosedPolyline : public   Polyline {
public:
    //геттер
    void getClosedPolyline() const {
        for (int i = 0; i < polyline_.size() - 1; ++i) {
            std::cout << "(" << polyline_[i].getX() << ";" << polyline_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    ClosedPolyline(){
        std:: cout << "Set Empty Closed Polyline" << '\n';

    }

    //конструтор
    ClosedPolyline(const std::vector<Point> &points) : Polyline(points) {
        assert(points.size() >= 3);
        assert((points[0].getX() == points[points.size()-1].getX()) && (points[0].getY() == points[points.size()-1].getY()));
        std:: cout << "Set Closed Polyline" << '\n';
    }

    //конструктор копирования
    ClosedPolyline(const ClosedPolyline &closedPolyline) : Polyline(closedPolyline)
    {
        std::cout << "Closed Polyline Copied" << '\n';
    }

    //перегрузка оператора присваивания
    ClosedPolyline& operator= (const ClosedPolyline &closedPolyline) {
        polyline_ = closedPolyline.polyline_;
        std::cout << "Assignment Operator (closed polyline)" << '\n';
        return *this;
    }

    //длина замкнутой ломаной
    float length() {
        float length = 0;
        for (int i = 0; i < polyline_.size() - 1; ++i) {
            length += pointToPoint(polyline_[i], polyline_[i+1]);
        }
        return length;
    }

    //деструктор
    ~ClosedPolyline() {
        std::cout << "Destructing Closed Polyline" << '\n';
    }
};

class Polygon {
protected:
    std::vector<Point> polygon_;

public:
    //геттер
    void getPolygon() const {
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            std::cout << "(" << polygon_[i].getX() << ";" << polygon_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    Polygon() {
        std::cout << "Set Empty Polygon" << '\n';
    }

    //расстояние между двумя точками
    float pointToPoint(const Point &point1, const Point &point2) {
        return sqrt(pow(point1.getX() - point2.getX(), 2)
                    + pow(point1.getY() - point2.getY(), 2));
    }

    //площадь треугольника
    static float areaTriangle(const Point &point1, const Point &point2, const Point &point3) {
        float area = 0;
        area = abs(((point1.getX() - point3.getX())*(point2.getY() - point3.getY()) -
                (point2.getX() - point3.getX())*(point1.getY() - point3.getY()))/2);
        return area;
    }

    //центр масс фигуры
    Point centerMass() const {
        float x = 0;
        float y = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            x += polygon_[i].getX();
            y += polygon_[i].getY();
        }
        x /= (polygon_.size() - 1);
        y /= (polygon_.size() - 1);
        Point centerMass {x, y};
        return centerMass;
    }

    //проверка на выпуклость
    bool isItConvexPolygon() const {
        for (int i = 0; i < polygon_.size() - 2; ++i) {
            if (2*areaTriangle(polygon_[i], polygon_[i + 1], centerMass()) <=
                    areaTriangle(polygon_[i], polygon_[i + 2], centerMass()))
                return 0;
        }
        return 1;
    }

    //конструктор
    Polygon(const std::vector<Point> &points) {
        assert(isItConvexPolygon());
        std:: cout << "Set Polygon" << '\n';
    }

    //конструктор копирования
    Polygon(const Polygon &polygon) : polygon_(polygon.polygon_)
    {
        std::cout << "Polygon Copied" << '\n';
    }

    //перегрузка оператора присваивания
    Polygon& operator= (const Polygon &polygon) {
        polygon_ = polygon.polygon_;
        std::cout << "Assignment Operator (polygon)" << '\n';
        return *this;
    }

    //длина многоугольника
    float length() {
        float length = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            length += pointToPoint(polygon_[i], polygon_[i + 1]);
        }
        return length;
    }

    //площадь многоугольника
    virtual float area() {
        float area = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            area += areaTriangle(polygon_[i], polygon_[i + 1], centerMass());
        }
        return area;
    }

    //деструктор
    ~Polygon(){
        std::cout << "Destructing Polygon" << '\n';
    }
};

class Triangle : public Polygon {
public:
    //геттер
    void getTriangle() const {
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            std::cout << "(" << polygon_[i].getX() << ";" << polygon_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    Triangle() {
        std::cout << "Set Empty Triangle" << '\n';
    }

    //конструктор
    Triangle(const std::vector<Point> &points) : Polygon(points) {
        assert(polygon_.size() == 4);
        std::cout << "Set Triangle" << '\n';
    }
    //конструктор копирования
    Triangle(const Triangle &triangle) : Polygon(triangle)
    {
        std::cout << "Triangle Copied" << '\n';
    }

    //перегрузка оператора присваивания
    Triangle& operator= (const Triangle &triangle) {
        polygon_ = triangle.polygon_;
        std::cout << "Assignment Operator (triangle)" << '\n';
        return *this;
    }

    //периметр треугольника
    float length() {
        float length = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            length += pointToPoint(polygon_[i], polygon_[i + 1]);
        }
        return length;
    }

    //площадь треугольника
    float area() {
        float area = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            area += areaTriangle(polygon_[i], polygon_[i + 1], centerMass());
        }
        return area;
    }

    //деструктор
    ~Triangle(){
        std::cout << "Destructing Triangle" << '\n';
    }
};

class Trapezoid : public Polygon {
public:
    //геттер
    void getTrapezoid() const {
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            std::cout << "(" << polygon_[i].getX() << ";" << polygon_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    Trapezoid(){
        std::cout << "Set Empty Trapezoid" << '\n';
    }

    //провекрка на трапецию
    bool isItTrapezoid() const {
        if ((polygon_[0].getX() == polygon_[1].getX() && polygon_[2].getX() == polygon_[3].getX())
            && (polygon_[2].getY() != polygon_[1].getY() || polygon_[0].getY() != polygon_[3].getY()))
            return 1;
        if ((polygon_[0].getY() == polygon_[1].getY() && polygon_[2].getY() == polygon_[3].getY())
            && (polygon_[2].getX() != polygon_[1].getX() || polygon_[0].getX() != polygon_[3].getX()))
            return 1;
        if ((polygon_[2].getX() == polygon_[1].getX() && polygon_[0].getX() == polygon_[3].getX())
            && (polygon_[0].getY() != polygon_[1].getY() || polygon_[2].getY() != polygon_[3].getY()))
            return 1;
        if ((polygon_[2].getY() == polygon_[1].getY() && polygon_[0].getY() == polygon_[3].getY())
            && (polygon_[0].getX() != polygon_[1].getX() || polygon_[2].getX() != polygon_[3].getX()))
            return 1;
        return 0;
    };

    //конструктор
    Trapezoid(const std::vector<Point> &points) : Polygon(points) {
        assert(polygon_.size() == 5);
        assert(isItTrapezoid());
        std::cout << "Set Trapezoid" << '\n';
    }

    //конструктор коипрования
    Trapezoid(const Trapezoid &trapezoid) : Polygon(trapezoid)
    {
        std::cout << "Trapezoid Copied" << '\n';
    }

    //перегрузка оператора присваивания
    Trapezoid& operator= (const Trapezoid &trapezoid) {
        polygon_ = trapezoid.polygon_;
        std::cout << "Assignment Operator (trapezoid)" << '\n';
        return *this;
    }

    //периметр трапеции
    float length() {
        float length = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            length += pointToPoint(polygon_[i], polygon_[i + 1]);
        }
        return length;
    }

    //площадь трапеции
    float area() {
        float area = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            area += areaTriangle(polygon_[i], polygon_[i + 1], centerMass());
        }
        return area;
    }

    //деструктор
    ~Trapezoid() {
        std::cout << "Destructing Trapezoid" << '\n';
    }
};

class RegularPolygon : public Polygon {
public:
    //геттер
    void getRegularPolygon() const {
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            std::cout << "(" << polygon_[i].getX() << ";" << polygon_[i].getY() << ") ";
        }
        std::cout << '\n';
    }

    //конструктор по умолчанию
    RegularPolygon() {
        std::cout << "Set Empty Regular Polygon" << '\n';
    }

    //проверка на правильный многоугольник
    bool isItRegularPolygon() const {
        for (int i = 0; i < polygon_.size() - 2; ++i) {
            if (areaTriangle(polygon_[i], polygon_[i + 1], centerMass())
                != areaTriangle(polygon_[i + 1], polygon_[i + 2], centerMass()))
                return 0;
        }
        return 1;
    }

    //конструктор
    RegularPolygon(const std::vector<Point> &points) : Polygon(points) {
        assert(isItRegularPolygon());
        std::cout << "Set Regular Polygon" << '\n';
    }

    //конструктор копирования
    RegularPolygon(const RegularPolygon &regularPolygon) : Polygon(regularPolygon)
    {
        std::cout << "Regular Polygon Copied" << '\n';
    }

    //перегрузка оператора присваивания
    RegularPolygon& operator= (const RegularPolygon &regularPolygon) {
        polygon_ = regularPolygon.polygon_;
        std::cout << "Assignment Operator (regular polygon)" << '\n';
        return *this;
    }

    //периметр правильного многоугольника
    float length() {
        float length = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            length += pointToPoint(polygon_[i], polygon_[i + 1]);
        }
        return length;
    }

    //площадь правильного многоугольника
    float area() {
        float area = 0;
        for (int i = 0; i < polygon_.size() - 1; ++i) {
            area += areaTriangle(polygon_[i], polygon_[i + 1], centerMass());
        }
        return area;
    }

    //деструктор
    ~RegularPolygon(){
        std::cout << "Destructing Regular Polygon" << '\n';
    }
};

int main() {
    Point myPoint0;
    Point myPoint1 {0,4};
    Point myPoint2 {4,4};
    Point myPoint3 {4,0};

    Point myPoint4 {3,-3};

    Point myPoint12 {2,2};

    std::vector<Point> points {myPoint0, myPoint1, myPoint2, myPoint3, myPoint0};
    RegularPolygon my {points};
    return 0;
}
