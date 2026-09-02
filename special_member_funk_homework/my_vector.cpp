#include <iostream>

class MY_vector{
    private:
        int* data;
        size_t size;
        size_t capacity;

    public:
       MY_vector() : data(nullptr), size(0), capacity(0)
       {
       }

       MY_vector(size_t count)
       {
            data = new int[count];
            size = count;
            capacity = count;

            for(size_t i = 0; i < count; ++i)
            {
                 data[i] = 0;
            }
       }

       MY_vector(const MY_vector& other)
            : data(new int[other.capacity]), size(other.size), capacity(other.capacity)
       {
         for(size_t i = 0; i < other.size ; ++i)
         {
            data[i] = other.data[i];
         }
       }

       MY_vector(MY_vector&& other)
            : data(other.data),size(other.size), capacity(other.capacity)
        {
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }

        ~MY_vector()
        {
            delete[] data;
        }


        void push_back(int value)
        {

            size_t new_capacity = 0;

            if(size < capacity)
            {
                data[size] = value;
                size ++;
            }
            else
            {
                if(capacity == 0)
                {
                   new_capacity = capacity + 1;
                }
                else
                {
                    new_capacity = capacity * 2;
                }
                int* new_data = new int[new_capacity];

                for(size_t i = 0; i < size; ++i)
                {
                    new_data[i] = data[i];
                }
                delete [] data;
                data = new_data;
                capacity = new_capacity;
                data[size] = value;
                size++;
            }
        }

        void pop_back()
        {
            if(size == 0)
            {
                return;
            }
            else
            {
               size--;
            }
        }

        int get(size_t index) const
        {
            if(index < size)
            {
                return data[index];
            }
            else
            {
                return 0;
            }
        }

        void set(size_t index, int value)
        {
            if(index < size)
            {
                data[index] = value;
            }
            else
            {
                return;
            }
        }

        size_t  get_size() const
        {
            return size;
        }


        size_t get_capacity() const
        {
            return capacity;
        }


        bool empty() const
        {
            if(size == 0)
            {
                return true;
            }
            else
            {
               return false;
            }
        }

        void clear()
        {
          size = 0;
        }


        void insert(size_t index, int value)
        {
            if(index > size)
            {
                return;
            }

            if(size == capacity)
            {
                size_t new_capacity = 0;
                if(capacity == 0)
                {
                   new_capacity = capacity + 1;
                }
                else
                {
                    new_capacity = capacity * 2;
                }
                int* new_data = new int[new_capacity];

                for(size_t i = 0; i < size; ++i)
                {
                    new_data[i] = data[i];
                }

                delete [] data;
                data = new_data;
                capacity = new_capacity;
            }

            for(size_t i = size; i > index; --i)
            {
                data[i] = data[i - 1];
            }
            data[index] = value;
            size++;
        }

        void erase(size_t index)
        {
            if(index >= size)
            {
                return;
            }

            for(size_t i = index; i < size - 1; ++i)
            {
                data[i] = data[i + 1];
            }
            size--;
        }


        
        MY_vector& operator=(const MY_vector& other)
        {

            if (this == &other)
            {
                return *this;
            }
            else
            {
                delete [] data;
            }

            data = new int[other.capacity];
            for (size_t i = 0; i < other.size; ++i)
            {
                data[i] = other.data[i];
            }
            size = other.size;
            capacity = other.capacity;

            return *this;
        }

        MY_vector& operator=(MY_vector&& other)
        {
            if (this == &other)
            {
                return *this;
            }
            else
            {
                delete [] data;
            }

            data = other.data;
            size = other.size;
            capacity = other.capacity;

            other.data = nullptr;
            other.size =  0;
            other.capacity = 0;

            return *this;
        }

        MY_vector& operator+=(int value)
        {
            push_back(value);
            return *this;
        }

        MY_vector& operator+=(const MY_vector& other)
        {
            for(size_t i = 0; i < other.size; ++i)
            {
                push_back(other.data[i]);
            }
            return *this;
        }

        bool operator==(const MY_vector& other) const
        {
            if(size != other.size)
            {
                return false;
            }
            else
            {
                for(size_t i = 0; i < other.size; ++i)
                {
                    if(data[i] != other.data[i])
                    {
                        return false;
                    }
                }
            }
            return true;
        }
};


int main()
{
    MY_vector v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    for (size_t i = 0; i < v.get_size(); ++i)
     std::cout << v.get(i) << " ";
    std::cout << "\n";
    // 10 20 30

    v.insert(1, 50);

    for (size_t i = 0; i < v.get_size(); ++i)
     std::cout << v.get(i) << " ";
    std::cout << "\n";
    // 10 50 20 30

    v.erase(2);

    for (size_t i = 0; i < v.get_size(); ++i)
     std::cout << v.get(i) << " ";
    std::cout << "\n";
    // 10 50 30

    v.set(0, 100);

    for (size_t i = 0; i < v.get_size(); ++i)
     std::cout << v.get(i) << " ";
    std::cout << "\n";
    // 100 50 30

    v += 200;

    for (size_t i = 0; i < v.get_size(); ++i) 
     std::cout << v.get(i) << " ";
    std::cout << "\n";
    // 100 50 30 200

    MY_vector v2(v);

    if (v == v2)
    {
        std::cout << "vectors are equal\n";
    }

    MY_vector v3;
    v3 = v2;

    MY_vector v4(std::move(v3));

    MY_vector v5;
    v5 = std::move(v4);

    for (size_t i = 0; i < v5.get_size(); ++i)
     std::cout << v5.get(i) << " ";
    std::cout << "\n";
    // 100 50 30 200

    return 0;
}