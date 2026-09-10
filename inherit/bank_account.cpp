#include <iostream>
#include <string>

class Bankaccount{
    public:
        std::string name;
        float balance;
        int id_num;
    public:

        Bankaccount(std::string name, float balance, int id_num) 
            : name(name), balance(balance), id_num(id_num) {}

        void deposit(float money)
        {
            if(money > 0)
            {
                balance += money;
            }
            else
            {
                std::cout << " Unvalible " << std::endl;
            }
        }

        void withdraw(float mon)
        {
            if(balance > mon)
            {
                balance -= mon;
            }
            else
            {
                std::cout << "There is not any money left" << std::endl;
            }
        }

        float getBalance()
        {
            return balance;
        }

};

class SavingsAccount : public Bankaccount {
    public:
       float annual_rate;
    
    public:

        SavingsAccount(std::string name, float balance, int id_num, float annual_rate) 
            : Bankaccount(name, balance, id_num), annual_rate(annual_rate) {}


        float calculateInterest()
        {
            float interestAmount = 0;
            if(balance < 100000)
            {
                interestAmount = balance * (3.0f / 100.0f);
            }
            else if(balance >= 100000 && balance <= 500000)
            {
                interestAmount = balance * (5.0f / 100.0f);
            }
            else if(balance > 500000)
            {
                interestAmount = balance * (7.0f / 100.0f);
            }
            return interestAmount;
        }

        void  applyInterest()
        {
            balance = balance + calculateInterest();
        }

        void deposit(float amount)
        {
            if (amount > 500000)
            {
                float bonus = amount * 0.01f;
                balance = balance + amount + bonus;
            }
            else
            {
                balance = balance + amount;
            }
        }

        void  withdraw(float amount)
        {
            float new_balance = balance - amount;
            if(new_balance < 10000)
            {
                std::cout << "There is no avaliable balance: " << new_balance << std::endl;
            }
            else
            {
                balance = new_balance;
            }
        }
}; 
 
class PremiumAccount : public Bankaccount{
    public: 
        int bonuspoint;
    public:
        PremiumAccount(std::string name, float balance, int id_num, int bonuspoint) 
            : Bankaccount(name, balance,id_num), bonuspoint(bonuspoint) {}
    
    void addBonus(int points)
    {
        bonuspoint += points;
    }

    void addBonus( float purchaseamount)
    {
        int points;
        if(purchaseamount < 100000)
        {
            points = purchaseamount / 1000 * 1;
        }
        else
        {
            points = purchaseamount / 1000 * 2;
        }
        bonuspoint += points;
    }

    void convertBonus()
    {
        float rate;
        float convertedamount;
        if(bonuspoint  < 500)
        {
            rate = 500.0f / 100.0f;
        }
        else
        {
            rate = 700.0f / 100.0f;
        }
        convertedamount = bonuspoint * rate;
        balance = balance + convertedamount;
        bonuspoint = 0;
    }

    void getAccountLevel() 
    {
        if(bonuspoint < 500)
        {
            std::cout << "Basic" << std::endl;
        }
        else if(bonuspoint < 1000)
        {
            std::cout << "Silver" << std::endl;
        }
        else
        {
            std::cout << "Gold" << std::endl;
        }
    }

    void withdraw(float amount)
    {
        float new_balance = balance - amount;
        float hold;
        if(bonuspoint >= 1000)
        {
            hold = 5000;
        }
        else
        {
            hold = 10000;
        }
        if(new_balance < hold)
        {
            std::cout << " Unavaliable" << std::endl;
        }
        else
        {
            balance = new_balance;
            std::cout << " Done!" << std::endl;
        }
    }

};



int main()
{
    SavingsAccount sav("Ani", 50000, 1, 0);

    std::cout << "\nDeposit: interest rate tiers" << std::endl;
    std::cout << "Initial balance: " << sav.getBalance() << std::endl;
    std::cout << "Interest (<100k): " << sav.calculateInterest() << std::endl;

    sav.deposit(80000); 
    std::cout << "Balance after deposit: " << sav.getBalance() << std::endl;
    std::cout << "Interest (100k-500k): " << sav.calculateInterest() << std::endl;

    sav.deposit(400000); 
    std::cout << "Balance after deposit: " << sav.getBalance() << std::endl;
    std::cout << "Interest (>500k): " << sav.calculateInterest() << std::endl;

    std::cout << "\n applyInterest" << std::endl;
    sav.applyInterest();
    std::cout << "Balance after interest applied: " << sav.getBalance() << std::endl;

    std::cout << "\nWithdraw below min balance" << std::endl;
    SavingsAccount sav2("Aram", 15000, 2, 0);
    sav2.withdraw(10000); 
    std::cout << "Balance after rejected withdraw: " << sav2.getBalance() << std::endl;

    std::cout << "\n Deposit bonus check" << std::endl;
    SavingsAccount sav3("Maria", 10000, 3, 0);
    sav3.deposit(600000); 
    std::cout << "Balance after big deposit + bonus: " << sav3.getBalance() << std::endl;


    std::cout << "\n\nPremiumAccount tests " << std::endl;

    PremiumAccount prem("Davit", 100000, 4, 0);

    std::cout << "\n Bonus calc: small vs large purchase " << std::endl;
    prem.addBonus(50000.0f);  
    std::cout << "Bonus points after small purchase: " << prem.bonuspoint << std::endl;

    prem.addBonus(150000.0f); 
    std::cout << "Bonus points after large purchase: " << prem.bonuspoint << std::endl;

    std::cout << "\n getAccountLevel " << std::endl;
    prem.getAccountLevel();

    std::cout << "\n convertBonus " << std::endl;
    std::cout << "Balance before conversion: " << prem.getBalance() << std::endl;
    prem.convertBonus();
    std::cout << "Balance after conversion: " << prem.getBalance() << std::endl;
    std::cout << "Bonus points after conversion: " << prem.bonuspoint << std::endl;

    std::cout << "\n Premium withdraw, bonus >= 1000 " << std::endl;
    PremiumAccount prem2("Anush", 20000, 5, 1500);
    prem2.withdraw(15000); 
    std::cout << "Balance after withdraw: " << prem2.getBalance() << std::endl;

    std::cout << "\n Premium withdraw, bonus < 1000" << std::endl;
    PremiumAccount prem3("Gor", 20000, 6, 200);
    prem3.withdraw(15000); 
    std::cout << "Balance after rejected withdraw: " << prem3.getBalance() << std::endl;

    return 0;
}