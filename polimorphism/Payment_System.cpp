#include <iostream>

class PaymentMethod{
    public:
        virtual void processPayment(double amount) 
        {
            std::cout << "Վճարումը մշակվում է:" << std::endl; 
        }
        
        virtual ~PaymentMethod() = default;
};

class CreditCard : public PaymentMethod{
    public:
        virtual void processPayment(double amount) override
        {
            std::cout << "Վճարվեց  " << amount << " դրամ բանկային քարտով:" << std::endl;
        }
};

class PayPal : public PaymentMethod{
    public:
        virtual void processPayment(double amount) override
        {
            std::cout << "Վճարվեց  "<< amount << "  դրամ PayPal հաշվի միջոցով:" << std::endl;
        }
};


int main()
{
    PaymentMethod* payment1 = new CreditCard();
    PaymentMethod* payment2 = new PayPal();

    payment1->processPayment(100.0);
    payment2->processPayment(100.0);

    delete payment1;
    delete payment2;

    return 0;
}