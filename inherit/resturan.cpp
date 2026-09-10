#include <iostream>
#include <string>
#include <vector>



class Dish {
    protected:
        std::string name;
        double price;
public:
        Dish(std::string dishName, double dishPrice) 
            : name(dishName), price(dishPrice) {}

        Dish(const Dish& other)
            : name(other.name) , price(other.price) {}

            
        Dish& operator=(const Dish& other)
        {
            if(this == &other)
            {
                return *this;
            }

            name = other.name;
            price = other.price;

            return *this;

        }
        Dish(Dish&& other) noexcept
            : name(std::move(other.name)), price(other.price)
        {
            other.price = 0.0;
        }

        Dish& operator=(Dish&& other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }

            name = std::move(other.name);
            price = other.price;

            other.price = 0.0;

            return *this;
        }

        virtual void display() const
        {
            std::cout << "Product name: " << name << " , " << "Price: " << price << std::endl; 
        }

        virtual Dish* clone() const 
        { 
            return new Dish(*this); 
        }

        double getPrice() const
        {
            return price;
        }

        std::string getName() const
        {
            return name;
        }

        virtual ~Dish() = default;
};


class Appetizer : public Dish {
    private:
        bool isSpicy;
    public:
        Appetizer(std::string dishName, double dishPrice, bool spicy)
            : Dish(dishName, dishPrice), isSpicy(spicy) {}

        virtual void display() const
        {
            Dish::display();  
            std::cout << "Spicy: " << (isSpicy ? "Yes" : "No") << std::endl;
        }

        Dish* clone() const override 
        { 
            return new Appetizer(*this); 
        }
};

class Entree : public Dish {
    private:
        int calories;
    public:
        Entree(std::string dishName, double dishPrice, int cal)
            : Dish(dishName,dishPrice), calories(cal) {}

        virtual void display() const
        {
            Dish::display();
            std::cout << "Calories equal to : " << calories << std::endl;
        }

        Dish* clone() const override 
        { 
            return new Entree(*this); 
        }
};
class Dessert : public Dish {
    private:
        bool containsNuts;
    public:
        Dessert(std::string dishName, double dishPrice, bool nuts)
            :   Dish(dishName, dishPrice), containsNuts(nuts) {}

        virtual void display() const
        {
            Dish::display();
            std::cout << "Contains nuts: " << (containsNuts ? "Yes" : "No") << std::endl;
        }

        Dish* clone() const override 
        { 
            return new Dessert(*this); 
        }
};


class Menu {
    private:
        std::vector<Dish*> dishes;
    public:
        Menu() = default;

        Menu(const Menu& other)
        {
            for (Dish* d : other.dishes) 
            {
                dishes.push_back(d->clone());
            }   
        }

        Menu& operator=(const Menu& other)
        {
            if(this == &other)
            {
                return *this;
            }

            for (Dish* d : dishes)
            {
                delete d;
            }
            dishes.clear();

            for (Dish* d : other.dishes)
            {
                dishes.push_back(d->clone());
            }

            return *this;   

        }
    
        Menu(Menu&& other) noexcept
            : dishes(std::move(other.dishes))  {}


        Menu& operator=(Menu&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            for (Dish* d : dishes)
            {
                delete d;
            }
            dishes.clear();

            dishes = std::move(other.dishes);

            return *this;

        }

        ~Menu()
        {
            for(Dish* d : dishes)
            {
                delete d;
            }
        }

        void addDish(Dish* dish)
        {
            dishes.push_back(dish);
        }

        void displayMenu() const
        {
            for(Dish* d : dishes)
            {
                d -> display();
            }
        }

        Dish* getDishByName(const std::string& dishName) const
        {
            for(Dish* d : dishes)
            {
                if(d->getName() == dishName)
                {
                    return d;
                }
            }

            return nullptr;
        }
};


class Customer; // forward declaration

class Order {
    private:
        Customer* customer;
        std::vector<Dish*> orderedDishes;
        double totalPrice;
    public:
        Order(Customer* customer)
            : customer(customer), totalPrice(0.0) {}

        Order(const Order& other)
            : customer(other.customer), orderedDishes(other.orderedDishes), totalPrice(other.totalPrice) {}

        Order& operator=(const Order& other)
        {
            if(this == &other)
            {
                return *this;
            }

            customer = other.customer;
            totalPrice = other.totalPrice;
            orderedDishes = other.orderedDishes;

            return *this;
        }
          

        Order(Order&& other) noexcept
            : customer(other.customer), orderedDishes(std::move(other.orderedDishes)), totalPrice(other.totalPrice)
        {
            other.customer = nullptr;
            other.totalPrice = 0.0;
        }

        Order& operator=(Order&& other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }

            customer = other.customer;
             orderedDishes = std::move(other.orderedDishes);
            totalPrice = other.totalPrice;

            other.customer = nullptr;
            other.totalPrice = 0.0;

