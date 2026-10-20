#include <iostream>
#include <cstddef>
#include <utility>

struct ControlBlock
{
    int*   ptr;
    size_t shared_count;
    size_t weak_count;
};

class WeakPtr;

class SharedPtr{
    private:
        ControlBlock* cb;

        friend class WeakPtr;

        
        explicit SharedPtr(ControlBlock* c) : cb(c)
        {
            if (cb) ++cb->shared_count;
        }

        void release_ownership()
        {
            if (!cb) return;

            if (--cb->shared_count == 0)
            {
                delete cb->ptr;       
                cb->ptr = nullptr;

                if (cb->weak_count == 0)
                    delete cb;         
            }
            cb = nullptr;
        }

    public:
        SharedPtr() : cb(nullptr) {}

        explicit SharedPtr(int* p) : cb(nullptr)
        {
            if (p)
            {
                try { cb = new ControlBlock{p, 1, 0}; }
                catch (...) { delete p; throw; }
            }
        }

        ~SharedPtr() { release_ownership(); }

        SharedPtr(const SharedPtr& other) : cb(other.cb)
        {
            if (cb) ++cb->shared_count;
        }

        SharedPtr(SharedPtr&& other) noexcept : cb(other.cb)
        {
            other.cb = nullptr;
        }

        SharedPtr& operator=(const SharedPtr& other)
        {
            if (cb != other.cb)
            {
                SharedPtr tmp(other);
                swap(tmp);
            }
            return *this;
        }

        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            if (this != &other)
            {
                SharedPtr tmp(std::move(other));
                swap(tmp);
            }
            return *this;
        }

        void swap(SharedPtr& other) noexcept { std::swap(cb, other.cb); }

        int* get() const { return cb ? cb->ptr : nullptr; }

        size_t use_count() const { return cb ? cb->shared_count : 0; }

        void reset() { release_ownership(); }

        void reset(int* p)
        {
            SharedPtr tmp(p);
            swap(tmp);
        }

        int& operator*() const { return *cb->ptr; }
        int* operator->() const { return cb->ptr; }

    
        explicit operator bool() const { return get() != nullptr; }
};

class WeakPtr{
    private:
        ControlBlock* cb;

        void release_weak()
        {
            if (!cb) return;

            --cb->weak_count;
            if (cb->shared_count == 0 && cb->weak_count == 0)
                delete cb;
            cb = nullptr;
        }

    public:
        WeakPtr() : cb(nullptr) {}

        WeakPtr(const SharedPtr& sp) : cb(sp.cb)
        {
            if (cb) ++cb->weak_count;     
        }

        ~WeakPtr() { release_weak(); }

        WeakPtr(const WeakPtr& other) : cb(other.cb)
        {
            if (cb) ++cb->weak_count;
        }

        WeakPtr(WeakPtr&& other) noexcept : cb(other.cb)
        {
            other.cb = nullptr;
        }

        WeakPtr& operator=(const WeakPtr& other)
        {
            if (cb != other.cb)
            {
                WeakPtr tmp(other);
                std::swap(cb, tmp.cb);
            }
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other) noexcept
        {
            if (this != &other)
            {
                WeakPtr tmp(std::move(other));
                std::swap(cb, tmp.cb);
            }
            return *this;
        }

        size_t use_count() const { return cb ? cb->shared_count : 0; }

        bool expired() const { return use_count() == 0; }

        SharedPtr lock() const
        {
            if (expired())
                return SharedPtr();
            return SharedPtr(cb);
        }
};

int main()
{
    WeakPtr w;
    {
        SharedPtr p1(new int(10));
        SharedPtr p2 = p1;
        SharedPtr p3 = p1;
        std::cout << "use_count: " << p1.use_count() << "\n";                

        w = p1;
        std::cout << "after weak, use_count: " << p1.use_count() << "\n";     
        std::cout << "expired: " << std::boolalpha << w.expired() << "\n";    

        if (SharedPtr s = w.lock())
        {
            std::cout << "lock OK, value=" << *s
                      << ", use_count=" << s.use_count() << "\n";             
        }
    }
    std::cout << "after scope, expired: " << w.expired() << "\n";             
    SharedPtr dead = w.lock();
    std::cout << "lock on expired -> get(): " << dead.get() << "\n";         

    SharedPtr a(new int(1));
    SharedPtr b(std::move(a));
    std::cout << "a.get()=" << a.get() << ", b=" << *b << "\n";
    b.reset(new int(2));
    std::cout << "b after reset: " << *b << ", use_count=" << b.use_count() << "\n";
    return 0;
}