#include <iostream>
#include <string>

class Bankaccount {
    public:
        std::string ownername;
        float bank_balance;
        float limit;

    public:
        Bankaccount(std::string ownername, float bank_balance, float limit)
            : ownername(ownername), bank_balance(bank_balance), limit(limit) {}

        void deposit()
        {
            std::cout << "I transfer a money" << std::endl;
        }

        void deposit(float n)
        {
            bank_balance += n;
        }

        void deposit(int n)
        {
            bank_balance += n;
        }

        void withdraw(float amount)
        {
            if (amount > bank_balance)
            {
                std::cout << "insufficient funds" << std::endl;
            }
            else
            {
                bank_balance -= amount;
                std::cout << "Done! " << amount << std::endl;
            }
        }
};

class SavingsAccount : public Bankaccount {
    public:
        float rate;
    public:
        SavingsAccount(std::string ownername, float bank_balance, float rate, float limit) 
            : Bankaccount(ownername, bank_balance, limit), rate(rate) {}

        void calculateInterest()
        {
            std::cout << "I have calculated your interest" << std::endl;
        }
};

class CheckingAccount : public Bankaccount {
    public:
        int transaction_count;

    public:
        CheckingAccount(std::string ownername, float bank_balance, int transaction_count, float limit) 
            : Bankaccount(ownername, bank_balance, limit), transaction_count(transaction_count) {}

        void addTransaction(int n)
        {
            transaction_count += n;
            std::cout << "Added transaction: " << transaction_count << std::endl;
        }

        void isLimitReached()
        {
            if (transaction_count > limit)
            {
                std::cout << "You reached limit" << std::endl;
            }
            else
            {
                std::cout << "You can transfer more money" << std::endl;
            }
        }
};

int main()
{
    SavingsAccount sa("Aram", 5000.0f, 3.5f, 5.0f);
    sa.deposit();
    sa.deposit(1500.0f);
    sa.withdraw(2000.0f);
    sa.calculateInterest();

    std::cout << "Savings balance: " << sa.bank_balance << std::endl;



    CheckingAccount ca("Nare", 3000.0f, 0, 3.0f);
    ca.deposit(500.0f);
    ca.addTransaction(1);
    ca.addTransaction(1);
    ca.withdraw(1000.0f);
    ca.isLimitReached();

    std::cout << "Checking balance: " << ca.bank_balance << std::endl;

    return 0;
}