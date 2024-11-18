#ifndef COMPANY_HPP
#define COMPANY_HPP

#include "Employee.hpp"
#include "Constants.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>

class Company {
private:
    double funds;                                // fundusze
    std::vector<std::unique_ptr<Employee>> employees; // pracownicy
    int storedProducts = 0;                     // liczba produktow w magazynie
    std::vector<double> monthlyRevenues;        // miesieczne przychody

    struct Loan {
        double amount;
        double monthlyPayment;
        int monthsRemaining;
    };
    std::vector<Loan> loans;                    // pozyczki

    // Statystyki gry
   
    int totalProducedProducts = 0;              // wyprodukowanych produktów w calej grze
    int totalSoldProducts = 0;                  // liczba sprzedanych produktów w calej grze
    double totalRevenue = 0;                    // Łączny przychód

    // Generowanie losowych danych dla pracownikow
    std::string generateRandomName() const {
        std::vector<std::string> firstNames = {"John", "Emily", "Michael", "Sarah", "David", "Emma"};
        std::vector<std::string> lastNames = {"Smith", "Johnson", "Brown", "Taylor", "Anderson", "Thomas"};
        return firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
    }

    std::string generateRandomFaculty() const {
        std::vector<std::string> faculties = {"Computer Science", "Mechanical Engineering", "Electrical Engineering"};
        return faculties[rand() % faculties.size()];
    }

    bool generateRandomForkliftCertified() const {
        return rand() % 2 == 0;
    }

    int generateRandomFollowers() const {
        return rand() % 10001;
    }

    double generateRandomShoeSize() const {
        return 35.0 + static_cast<double>(rand() % 150) / 10.0;
    }

public:
    // Konstruktor
    Company() : funds(STARTING_FUNDS) {
        srand(static_cast<unsigned>(time(0))); // Inicjalizacja generatora losowego
        hireEngineer();
        hireWarehouseWorker();
        hireMarketer();
        hireWorker();
    }

     int totalTurns = 0;                         // liczba tur/msc

    // Zatrudnianie pracowników
    void hireEngineer() {
        std::string name = generateRandomName();
        std::string faculty = generateRandomFaculty();
        employees.push_back(std::make_unique<Engineer>(name, ENGINEER_SALARY, faculty));
        funds -= ENGINEER_SALARY;
        std::cout << "Hired Engineer: " << name << ", Faculty: " << faculty << "\n";
    }

    void hireWarehouseWorker() {
        std::string name = generateRandomName();
        bool forkliftCertified = generateRandomForkliftCertified();
        employees.push_back(std::make_unique<WarehouseWorker>(name, WAREHOUSE_WORKER_SALARY, forkliftCertified));
        funds -= WAREHOUSE_WORKER_SALARY;
        std::cout << "Hired Warehouse Worker: " << name
                  << ", Forklift Certified: " << (forkliftCertified ? "Yes" : "No") << "\n";
    }

    void hireMarketer() {
        std::string name = generateRandomName();
        int followers = generateRandomFollowers();
        employees.push_back(std::make_unique<Marketer>(name, MARKETER_SALARY, followers));
        funds -= MARKETER_SALARY;
        std::cout << "Hired Marketer: " << name << ", Followers: " << followers << "\n";
    }

    void hireWorker() {
        std::string name = generateRandomName();
        double shoeSize = generateRandomShoeSize();
        employees.push_back(std::make_unique<Worker>(name, WORKER_SALARY, shoeSize));
        funds -= WORKER_SALARY;
        std::cout << "Hired Worker: " << name << ", Shoe Size: " << shoeSize << "\n";
    }

    // Wyświetlanie listy pracowników
    void listEmployees() const {
        std::cout << "Employees:\n";
        for (const auto& e : employees) {
            e->displayInfo();
        }
    }

    // Operacje miesięczne
    void processMonthlyFinances() {
        produce();
        sell();
        calculateMonthlyCosts();
        payLoans();
        

        // warning jesli na minusie
        if (funds < 0) {
            std::cout << "Warning: You are in debt! Current funds: $" << funds << "\n";
        }
    }

    // Koszty miesięczne - pensje pracownikow
    void calculateMonthlyCosts() {
        double totalSalary = 0;

        for (const auto& employee : employees) {
            totalSalary += employee->getSalary();
        }

        funds -= totalSalary;

        std::cout << "Monthly costs: $" << totalSalary << "\n";
    }

