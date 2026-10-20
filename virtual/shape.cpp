#include <iostream>
#include <string>
#include <cmath>

class Shape{
    public:
        virtual  double area() const  = 0;   
        virtual void draw() const = 0;
        virtual double perimeter() const = 0;
        virtual ~Shape() = default;
};

class Circle : public Shape {
    public:
        double r;
    public:
        Circle(double r);
        double area() const override
        {
            return 3.14 * r *r;
        }
        double perimeter() const override
        {
            return 2 * 3.14 * r;
        }
        void draw() const override
        {
            std::cout << "Drawing Circle (r = " << r << ")" << std::endl;
        }
};

Circle::Circle(double r)
{
    this->r = r;
}

class Rectangle : public Shape{
    public:
        double a;
        double b;
    public:
        Rectangle(double a, double b);
         double area() const override
        {
            return a * b;
        }
        double perimeter() const override
        {
            return 2 * (a + b);
        }
        void draw() const override
        {
            std::cout << "Drawing Rectangle (" << a << " x " << b << ")" << std::endl;
        }
};

Rectangle::Rectangle(double a, double b)
{
    this->a = a;
    this->b = b;
}

class Triangle : public Shape{
    public:
        double a;
        double b;
        double c;
    public:
        Triangle(double a, double b, double c);
        double area() const override
        {
            double s = perimeter() / 2;
            return std::sqrt(s * (s - a) * (s - b) * (s - c));
        }
        double perimeter() const override
        {
            return a + b + c;
        }
        void draw() const override
        {
            std::cout << "Drawing Triangle (" << a << ", " << b << ", " << c << ")" << std::endl;
        }
};

Triangle::Triangle(double a, double b, double c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

int main()
{
    // Shape s; 

    Shape* shapes[3];
    shapes[0] = new Circle(5);
    shapes[1] = new Rectangle(4, 6);
    shapes[2] = new Triangle(3, 4, 5);

    for (int i = 0; i < 3; i++)
    {
        shapes[i]->draw();
        std::cout << "  area      = " << shapes[i]->area() << std::endl;
        std::cout << "  perimeter = " << shapes[i]->perimeter() << std::endl;
    }

    for (int i = 0; i < 3; i++)
        delete shapes[i];

    return 0;
}