#include <iostream>
#include <utility>

class UniquePtr{
    private:
        int* pt;

    public:
        explicit UniquePtr(int* p = nullptr) : pt(p) {}

        ~UniquePtr() { delete pt; }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) noexcept : pt(other.pt)
        {
            other.pt = nullptr;
        }

        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            if (this != &other)
            {
                delete pt;
                pt = other.pt;
                other.pt = nullptr;
            }
            return *this;
        }

        int* get() const { return pt; }

        int* release()
        {
            int* tmp = pt;
            pt = nullptr;
            return tmp;
        }

        void reset(int* p = nullptr)
        {
            if (pt != p)
            {
                delete pt;
                pt = p;
            }
        }

        int& operator*() const { return *pt; }
        int* operator->() const { return pt; }
};

int main()
{
    UniquePtr a(new int(5));
    std::cout << *a << "\n";               

    UniquePtr b(std::move(a));
    std::cout << (a.get() == nullptr) << " " << *b << "\n";   

    int* raw = b.release();                
    std::cout << *raw << " " << (b.get() == nullptr) << "\n";
    delete raw;                          

    b.reset(new int(7));
    b.reset();                             
    std::cout << (b.get() == nullptr) << "\n";   
    
    return 0;
}