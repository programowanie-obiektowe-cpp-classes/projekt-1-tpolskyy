#include "Company.hpp"
#include <iostream>

int main() {
    Company company;

    std::cout << "Welcome to the economic simulation game!\n";
    std::cout << "You are the manager of a company. Your goal is to make your company worth $100,000 or more.\n";
    std::cout << "Starting funds: $" << company.getFunds() << "\n";

    while (true) {
        // Sprawdzenie ostrzeżenia o magazynie po każdej akcji, czy liczba produktow przekracza liczbe miejsca
        company.checkStorageWarning();
        
        std::cout << "\nOptions:\n"
                  << "1. List employees\n"
                  << "2. Hire Engineer ($" << ENGINEER_SALARY << " per month)\n"
                  << "3. Hire Warehouse Worker ($" << WAREHOUSE_WORKER_SALARY << " per month)\n"
                  << "4. Hire Marketer ($" << MARKETER_SALARY << " per month)\n"
                  << "5. Hire Worker ($" << WORKER_SALARY << " per month)\n"
                  << "6. End Month\n"
                  << "7. Take Loan\n"
                  << "8. View Loans\n"
                  << "9. View Company Information\n"
                  << "10. Exit\n"
                  << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                company.listEmployees();
                break;
            }
            case 2: {
                company.hireEngineer();
                break;
            }
            case 3: {
                company.hireWarehouseWorker();
                break;
            }
            case 4: {
                company.hireMarketer();
                break;
            }
            case 5: {
                company.hireWorker();
                break;
            }
            case 6: {
                company.totalTurns++;
                company.payLoans();
                company.produce();
                company.sell();
                company.calculateMonthlyCosts();
                company.displayCompanyInfo();

                if (company.getFunds() < 0) {
                    std::cout << "You have gone bankrupt. Game over.\n";
                    company.displayEndGameStats();
                    return 0;
                }

                if (company.calculateCompanyValue() >= VICTORY_VALUE) {
                    std::cout << "Congratulations! Your company is worth $" << VICTORY_VALUE << " or more. You win!\n";
                    company.displayEndGameStats();
                    return 0;
                }
                break;
            }
            case 7: {
                double amount;
                int months;
                std::cout << "Max loan amount: " << company.getMaxAllowedDebt() << ".\n" ;
                std::cout << "Enter loan amount: ";
                std::cin >> amount;
                std::cout << "Max repayment duration: "<< MAX_LOAN_MONTHS << ".\n" ;
                std::cout << "Enter repayment duration: ";
                std::cin >> months;
                company.takeLoan(amount, months);
                break;
            }
            case 8: {
                company.displayLoans();
                break;
            }
            case 9: {
                company.displayCompanyInfo();
                break;
            }
            case 10: {
                std::cout << "Exiting the game. Thanks for playing!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        
        }
    }

    return 0;
}