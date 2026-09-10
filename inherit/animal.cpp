#include <iostream>
#include <string>

class Animal {
    public:
        std::string name;
        int age;

    public:
        Animal(std::string name, int age) 
            : name(name), age(age) {}

        void printInfo() 
        {
            std::cout << "Name: " << name << ", Age: " << age << std::endl;
        }

        void eat() 
        {
            std::cout << name << " is eating." << std::endl;
        }
};

class Dog : public Animal {
    public:
        std::string breed;

    public:
        Dog(std::string name, int age, std::string breed)
            : Animal(name, age), breed(breed) {}

    
        void bark()
        {
            std::cout << name << " (" << breed << "): HAAF-HAAF!" << std::endl;
        }

   
        void bark(int times)
        {
            for (int i = 0; i < times; i++) {
                std::cout << name << " (" << breed << "): HAAF-HAAF! ";
            }
            std::cout << std::endl;
        }
};

class Cat : public Animal {
    public:
        std::string colour;

    public:
        Cat(std::string name, int age, std::string colour)
            : Animal(name, age), colour(colour) {}

        void sleep() 
        {
            std::cout << name << " is sleeping. Zzzzz~" << std::endl;
        }

        void jump(int height) 
        {
            std::cout << name << " jumped " << height << " cm high." << std::endl;
        }
};

class Bird : public Animal {
    public:
        int wingspan;

    public:
        Bird(std::string name, int age, int wingspan)
            : Animal(name, age), wingspan(wingspan) {}

    
        void fly()
        {
            std::cout << name << " is flying." << std::endl;
        }

   
        void fly(int distance) 
        {
            std::cout << name << " flew " << distance << " km." << std::endl;
        }
};

int main() {
    Dog dog("Rex", 3, "Labrador");
    dog.printInfo();
    dog.eat();
    dog.bark();
    dog.bark(3);

    std::cout << std::endl;

    Cat cat("Whiskers", 2, "Black");
    cat.printInfo();
    cat.eat();
    cat.sleep();
    cat.jump(40);

    std::cout << std::endl;

    Bird bird("Tweety", 1, 25);
    bird.printInfo();
    bird.eat();
    bird.fly();
    bird.fly(15);

    return 0;
}