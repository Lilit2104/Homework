#include <iostream>
#include <utility>

class Date
{
private:
    int* hour;
    int* minute;

public:
   
    Date(int h = 0, int m = 0)
        : hour(new int(h)), minute(new int(m))
    {
    }

   
    Date(const Date& other)
        : hour(new int(*other.hour)),
          minute(new int(*other.minute))
    {
        std::cout << "Copy ctor" << std::endl;
    }

    
    Date(Date&& other) noexcept
        : hour(other.hour),
          minute(other.minute)
    {
        std::cout << "Move ctor" << std::endl;

        other.hour = nullptr;
        other.minute = nullptr;
    }

    
    Date& operator=(const Date& other)
    {
        std::cout << "Copy assign" << std::endl;

        if (this == &other)
            return *this;

        *hour = *other.hour;
        *minute = *other.minute;

        return *this;
    }

    
    Date& operator=(Date&& other) noexcept
    {
        std::cout << "Move assign" << std::endl;

        if (this == &other)
            return *this;

        delete hour;
        delete minute;

        hour = other.hour;
        minute = other.minute;

        other.hour = nullptr;
        other.minute = nullptr;

        return *this;
    }


    ~Date()
    {
        delete hour;
        delete minute;
    }

    
    Date& operator++()
    {
        ++(*hour);
        if (*hour >= 24)
            *hour %= 24;
        return *this;
    }

   
    Date operator++(int)
    {
        Date temp(*this);
        ++(*hour);
        if (*hour >= 24)
            *hour %= 24;
        return temp;
    }

    
    Date& operator--()
    {
        --(*hour);
        if (*hour < 0)
            *hour = 23;
        return *this;
    }

    
    Date operator--(int)
    {
        Date temp(*this);
        --(*hour);
        if (*hour < 0)
            *hour = 23;
        return temp;
    }

   
    Date operator+(const Date& other) const
    {
        int newHour = *hour + *other.hour;
        int newMinute = *minute + *other.minute;

        if (newMinute >= 60)
        {
            newHour += newMinute / 60;
            newMinute %= 60;
        }

        newHour %= 24;

        return Date(newHour, newMinute);
    }

    
    friend std::ostream& operator<<(std::ostream& os, const Date& obj)
    {
        os << "Hour " << *obj.hour << " Minute " << *obj.minute;
        return os;
    }
};

int main()
{
    Date d1;
    Date d2(23, 50);
    std::cout << d1 << std::endl;
    std::cout << d2 << std::endl;

    ++d2;
    std::cout << d2 << std::endl;
    d2++;
    std::cout << d2 << std::endl;
    --d2;
    std::cout << d2 << std::endl;
    d2--;
    std::cout << d2 << std::endl;

    Date d3(10, 30);
    Date d4(5, 40);

    Date d5 = d3 + d4;
    std::cout << d5 << std::endl;

    Date d6 = d5;   
    Date d7;
    d7 = d5;       

    Date d8 = std::move(d5);  
    Date d9;
    d9 = std::move(d8);       

    return 0;
}