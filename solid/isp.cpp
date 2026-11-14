#include <iostream>

class IPrinter {
public:
    virtual void print() = 0;
    virtual ~IPrinter() = default;
};

class IScanner {
public:
    virtual void scan() = 0;
    virtual ~IScanner() = default;
};

class IFax {
public:
    virtual void fax() = 0;
    virtual ~IFax() = default;
};

class Printer : public IPrinter {
public:
    void print() override 
    {
        std::cout << "Printing..." << std::endl;
    }
};

class Scanner : public IScanner {
public:
    void scan() override 
    {
        std::cout << "Scanning..." << std::endl;
    }
};

class Fax : public IFax {
public:
    void fax() override 
    {
        std::cout << "Faxing..." << std::endl;
    }
};


class MultifunctionDevice : public IPrinter, public IScanner, public IFax {
public:
    void print() override
    {
        std::cout << "Multifunction: Printing..." << std::endl;
    }

    void scan() override 
    {
        std::cout << "Multifunction: Scanning..." << std::endl;
    }

    void fax() override 
    {
        std::cout << "Multifunction: Sending fax..." << std::endl;
    }
};

int main() {
    Printer printer;
    Scanner scanner;
    Fax fax;
    MultifunctionDevice multifunction;

    printer.print();
    scanner.scan();
    fax.fax();

    multifunction.print();
    multifunction.scan();
    multifunction.fax();

    return 0;
}