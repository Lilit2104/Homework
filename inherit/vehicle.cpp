#include <iostream>
#include <string>

class Vehicle{
    public:
        std::string brend;
        int year;
    public:
        Vehicle(std::string brend, int year) 
            : brend(brend), year(year) {}
    
        void printInfo() {
        std::cout << "Brend: " << brend << ", year: " << year<< std::endl;
        }
};

class Bus : public Vehicle{
    public:
        int benches;
        int passengercount = 0;
    public:
        Bus(std::string brend, int year,int benches) 
            : Vehicle(brend, year), benches(benches) {}

        void addPassenger()
        {
            passengercount += 1;
        } 

        void addPassenger(int count)
        {
            passengercount += count;
        } 
};


class Car : public Vehicle{
    public:
        int door;
    public:
        Car(std::string brend, int year,int door) 
            : Vehicle(brend, year), door(door) {}

        void openTrunk()
        {
            std::cout << "TRUK ISS OPEEN" << std::endl;
        }

        void  drive(int km)
        {
            std::cout << "I drive :  " << km << " km" <<std::endl;
        }
};


class Bicycle : public Vehicle{
    public:
        int gear;
    public:
        Bicycle(std::string brend, int year,int gear) 
            : Vehicle(brend, year), gear(gear) {}

        void ringBell()
        {
            std::cout << " RINGG RINGG RINGG~~" << std::endl;
        }

        void changeGear()
        {
            gear++;
        }
};

int main() {
    
    Car myCar("BNW", 2020, 4);
    myCar.printInfo();
    myCar.openTrunk();
    myCar.drive(150);


    
    Bicycle myBike("Giant", 2022, 21);
    myBike.printInfo();
    myBike.ringBell();
    myBike.changeGear();
    std::cout << "Gear: " << myBike.gear << std::endl;


    
    Bus myBus("Mercedes", 2019, 30);
    myBus.printInfo();
    myBus.addPassenger();        
    myBus.addPassenger(5);       
    std::cout << "Passenger count: " << myBus.passengercount << std::endl;

    return 0;
}