    void produce() {
    int totalProduction = 0;
    int warehouseCapacity = 0;

    // obliczanie pojemności magazynu
    for (const auto& employee : employees) {
        if (auto warehouseWorker = dynamic_cast<WarehouseWorker*>(employee.get())) {
            warehouseCapacity += WAREHOUSE_CAPACITY;
        }
    }

    // obliczanie produkcji
    for (const auto& employee : employees) {
        if (auto worker = dynamic_cast<Worker*>(employee.get())) {
            totalProduction += WORKER_SUPPLY;
        }
    }

    // sprawdzanie pojemności na przyszły miesiąc - do info o firmie
    int availableCapacity = warehouseCapacity - storedProducts;
    if (totalProduction > availableCapacity) {
        std::cout << "Warning: Not enough storage capacity for next month's production!\n";
        std::cout << "Production: " << totalProduction 
                  << ", Available storage: " << availableCapacity << "\n";
    }

    // produkcja na ten miesiąc - do info o firmie
    int productsToStore = std::min(totalProduction, availableCapacity);
    storedProducts += productsToStore;
    totalProducedProducts += productsToStore;

    std::cout << "Produced " << productsToStore << " products. Total in storage: " << storedProducts << "\n";
}


    // sprzedaz
    void sell() {
        int totalDemand = 0;
        double productPrice = 0;

        for (const auto& employee : employees) {
            if (auto marketer = dynamic_cast<Marketer*>(employee.get())) {
                totalDemand += MARKETER_DEMAND;
            }
            if (auto engineer = dynamic_cast<Engineer*>(employee.get())) {
                productPrice += ENGINEER_PRODUCTIVITY;
            }
        }

        int productsSold = std::min(totalDemand, storedProducts);
        double revenue = productsSold * productPrice;

        storedProducts -= productsSold;
        funds += revenue;
        totalSoldProducts += productsSold;
        totalRevenue += revenue;

        monthlyRevenues.push_back(revenue);

        std::cout << "Sold " << productsSold << " products at $" << productPrice
                  << " each. Revenue: $" << revenue << "\n";
    }

    // pozyczka
    void takeLoan(double amount, int months) {
        if (months > 12) {
            std::cout << "Maximum loan duration is 12 months.\n";
            return;
        }
        double monthlyPayment = (amount + (amount * 0.05 * months)) / months;
        loans.push_back({amount, monthlyPayment, months});
        funds += amount;
        std::cout << "Loan of $" << amount << " taken. Monthly payment: $" << monthlyPayment << "\n";
    }
    //splata pozyczki w turze
    void payLoans() {
        double totalLoanPayment = 0;

        for (auto& loan : loans) {
            totalLoanPayment += loan.monthlyPayment;
            loan.monthsRemaining--;
        }

        funds -= totalLoanPayment;
        loans.erase(std::remove_if(loans.begin(), loans.end(),
                                   [](const Loan& loan) { return loan.monthsRemaining <= 0; }),
                    loans.end());

        std::cout << "Total loan payments: $" << totalLoanPayment << "\n";
    }

    void displayLoans() const {
        std::cout << "Active Loans:\n";
        for (const auto& loan : loans) {
            std::cout << "Amount: $" << loan.amount
                      << ", Monthly Payment: $" << loan.monthlyPayment
                      << ", Months Remaining: " << loan.monthsRemaining << "\n";
        }
    }

    // obliczanie wartosci firmy
    double calculateCompanyValue() const {
    if (monthlyRevenues.empty()) return 0.0; // jesli brak przychodu, wartość firmy to 0

    // liczba miesiecy do obliczania wartosci firmy (zapisana w stalych)
    int monthsToConsider = std::min(COMPANY_VALUE_PERIOD, static_cast<int>(monthlyRevenues.size()));

    // obliczanie średniego przychodu z ostatnich N msc
    double totalRevenueForPeriod = 0.0;
    for (int i = monthlyRevenues.size() - monthsToConsider; i < monthlyRevenues.size(); ++i) {
        totalRevenueForPeriod += monthlyRevenues[i];
    }

    return totalRevenueForPeriod / monthsToConsider;
    }

    // Getter dla funduszy
    double getFunds() const { return funds; }

