#include <iostream>
#include <string>
#include <vector>

class Vehicles{
    public:
        virtual void startEngine() 
        {
            std::cout << "Շարժիչը միացված է" << std::endl;
        }
};

class Car : public Vehicles{
    public:
       virtual void startEngine() override
       {
            std::cout << "Մեքենայի շարժիչը գործարկվեց բանալիով" << std::endl;
       }
};

class Electriccar : public Vehicles{
    public:
        virtual void startEngine() override
        {
           std::cout << " Էլեկտրամեքենան միացավ լուռ (լիցքը պատրաստ է)" << std::endl;
        }
};

void testVehicle(Vehicles* v)
{
    v->startEngine();
}

int main()
{
    Car car;
    Electriccar electricCar;

    testVehicle(&car);
    testVehicle(&electricCar);

    return 0;
}