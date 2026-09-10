#include <iostream>
#include <string>
#include <vector>

class Shapes{
    public:
        virtual double area()
        {
            return 0.0;
        }
};

class Rectangle : public Shapes{
    public:
        double width;
        double height;
    public:
        Rectangle(double width, double height) : width(width), height(height) {}
        virtual double area() override 
        {
            return width * height;
        }
};

class Circle : public Shapes{
    public:
        double radius;
    public:
        Circle(double radius) : radius(radius) {}

        virtual double area() override 
        {
            return 3.14 * radius *radius;
        }

};



int main()
{
    Rectangle r(3.0 , 1.0);
    Circle c(4.0);

    std::vector  <Shapes*> vector;

    vector.push_back(&r);
    vector.push_back(&c);

    for(int i = 0; i < vector.size(); ++i)
    {
        std::cout << vector[i]->area() << std::endl;
    }
    return 0;
}