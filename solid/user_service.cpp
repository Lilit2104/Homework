#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class User {
private:
    int id;
    std::string name;

public:
    User(int id, const std::string& name) : id(id), name(name) {}

    int getId() const 
    {
        return id;
    }

    const std::string& getName() const 
    {
        return name;
    }
};

class IUserRepository {
public:
    virtual void save(const User& user) const = 0;
    virtual ~IUserRepository() = default;
};

class FileUserRepository : public IUserRepository {
public:
    void save(const User& user) const override 
    {
        std::ofstream file("users.txt", std::ios::app);

        if (!file)
            throw std::runtime_error("Cannot open file");

        file << user.getId() << ","
             << user.getName() << '\n';
    }
};

class DatabaseUserRepository : public IUserRepository {
public:
    void save(const User& user) const override 
    {

        std::cout << "Saving user to database: "
                  << user.getId() << ", "
                  << user.getName() << '\n';
    }
};

class UserService {
private:
    const IUserRepository& repository;

public:
    explicit UserService(const IUserRepository& repository) : repository(repository) {}

    void registerUser(const User& user) const 
    {
        repository.save(user);
        std::cout << "User registered successfully!\n";
    }
};

int main() {
    User user(1, "Anna");

    FileUserRepository fileRepository;
    UserService fileService(fileRepository);
    fileService.registerUser(user);

    DatabaseUserRepository databaseRepository;
    UserService databaseService(databaseRepository);
    databaseService.registerUser(user);

    return 0;
}