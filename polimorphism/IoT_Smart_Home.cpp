#include <iostream>
#include <string>

class Sensor{
    public:
        virtual double readData()
        {
            return 0.0;
        }
        
        virtual std::string getType()
        {
            return "Անհայտ սենսոր";
        }

        virtual ~Sensor() = default;
};

class TemperatureSensor : public Sensor{
    public:
        virtual double readData() override
        {
            return 24.5;
        }

        virtual std::string getType() override
        {
            return "Ջերմաստիճան";
        }
};

class MotionSensor : public Sensor{
    public:
        virtual double readData() override
        {
            return 1.0;
        }

        virtual std::string getType() override
        {
            return "Շարժման սենսոր";
        }
};

int main()
{
    const int sensorCount = 2;
    Sensor* sensors[sensorCount];

    sensors[0] = new TemperatureSensor();
    sensors[1] = new MotionSensor();

    for(int i = 0; i < sensorCount; i++)
    {
        std::cout << sensors[i]->getType() << ": " << sensors[i]->readData() << std::endl;
    }

    for(int i = 0; i < sensorCount; i++)
    {
        delete sensors[i];
    }

    return 0;
}