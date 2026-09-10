#include <iostream>
#include <string>

class Student{
    public:
        std::string studentName;
        int studentID;
        int gradeLevel;
    public:
        Student(std::string studentName, int studentID, int gradeLevel) 
            : studentName(studentName), studentID(studentID), gradeLevel(gradeLevel) {}

        Student() : studentName("Unknown"), studentID(0), gradeLevel(0) {}

        void PrintInfo()
        {
            std::cout << "Student name: " << studentName << " , "  << gradeLevel << "th class " << " , " << "ID: " << studentID << std::endl;
        }
};

class Teacher{
    public:
        std::string teacherName;
        int teacherID;
        std::string subject;
    public:
        Teacher(std::string teacherName, int teacherID, std::string subject) 
            : teacherName(teacherName), teacherID(teacherID), subject(subject)  {}

        Teacher() : teacherName("Unknown"), teacherID(0), subject(subject) {}

        void PrintInfo()
        {
            std::cout << "Teacher name: " << teacherName << " , "  << "Subject: " <<  subject << " , " << "ID: " << teacherID << std::endl;
        }

};

class ClassRoom {
    public:       
        std::string className;
        Teacher classTeacher;        
        Student students[5];        
        int studentCount;
        int maxStudents;
    public:
        ClassRoom() : className("Unknown"), studentCount(0), maxStudents(0) {}

        ClassRoom(std::string className, Teacher teacher, int maxStudents) 
            : className(className), classTeacher(teacher), maxStudents(maxStudents), studentCount(0) {}

        void addSt(Student st)
        {
            if(studentCount < maxStudents)
            {
                students[studentCount] = st;
                studentCount ++;
            }
            else
            {
                std::cout << " Can not add the student class is full" << std::endl;
            }
        }

        void PrintInfo()
        {
            std::cout << "Classroom: " << className << std::endl;
            std::cout << " Teacher Info " << std::endl;
            classTeacher.PrintInfo();

            std::cout << " Students (" << studentCount << "/" << maxStudents << ") " << std::endl;
            for (int i = 0; i < studentCount; i++) {
                students[i].PrintInfo();
            }
        }
};
         
int main()
{
   
    Teacher t1("Mr. Sergey", 501, "Mathematics");

    
    
    Student s1("Lilit Mikayelyan", 201, 10);
    Student s2("Klaus Mikaelson", 202, 10);
    Student s3("Joe Joe", 203, 10);

   
    ClassRoom room("10th Grade Mathematics", t1, 2);

    
    room.addSt(s1);
    room.addSt(s2);
    room.addSt(s3); 
  
    room.PrintInfo();

    return 0;
}
    
