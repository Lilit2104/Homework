#include <iostream>
#include <string>
#include <vector>
#include <memory>



class INotificationSender {
public:
    virtual void send(const std::string& message) const = 0;
    virtual ~INotificationSender() = default;
};

class EmailNotification : public INotificationSender {
public:
    void send(const std::string& message) const override 
    {
        std::cout << "[EMAIL] " << message << '\n';
    }
};

class SMSNotification : public INotificationSender {
public:
    void send(const std::string& message) const override 
    {
        std::cout << "[SMS] " << message << '\n';
    }
};



class ISwitchable {
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~ISwitchable() = default;
};

class ITemperatureControl {
public:
    virtual void setTemperature(double temperature) = 0;
    virtual ~ITemperatureControl() = default;
};

class ICamera {
public:
    virtual void startRecording() = 0;
    virtual void stopRecording() = 0;
    virtual ~ICamera() = default;
};

class ILockControl {
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual ~ILockControl() = default;
};

class ISensor {
public:
    virtual bool detect() const = 0;
    virtual ~ISensor() = default;
};



class SmartLight : public ISwitchable {
public:
    void turnOn() override 
    {
        std::cout << "Light is ON\n";
    }

    void turnOff() override 
    {
        std::cout << "Light is OFF\n";
    }
};

class Thermostat : public ITemperatureControl {
public:
    void setTemperature(double temperature) override 
    {
        std::cout << "Temperature set to "
                  << temperature << " degrees\n";
    }
};

class Camera : public ICamera {
public:
    void startRecording() override 
    {
        std::cout << "Camera recording started\n";
    }

    void stopRecording() override 
    {
        std::cout << "Camera recording stopped\n";
    }
};

class SmartLock : public ILockControl {
public:
    void lock() override 
    {
        std::cout << "Door locked\n";
    }

    void unlock() override 
    {
        std::cout << "Door unlocked\n";
    }
};

class WaterLeakSensor : public ISensor {
private:
    bool waterDetected;

public:
    explicit WaterLeakSensor(bool detected) : waterDetected(detected) {}
    bool detect() const override 
    {
        return waterDetected;
    }
};



class IAutomationRule {
public:
    virtual void execute() const = 0;
    virtual ~IAutomationRule() = default;
};

class LightAutomationRule : public IAutomationRule {
private:
 ISwitchable& light;

public:
    explicit LightAutomationRule( ISwitchable& light) : light(light) {}

    void execute() const override 
    {
        light.turnOn();
    }
};

class TemperatureAutomationRule : public IAutomationRule {
private:
    ITemperatureControl& thermostat;
    double targetTemperature;

public:
    TemperatureAutomationRule( ITemperatureControl& thermostat, double targetTemperature )
        : thermostat(thermostat), targetTemperature(targetTemperature) {}

    void execute() const override 
    {
        thermostat.setTemperature(targetTemperature);
    }
};

class LeakDetectionRule : public IAutomationRule {
private:
    const ISensor& sensor;
    const INotificationSender& notifier;
    ILockControl& doorLock;

public:
    LeakDetectionRule( const ISensor& sensor, const INotificationSender& notifier, ILockControl& doorLock)
        : sensor(sensor), notifier(notifier), doorLock(doorLock) {}

    void execute() const override 
    {
        if (sensor.detect()) 
        {
            doorLock.lock();

            notifier.send( "WARNING! Water leakage detected!" );
        }
        else 
        {
            std::cout << "No water leakage detected\n";
        }
    }
};

class SmartHome {
private:
    std::vector<std::unique_ptr<IAutomationRule>> rules;

public:
    void addRule(std::unique_ptr<IAutomationRule> rule) 
    {
        rules.push_back(std::move(rule));
    }

    void executeRules() const
    {
        for (const auto& rule : rules)
        {
            rule->execute();
        }
    }
};


int main() {
    SmartLight light;
    Thermostat thermostat;
    Camera camera;
    SmartLock doorLock;
    WaterLeakSensor waterSensor(true);

    EmailNotification email;

    camera.startRecording();

    SmartHome home;

    home.addRule(
        std::make_unique<LightAutomationRule>(light)
    );

    home.addRule( std::make_unique<TemperatureAutomationRule>( thermostat, 22.0));

    home.addRule(
        std::make_unique<LeakDetectionRule>( waterSensor, email, doorLock ));

    home.executeRules();

    return 0;
}