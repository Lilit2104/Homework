#include <iostream>
#include <stdexcept>

class Temperature {
private:
    double* celsius;
    double* fahrenheit;
    double* kelvin;

    void syncFromCelsius(double c)
    {
        double k = c + 273.15;
        if (k < 0)
            throw std::invalid_argument("Kelvin cannot be negative");

        *celsius = c;
        *fahrenheit = c * 9 / 5 + 32;
        *kelvin = k;
    }

public:
    Temperature(double c = 0)
        : celsius(new double(0)), fahrenheit(new double(0)), kelvin(new double(0))
    {
        syncFromCelsius(c);
    }

    Temperature(const Temperature& other)
        : celsius(new double(*other.celsius)),
          fahrenheit(new double(*other.fahrenheit)),
          kelvin(new double(*other.kelvin))
    {
        std::cout << "copy ctor" << std::endl;
    }

    Temperature(Temperature&& other) noexcept
        : celsius(other.celsius),
          fahrenheit(other.fahrenheit),
          kelvin(other.kelvin)
    {
        std::cout << "move ctor" << std::endl;
        other.celsius = nullptr;
        other.fahrenheit = nullptr;
        other.kelvin = nullptr;
    }

    Temperature& operator=(const Temperature& other)
    {
        std::cout << "copy assign" << std::endl;
        if (this == &other)
            return *this;

        *celsius = *other.celsius;
        *fahrenheit = *other.fahrenheit;
        *kelvin = *other.kelvin;

        return *this;
    }

    Temperature& operator=(Temperature&& other) noexcept
    {
        std::cout << "move assign" << std::endl;
        if (this == &other)
            return *this;

        delete celsius;
        delete fahrenheit;
        delete kelvin;

        celsius = other.celsius;
        fahrenheit = other.fahrenheit;
        kelvin = other.kelvin;

        other.celsius = nullptr;
        other.fahrenheit = nullptr;
        other.kelvin = nullptr;

        return *this;
    }

    ~Temperature()
    {
        delete celsius;
        delete fahrenheit;
        delete kelvin;
    }

    void setCelsius(double value)
    {
        syncFromCelsius(value);
    }

    double getCelsius() const { return *celsius; }
    double getFahrenheit() const { return *fahrenheit; }
    double getKelvin() const { return *kelvin; }

    Temperature& operator++()
    {
        syncFromCelsius(*celsius + 1);
        return *this;
    }

    Temperature operator++(int)
    {
        Temperature tmp(*this);
        syncFromCelsius(*celsius + 1);
        return tmp;
    }

    Temperature& operator--()
    {
        syncFromCelsius(*celsius - 1);
        return *this;
    }

    Temperature operator--(int)
    {
        Temperature tmp(*this);
        syncFromCelsius(*celsius - 1);
        return tmp;
    }

    Temperature operator+(const Temperature& other) const
    {
        return Temperature(*celsius + *other.celsius);
    }

    double operator-(const Temperature& other) const
    {
        return *celsius - *other.celsius;
    }

    bool operator==(const Temperature& other) const { return *celsius == *other.celsius; }
    bool operator!=(const Temperature& other) const { return !(*this == other); }
    bool operator<(const Temperature& other) const { return *celsius < *other.celsius; }
    bool operator>(const Temperature& other) const { return *celsius > *other.celsius; }
    bool operator<=(const Temperature& other) const { return !(*this > other); }
    bool operator>=(const Temperature& other) const { return !(*this < other); }

    friend std::ostream& operator<<(std::ostream& os, const Temperature& obj)
    {
        os << "Celsius: " << *obj.celsius << "\n"
           << "Fahrenheit: " << *obj.fahrenheit << "\n"
           << "Kelvin: " << *obj.kelvin << "\n";
        return os;
    }
};

int main()
{
    Temperature t1(25);
    std::cout << t1 << std::endl;

    Temperature t2 = t1;
    Temperature t3 = std::move(t1);

    t2.setCelsius(10);
    std::cout << "t2 after setCelsius(10):\n" << t2 << std::endl;

    ++t2;
    t2++;
    std::cout << "t2 after ++t2 and t2++:\n" << t2 << std::endl;

    Temperature t4 = t2 + t3;
    std::cout << "t2 + t3:\n" << t4 << std::endl;

    std::cout << "t2 - t3 = " << (t2 - t3) << " degrees C\n";

    std::cout << "t2 == t3: " << (t2 == t3) << std::endl;
    std::cout << "t2 > t3:  " << (t2 > t3) << std::endl;


    return 0;
}