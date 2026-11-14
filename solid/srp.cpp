#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>

class Employee {
private:
    int id;
    std::string name;
    double baseSalary;
    int workingHours;

public:
    Employee(int id, const std::string& name,double baseSalary, int workingHours)
        : id(id), name(name), baseSalary(baseSalary), workingHours(workingHours) {}

    int getId() const 
    {
        return id;
    }    

    std::string getName() const 
    {
        return name;
    }

    double getBaseSalary() const 
    {
        return baseSalary;
    }

    int getWorkingHours() const 
    {
        return workingHours;
    }
};

class SalaryCalculator {
public:
    double calculate(const Employee& employee) const 
    {
        return employee.getBaseSalary() * employee.getWorkingHours();
    }
};




class EmployeeRepository {
public:
    void save(const Employee& employee,const std::string& filename) const 
    {
        std::ofstream file(filename, std::ios::app);

        if (!file.is_open()) 
        {
            throw std::runtime_error("Cannot open file");
        }

        file << employee.getId() << ","
             << employee.getName() << ","
             << employee.getBaseSalary() << ","
             << employee.getWorkingHours() << '\n';
    }
};


class PayrollReport {
public:
    std::string generate(const Employee& employee, double salary) const 
    {
        std::ostringstream report;

        report << "Employee ID: " << employee.getId() << '\n'
               << "Name: " << employee.getName() << '\n'
               << "Salary: " << salary << '\n';

        return report.str();
    }
};

class EmailSender {
public:
    void send(const std::string& email, const std::string& report) const 
    {
        std::cout << "Sending email to: " << email << '\n';
        std::cout << report << '\n';
    }
};


int main() {
    try 
    {
        Employee employee(1, "John Doe", 20.0, 40);
        SalaryCalculator calculator;
        double salary = calculator.calculate(employee);

        std::cout << "Employee ID: " << employee.getId() << std::endl;
        std::cout << "Employee Name: " << employee.getName() << std::endl;
        std::cout << "Base Salary: $" << employee.getBaseSalary() << std::endl;
        std::cout << "Working Hours: " << employee.getWorkingHours() << std::endl;
        std::cout << "Calculated Salary: $" << salary << std::endl;

        EmployeeRepository repository;
        repository.save(employee, "employees.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}