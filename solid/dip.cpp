#include <iostream>


class INotificationSender {
public:
    virtual void send(const std::string& message) const = 0;
    virtual ~INotificationSender() = default;
};

class EmailSender : public INotificationSender {
public:
    void send(const std::string& message) const override
    {
        std::cout << "Email: " << message << '\n';
    }
};

class SMSSender : public INotificationSender {
public:
    void send(const std::string& message) const override 
    {
        std::cout << "SMS: " << message << '\n';
    }
};

class OrderService {
private:
    const INotificationSender& sender;

public:
    explicit OrderService(const INotificationSender& sender)
        : sender(sender) {}

    void completeOrder() const
    {
        sender.send("Your order is completed!");
    }
};

int main() {
    EmailSender emailSender;
    SMSSender smsSender;

    OrderService emailOrderService(emailSender);
    OrderService smsOrderService(smsSender);

    emailOrderService.completeOrder();
    smsOrderService.completeOrder();

    return 0;
}