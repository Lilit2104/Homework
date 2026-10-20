#include <iostream>
#include <cstddef>
#include <utility>

class Stack {
    private:
        int*   m_data;
        size_t m_size;
        size_t m_capacity;

        void grow()
        {
            size_t newCap = (m_capacity == 0) ? 1 : m_capacity * 2;
            int* newData = new int[newCap];
            for (size_t i = 0; i < m_size; ++i)
            {
                newData[i] = m_data[i];
            }
            delete[] m_data;
            m_data = newData;
            m_capacity = newCap;
        }

    public:
        Stack() : m_data(nullptr), m_size(0), m_capacity(0) {}

        explicit Stack(size_t capacity)
            : m_data(capacity ? new int[capacity] : nullptr),
            m_size(0), m_capacity(capacity) {}

        Stack(const Stack& other)
            : m_data(other.m_capacity ? new int[other.m_capacity] : nullptr),
            m_size(other.m_size), m_capacity(other.m_capacity)
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Stack(Stack&& other) noexcept
            : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
        {
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        Stack& operator=(const Stack& other)
        {
            if (this != &other)
            {
                int* newData = other.m_capacity ? new int[other.m_capacity] : nullptr;
                for (size_t i = 0; i < other.m_size; ++i)
                {
                    newData[i] = other.m_data[i];
                }
                delete[] m_data;
                m_data = newData;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
            }
            return *this;
        }

        Stack& operator=(Stack&& other) noexcept
        {
            if (this != &other)
            {
                delete[] m_data;
                m_data = other.m_data;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;
            }
            return *this;
        }

        ~Stack() { delete[] m_data; }

        void push(int value)
        {
            if (m_size == m_capacity)
                grow();
            m_data[m_size++] = value;
        }

    
        bool pop()
        {
            if (m_size == 0)
                return false;
            --m_size;
            return true;
        }

        bool top(int& value) const
        {
            if (m_size == 0)
                return false;
            value = m_data[m_size - 1];
            return true;
        }

        bool   empty() const    { return m_size == 0; }
        size_t size() const     { return m_size; }
        size_t capacity() const { return m_capacity; }

        void clear() { m_size = 0; }
};


void Search_And_Change(Stack& st, int x, int y)
{
    int v;
    if (!st.top(v))
    {
        return;
    }
    st.pop();
    Search_And_Change(st, x, y);
    st.push(v == x ? y : v);
}

void printStack(Stack s, const char* name)
{
    std::cout << name << " (size=" << s.size() << ", cap=" << s.capacity() << "): ";
    Stack tmp;
    int v;
    while (s.top(v)) { tmp.push(v); s.pop(); }
    while (tmp.top(v)) { std::cout << v << ' '; tmp.pop(); }
    std::cout << "\n";
}

int main()
{
    
    Stack a;
    int v = 0;
    std::cout << "empty: " << a.empty() << ", cap: " << a.capacity() << "\n";
    std::cout << "top on empty: " << std::boolalpha << a.top(v) << "\n";
    std::cout << "pop on empty: " << a.pop() << "\n";

    
    Stack b(2);
    std::cout << "\ncap before: " << b.capacity() << "\n";
    for (int i = 1; i <= 5; ++i)
    {
        b.push(i);
        std::cout << "push " << i << " -> size=" << b.size()
                  << ", cap=" << b.capacity() << "\n";
    }
    if (b.top(v)) std::cout << "top: " << v << "\n";
    b.pop();
    if (b.top(v)) std::cout << "after pop, top: " << v << ", size: " << b.size() << "\n";

   
    Stack c(b);
    c.push(100);
    printStack(b, "b");
    printStack(c, "c (copy, + 100)");

   
    Stack d(std::move(c));
    printStack(d, "d (moved)");
    std::cout << "c after move: size=" << c.size() << ", cap=" << c.capacity() << "\n";
    c.push(7);
    if (c.top(v)) std::cout << "c after push: top=" << v << "\n";

   
    b.clear();
    std::cout << "\nb after clear: empty=" << b.empty() << ", cap=" << b.capacity() << "\n";

    
    Stack e;
    for (int x : {1, 2, 3, 2, 5, 2}) e.push(x);
    printStack(e, "\ne before");
    Search_And_Change(e, 2, 9);
    printStack(e, "e after (2 -> 9)");

    return 0;
}