    //Komenda wysweitlanie informacji, duzo info zeby zaplanowac rozgrywke
    void displayCompanyInfo() const {
        int numEngineers = 0, numWarehouseWorkers = 0, numMarketers = 0, numWorkers = 0;
        int warehouseCapacity = 0, totalDemand = 0, totalProduction = 0;
        double productPrice = 0;
        double totalSalaries = 0;
        double projectedRevenue = 0;

        // Pracownicy
        for (const auto& employee : employees) {
            if (auto engineer = dynamic_cast<Engineer*>(employee.get())) {
                numEngineers++;
                productPrice += ENGINEER_PRODUCTIVITY;
            }
            if (auto warehouseWorker = dynamic_cast<WarehouseWorker*>(employee.get())) {
                numWarehouseWorkers++;
                warehouseCapacity += WAREHOUSE_CAPACITY;
            }
            if (auto marketer = dynamic_cast<Marketer*>(employee.get())) {
                numMarketers++;
                totalDemand += MARKETER_DEMAND;
            }
            if (auto worker = dynamic_cast<Worker*>(employee.get())) {
                numWorkers++;
                totalProduction += WORKER_SUPPLY;
            }
            totalSalaries += employee->getSalary(); // Suma pensji
        }

        // Prognozowany przcyhod na koniec tury (uwzglednia operacje wewnatrz miesiaca)
        int productsToSell = std::min(totalDemand, totalProduction+totalProducedProducts);
        projectedRevenue = productsToSell * productPrice;

        // info display
        std::cout << "\n=== Company Information ===\n";
        std::cout << "Funds: $" << funds << "\n";
        std::cout << "Value of the Company: $" << calculateCompanyValue() << "\n"; // Wartość firmy
        std::cout << "Number of Employees:\n";
        std::cout << "  Engineers: " << numEngineers << "\n";
        std::cout << "  Warehouse Workers: " << numWarehouseWorkers << "\n";
        std::cout << "  Marketers: " << numMarketers << "\n";
        std::cout << "  Workers: " << numWorkers << "\n";
        std::cout << "Total Monthly Salaries: $" << totalSalaries << "\n"; // Suma wynagrodzeń
        std::cout << "Projected Revenue: $" << projectedRevenue << "\n"; // Prognozowany przychód
        std::cout << "Warehouse Capacity: " << warehouseCapacity << " units\n";
        std::cout << "Products in Storage: " << storedProducts << "\n";
        std::cout << "Product Price: $" << productPrice << "\n";
        std::cout << "Total Demand: " << totalDemand << " units\n";
        std::cout << "Total Production (per month): " << totalProduction << " units\n";
        std::cout << "===========================\n";

        
        if (!loans.empty()) {
            std::cout << "Loans:\n";
            for (const auto& loan : loans) {
                std::cout << "  Loan Amount: $" << loan.amount
                          << ", Monthly Payment: $" << loan.monthlyPayment
                          << ", Months Remaining: " << loan.monthsRemaining << "\n";
            }
        } else {
            std::cout << "No active loans.\n";
        }

        std::cout << "===========================\n";
    }

    //sprawdzanie czy nie produkujemy zbyt duzo, info dla gracza
    void checkStorageWarning() const {
    int totalProduction = 0;
    int warehouseCapacity = 0;

    
    for (const auto& employee : employees) {
        if (auto warehouseWorker = dynamic_cast<WarehouseWorker*>(employee.get())) {
            warehouseCapacity += WAREHOUSE_CAPACITY;
        }
        if (auto worker = dynamic_cast<Worker*>(employee.get())) {
            totalProduction += WORKER_SUPPLY;
        }
    }

    int availableCapacity = warehouseCapacity - storedProducts;
    if (totalProduction > availableCapacity) {
        std::cout << "\n=== Warning: Storage Capacity Issue ===\n";
        std::cout << "Production for next month: " << totalProduction << "\n";
        std::cout << "Available storage capacity: " << availableCapacity << "\n";
        std::cout << "Some products may not be stored. Consider hiring more warehouse workers.\n";
        std::cout << "=======================================\n";
    }
    }
    //koncowe staty
    void displayEndGameStats() const {
        std::cout << "\n=== End Game Statistics ===\n";
        std::cout << "Total Turns: " << totalTurns << "\n";
        std::cout << "Total Produced Products: " << totalProducedProducts << "\n";
        std::cout << "Total Sold Products: " << totalSoldProducts << "\n";
        std::cout << "Average Product Price: $"
                  << (totalSoldProducts > 0 ? totalRevenue / totalSoldProducts : 0) << "\n";
        std::cout << "===========================\n";
    }
};

#endif // COMPANY_HPP
