#include <iostream>
#include <string>

class Animal {
    public:
        virtual void speak()
        {
            std::cout << "Այս կենդանին ձայն է հանում:" << std::endl;
        }

        virtual ~Animal() = default;
};

class Dog : public Animal{
    public:
        virtual void speak() override 
        {
            std::cout << "Հաֆ-հաֆ!" << std::endl;
        }
};

class Cat : public Animal{
    public: 
        virtual void speak() override
        {
            std::cout << "Մյաու-մյաու!" << std::endl;
        }
};


int main()
{
    Animal* myDog = new Dog();
    Animal* myCat = new Cat();

    myDog->speak();
    myCat->speak();

    delete myDog;
    delete myCat;

    return 0;
}