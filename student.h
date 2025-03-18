#ifndef STUDENT_H
#define STUDENT_H
#include "mainDLL.h"
#include "course.h"

extern MainDLL* studentHead;
extern MainDLL* studentTail;

typedef struct student {
    int rollNo;
    char name[100];
    float CGPA;
    int NumberOfSubjects;
    CoursesDLL *headCourse;
} Student;

char* addStudent(int rollNo, char name[], float CGPA, int NumberOfSubjects);
char* deleteStudent(int rollNo);
char* modifyStudent(int rollNo, float CGPA);
Student* searchStudent(int rollNo);
void printStudent();

// void addStudent(struct mainDLL* headStudent, int rollNo, char name[], float CGPA, int NumberOfSubjects);
// void deleteStudent(struct mainDLL* headStudent, int rollNo);
// void modifyStudent(struct mainDLL* headStudent, int rollNo, float CGPA);
// Student* searchStudent(struct mainDLL* headStudent, int rollNo, char name[]);

#endif
