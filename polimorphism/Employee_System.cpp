#include <iostream>
#include <string>

class Employee{
    public:
        std::string name;
    public:
        Employee(std::string empName) : name(empName) {}

        virtual double calculateSalary()
        {
            return 0.0;
        }

        virtual ~Employee() = default;
};


class FullTimeEmployee : public Employee{
    public:
        double monthlySalary;
    public:
        FullTimeEmployee(std::string empName, double salary)
            : Employee(empName), monthlySalary(salary) {}

        virtual double calculateSalary() override
        {
            return monthlySalary;
        }
};

class HourlyEmployee : public Employee{
    public:
        double hourlyRate;
        double hoursWorked;
    public:
        HourlyEmployee(std::string empName, double rate, double hours)
            : Employee(empName), hourlyRate(rate), hoursWorked(hours) {}

        virtual double calculateSalary() override
        {
            return hourlyRate * hoursWorked;
        }
};

int main()
{
    Employee* emp1 = new FullTimeEmployee("Anna", 450000.0);
    Employee* emp2 = new HourlyEmployee("Davit", 2500.0, 160.0);

    std::cout << emp1->name << " - Salary: " << emp1->calculateSalary() << std::endl;
    std::cout << emp2->name << " - Salary: " << emp2->calculateSalary() << std::endl;

    delete emp1;
    delete emp2;

    return 0;
}