            return *this;
        }

        ~Order() = default;

        void addDish(Dish* dish)
        {
            orderedDishes.push_back(dish);
        }

        void calculateTotal()
        {       
            totalPrice = 0.0;
            for (Dish* d : orderedDishes)
            {
                totalPrice += d->getPrice();
            }
        }
        
        void displayOrder() const
        {
             for(Dish* d : orderedDishes)
            {
                d -> display();
            }
            std::cout << "Total price: " << totalPrice << std::endl;
        }
};



class Customer {
    private:
        std::string name;
        std::string contactInfo;
        std::vector<Order> orderHistory;
    public:

        Customer(std::string customerName, std::string contact)
            : name(customerName), contactInfo(contact) {}

        Customer(const Customer& other)
            : name(other.name), contactInfo(other.contactInfo), orderHistory(other.orderHistory) {}

        Customer& operator=(const Customer& other)
        {
            if(this == &other)
            {
                return *this;
            }

            name = other.name;
            contactInfo = other.contactInfo;
            orderHistory = other.orderHistory;

            return *this;

        }

        Customer(Customer&& other) noexcept
            : name(std::move(other.name)), contactInfo(std::move(other.contactInfo)), orderHistory(std::move(other.orderHistory)) {}
 

        Customer& operator=(Customer&& other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }

            name = std::move(other.name);
            contactInfo = std::move(other.contactInfo);
            orderHistory = std::move(other.orderHistory);

            return *this;
        }

        ~Customer() = default;

        void placeOrder(Order order)
        {
            orderHistory.push_back(order);
        }
        
        void viewOrderHistory() const
        {
            for (const Order& o : orderHistory)
            {
                o.displayOrder();
            }
        }

        std::string getName() const
        {
            return name;
        }

};


class Restaurant {
    private:
        Menu menu;
        std::vector<Customer*> customers;
        std::vector<Order> orders;
    public:
        Restaurant() {}

        Restaurant(const Restaurant& other)
            : menu(other.menu), orders(other.orders)
        {
            for (Customer* c : other.customers)
            {
                customers.push_back(new Customer(*c));
            }
        }

        Restaurant& operator=(const Restaurant& other)
        {
            if (this == &other)
            {
                return *this;
            }

            menu = other.menu;
            orders = other.orders;

            for (Customer* c : customers)
            {
                delete c;
            }
            customers.clear();

            for (Customer* c : other.customers)
            {
                customers.push_back(new Customer(*c));
            }

            return *this;
        }

        Restaurant(Restaurant&& other) noexcept
            : menu(std::move(other.menu)), customers(std::move(other.customers)), orders(std::move(other.orders)) {}

        Restaurant& operator=(Restaurant&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            for (Customer* c : customers)
            {
                delete c;
            }
            customers.clear();

            menu = std::move(other.menu);
            customers = std::move(other.customers);
            orders = std::move(other.orders);

            return *this;
        }

        ~Restaurant()
        {
            for (Customer* c : customers)
            {
                delete c;
            }
        }

        void showMenu() const
        {
            menu.displayMenu();
        }

        Customer* getCustomerByName(const std::string& name)
        {
            for (Customer* c : customers)
            {
                if (c->getName() == name)
                {
                    return c;
                }
            }
            return nullptr;
        }

        void placeNewOrder(const std::string& customerName)
        {
            Customer* c = getCustomerByName(customerName);
            if (c == nullptr)
            {
                c = new Customer(customerName, "N/A");
                customers.push_back(c);
            }

            Order order(c);

            Dish* d1 = menu.getDishByName("Spring Rolls");
            if (d1 != nullptr)
            {
                order.addDish(d1);
            }

            Dish* d2 = menu.getDishByName("Steak");
            if (d2 != nullptr)
            {
                order.addDish(d2);
            }

            order.calculateTotal();
            orders.push_back(order);
            c->placeOrder(order);
        }

        void viewCustomerOrderHistory(const std::string& customerName) const
        {
            for (Customer* c : customers)
            {
                if (c->getName() == customerName)
                {
                    c->viewOrderHistory();
                    return;
                }
            }
            std::cout << "Customer not found." << std::endl;
        }

        void addDishToMenu(Dish* dish)
        {
            menu.addDish(dish);
        }
};

int main()
{
    Restaurant restaurant;

    restaurant.addDishToMenu(new Appetizer("Spring Rolls", 5.99, true));
    restaurant.addDishToMenu(new Entree("Steak", 25.99, 800));
    restaurant.addDishToMenu(new Dessert("Ice Cream", 4.99, false));

    std::cout << " Menu " << std::endl;
    restaurant.showMenu();

    std::cout << "\nPlacing Order " << std::endl;
    restaurant.placeNewOrder("Alice Johnson");

    std::cout << "\n Order History" << std::endl;
    restaurant.viewCustomerOrderHistory("Alice Johnson");

    return 0;
}