#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "course.h"
#include "mainDLL.h"

MainDLL* studentHead = NULL;
MainDLL* studentTail = NULL;

void printStudent() {
    MainDLL* temp = studentHead;
    while (temp != NULL) {
        printf("Roll No: %d\n", temp->studentData->rollNo);
        printf("Name: %s\n", temp->studentData->name);
        printf("CGPA: %.3f\n", temp->studentData->CGPA);
        printf("NumberOfSubjects: %d\n", temp->studentData->NumberOfSubjects);
        printf("Course Information:- \n");

        CoursesDLL* courseTemp = temp->studentData->headCourse;
        while (courseTemp != NULL) {
            printf("Course Code : %d\n", courseTemp->courseData->courseCode);
            printf("Marks : %f\n", courseTemp->courseData->marks);

            courseTemp = courseTemp->next;
        }
        printf("\n");
        temp = temp->next;
    }
    printf("\n*********************************************\n\n");
}

char* addStudent(int rollNo, char name[], float CGPA, int NumberOfSubjects) {
    char* rst = (char*)malloc(sizeof(char) * 2048);
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";
    }

    if (searchStudent(rollNo)) {
        snprintf(rst, 2048, "Student with roll number %d is already registered in the database...Failed!!\n", rollNo);
        return rst;
    }

    MainDLL* newStudent = (MainDLL*)malloc(sizeof(MainDLL));
    if (newStudent == NULL) {
        free(rst);
        return "Memory allocation failed for new student...Failed!!\n";
    }

    newStudent->studentData = (Student*)malloc(sizeof(Student));
    if (newStudent->studentData == NULL) {
        free(rst);
        free(newStudent);
        return "Memory allocation failed for student data...Failed!!\n";
    }

    newStudent->studentData->rollNo = rollNo;
    strcpy(newStudent->studentData->name, name);
    newStudent->studentData->CGPA = CGPA;
    newStudent->studentData->NumberOfSubjects = NumberOfSubjects;
    newStudent->studentData->headCourse = NULL;

    newStudent->prev = NULL;
    newStudent->next = NULL;

    if (studentHead == NULL) {
        studentHead = newStudent;
        studentTail = newStudent;
    } else {
        studentTail->next = newStudent;
        newStudent->prev = studentTail;
        studentTail = studentTail->next;
    }

    snprintf(rst, 2048, "Student with roll number %d successfully added.\n", rollNo);
    return rst;
}

char* deleteStudent(int rollNo) {
    char* rst = (char*)malloc(sizeof(char) * 2048);
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";
    }

    if (studentHead == NULL) {
        snprintf(rst, 2048, "No students in the database...Failed!!\n");
        return rst;
    }

    if (studentHead->studentData->rollNo == rollNo) {
        MainDLL* toDelete = studentHead;
        studentHead = studentHead->next;
        if (studentHead != NULL) {
            studentHead->prev = NULL;
        } else {
            studentTail = NULL;  // If head is null, tail should also be null
        }
        free(toDelete->studentData);
        free(toDelete);
        snprintf(rst, 2048, "Student with roll number %d successfully deleted.\n", rollNo);
        return rst;
    }

    MainDLL* temp = studentHead;
    while (temp->next != NULL && temp->next->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp->next == NULL) {
        snprintf(rst, 2048, "Student with Roll Number %d not found while trying to delete it...Failed!!\n", rollNo);
        return rst;
    }

    MainDLL* toDelete = temp->next;

    if (toDelete->next == NULL) {
        temp->next = NULL; // Last node deletion case
        studentTail = temp;
    } else {
        temp->next = toDelete->next;
        toDelete->next->prev = temp;
    }

    free(toDelete->studentData);
    free(toDelete);

    snprintf(rst, 2048, "Student with roll number %d successfully deleted.\n", rollNo);
    return rst;
}

char* modifyStudent(int rollNo, float CGPA) {
    char* rst = (char*)malloc(sizeof(char) * 2048);
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";
    }

    MainDLL* temp = studentHead;
    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        snprintf(rst, 2048, "Student with Roll Number %d not found while trying to modify it...Failed!!\n", rollNo);
        return rst;
    }

    temp->studentData->CGPA = CGPA;
    snprintf(rst, 2048, "Student with roll number %d successfully modified with new CGPA: %.3f.\n", rollNo, CGPA);
    return rst;
}

Student* searchStudent(int rollNo) {
    MainDLL* temp = studentHead;
    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        return NULL;
    }

    return temp->studentData;
}
