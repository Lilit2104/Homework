#include <iostream>


class  Person{
    public:
        char* name;
        int age;
    public:
        Person(char* name, int age) 
            : name(name), age(age) {}
    
        void printInfo() 
        {
            std::cout << "Name: " << name << ", Age: " << age << std::endl;
        }

};

class Student : public Person{
    public:
        char* uni;
        int studentid;

    public:
        Student(int age,char* name, char* uni, int studentid) 
            : Person(name, age), uni(uni), studentid(studentid){}

        void printstudentInfo() 
        {
            std::cout << "Name:   " << name << ", Age:   " << age << ", Uni:  " << uni << ", Student id:  " << studentid << std::endl;
        }

        void  changeUniversity(char* newuni)
        {
            uni = newuni;
        }

        void changeUniversity(char* newUni, int newStudentId) {
        uni = newUni;
        studentid = newStudentId;
    }

};


int main()
{
   
    char personName[] = "Lily";
    Person p(personName, 22);
    p.printInfo();

    
    char studentName[] = "Lilit";
    char uniName[] = "YSU";
    Student s(22, studentName, uniName, 12345);
    s.printstudentInfo();

    char newUni1[] = "AUA";
    s.changeUniversity(newUni1);
    s.printstudentInfo();

   
    char newUni2[] = "ASUE";
    s.changeUniversity(newUni2, 67890);
    s.printstudentInfo();

    return 0;
}