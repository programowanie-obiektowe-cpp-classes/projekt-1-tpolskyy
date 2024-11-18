#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include <string>
#include <iostream>

class Employee {
protected:
    std::string name;
    double salary;

public:
    Employee(const std::string& name, double salary)
        : name(name), salary(salary) {}

    virtual void displayInfo() const = 0;
    virtual double getSalary() const { return salary; }
    virtual ~Employee() {}
};

class Engineer : public Employee {
private:
    std::string faculty;

public:
    Engineer(const std::string& name, double salary, const std::string& faculty)
        : Employee(name, salary), faculty(faculty) {}

    void displayInfo() const override {
        std::cout << "Engineer: " << name << ", Faculty: " << faculty << ", Salary: " << salary << "\n";
    }
};

class WarehouseWorker : public Employee {
private:
    bool forkliftCertified;

public:
    WarehouseWorker(const std::string& name, double salary, bool forkliftCertified)
        : Employee(name, salary), forkliftCertified(forkliftCertified) {}

    void displayInfo() const override {
        std::cout << "Warehouse Worker: " << name << ", Forklift Certified: " 
                  << (forkliftCertified ? "Yes" : "No") << ", Salary: " << salary << "\n";
    }
};

class Marketer : public Employee {
private:
    int followers;

public:
    Marketer(const std::string& name, double salary, int followers)
        : Employee(name, salary), followers(followers) {}

    void displayInfo() const override {
        std::cout << "Marketer: " << name << ", Followers: " << followers << ", Salary: " << salary << "\n";
    }
};

class Worker : public Employee {
private:
    double shoeSize;

public:
    Worker(const std::string& name, double salary, double shoeSize)
        : Employee(name, salary), shoeSize(shoeSize) {}

    void displayInfo() const override {
        std::cout << "Worker: " << name << ", Shoe Size: " << shoeSize << ", Salary: " << salary << "\n";
    }
};

#endif // EMPLOYEE_HPP