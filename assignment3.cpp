#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


// Custom exception for negative deposit attempts
class NegativeDepositException : public std::runtime_error {
    public:
        NegativeDepositException() 
            : std::runtime_error("Negative deposit attempt") {}
    };
    

    // Custom exception for withdrawing more than the available balance
    class OverdrawException : public std::runtime_error {
    public:
        OverdrawException() 
            : std::runtime_error("Withdrawal amount exceeds current balance") {}
    };
    

    // Custom exception for transactions on a closed account
    class InvalidAccountOperationException : public std::runtime_error {
    public:
        InvalidAccountOperationException() 
            : std::runtime_error("Invalid operation on a closed account") {}
    };


class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;


public:
    // Constructor to initialize account number, balance, and active status
    BankAccount(std::string accNum, double initialBalance) : accountNumber(accNum), balance(initialBalance), isActive(true) {}


    // Deposit method with exception handling
    void deposit(double amount) {
        if (!isActive)
            throw InvalidAccountOperationException();
        if (amount < 0)
            throw NegativeDepositException();
        balance += amount;
    }


    // Withdraw method with exception handling
    void withdraw(double amount) {
        if (!isActive)
            throw InvalidAccountOperationException();
        if (amount > balance)
            throw OverdrawException();
        balance -= amount;
    }


    // Method to get current balance
    double getBalance() const {
        return balance;
    }


    // Method to close the account
    void closeAccount() {
        // TODO: Prevent further transactions when the account is closed
        isActive = false;
    }
};


int main() {
    try {
        double initialBalance;
        std::cout << "Enter initial balance: ";
        std::cin >> initialBalance; // prompt user for inital balance amount
        
        // create BankAccount object using a smart pointer
        auto account = std::make_unique<BankAccount>("123456", initialBalance);
        std::cout << "Bank Account Created: #" << "123456" << std::endl;

        // normal operations: deposit and withdraw
        std::cout << "Depositing $1000..." << std::endl;
        account->deposit(1000);

        std::cout << "Withdrawing $500..." << std::endl;
        account->withdraw(500);

        std::cout << "Current Balance: $" << account->getBalance() << std::endl;

        // Test: Negative deposit
        std::cout << "\nTesting negative deposit..." << std::endl;
        try {
            account->deposit(-100);
        } catch (const NegativeDepositException& e) {
            std::cerr << "Caught NegativeDepositException: " << e.what() << std::endl;
        }

        // Test: Invalid account operation
        std::cout << "\nClosing account..." << std::endl;
        account->closeAccount();
        std::cout << "Testing transaction on a closed account..." << std::endl;
        try {
            account->withdraw(100);
        } catch (const InvalidAccountOperationException& e) {
            std::cerr << "Caught InvalidAccountOperationException: " << e.what() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}