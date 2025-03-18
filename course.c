#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "course.h"
#include "mainDLL.h"

extern MainDLL* studentHead;

char* addCourse(int rollNo, float marks, int courseCode, char x) {
    char* rst = (char*)malloc(sizeof(char) * 2048);  // Allocate memory for return message
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";  // Handle memory allocation failure
    }

    MainDLL* temp = studentHead;
    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        snprintf(rst, 2048, "Student with Roll Number %d not found while trying to add course...Failed!!\n", rollNo);
        return rst;
    }

    if (searchCourse(rollNo, courseCode)) {
        snprintf(rst, 2048, "Course with course code %d already registered for student with roll number %d...Failed!!\n", courseCode, rollNo);
        return rst;
    }

    // Allocate and initialize the new course node
    CoursesDLL* newCourseNode = (CoursesDLL*)malloc(sizeof(CoursesDLL));
    if (newCourseNode == NULL) {
        free(rst);
        return "Memory allocation failed for new course node...Failed!!\n";
    }

    Course* newCourse = (Course*)malloc(sizeof(Course));
    if (newCourse == NULL) {
        free(rst);
        free(newCourseNode);
        return "Memory allocation failed for new course...Failed!!\n";
    }

    newCourse->courseCode = courseCode;
    newCourse->marks = marks;
    newCourseNode->courseData = newCourse;
    newCourseNode->next = NULL;

    if (temp->studentData->headCourse == NULL) {
        newCourseNode->prev = NULL;
        temp->studentData->headCourse = newCourseNode;
    } else {
        CoursesDLL* tempCourse = temp->studentData->headCourse;
        while (tempCourse->next != NULL) {
            tempCourse = tempCourse->next;
        }
        tempCourse->next = newCourseNode;
        newCourseNode->prev = tempCourse;
    }

    if (x != 'i') {
        temp->studentData->NumberOfSubjects += 1;
    }

    snprintf(rst, 2048, "Course with course code %d successfully added for student with roll number %d.\n", courseCode, rollNo);
    return rst;
}

char* deleteCourse(int rollNo, int courseCode) {
    MainDLL* temp = studentHead;
    char* rst = (char*)malloc(sizeof(char) * 2048);
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";
    }

    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        snprintf(rst, 2048, "Student with roll number %d not found while trying to delete a course...Failed!!\n", rollNo);
        return rst;
    }

    CoursesDLL* tempCourse = temp->studentData->headCourse;
    while (tempCourse != NULL && tempCourse->courseData->courseCode != courseCode) {
        tempCourse = tempCourse->next;
    }

    if (tempCourse == NULL) {
        snprintf(rst, 2048, "Course with course code %d not found for student with roll number %d...Failed!!\n", courseCode, rollNo);
        return rst;
    }

    // Remove the course from the list
    if (tempCourse->prev != NULL) {
        tempCourse->prev->next = tempCourse->next;
    } else {
        temp->studentData->headCourse = tempCourse->next;
    }

    if (tempCourse->next != NULL) {
        tempCourse->next->prev = tempCourse->prev;
    }

    free(tempCourse->courseData);
    free(tempCourse);

    temp->studentData->NumberOfSubjects -= 1;
    if (temp->studentData->NumberOfSubjects <= 0) {
        temp->studentData->NumberOfSubjects = 0;
    }

    snprintf(rst, 2048, "Course with course code %d successfully deleted for student with roll number %d.\n", courseCode, rollNo);
    return rst;
}

char* modifyCourse(int rollNo, int courseCode, float marks) {
    char* rst = (char*)malloc(sizeof(char) * 2048);
    if (rst == NULL) {
        return "Memory allocation failed...Failed!!\n";
    }

    MainDLL* temp = studentHead;
    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        snprintf(rst, 2048, "Student with roll number %d not found while trying to modify a course...Failed!!\n", rollNo);
        return rst;
    }

    CoursesDLL* tempCourse = temp->studentData->headCourse;
    while (tempCourse != NULL && tempCourse->courseData->courseCode != courseCode) {
        tempCourse = tempCourse->next;
    }

    if (tempCourse == NULL) {
        snprintf(rst, 2048, "Course with course code %d not found for student with roll number %d...Failed!!\n", courseCode, rollNo);
    } else {
        tempCourse->courseData->marks = marks;
        snprintf(rst, 2048, "Course with course code %d successfully modified for student with roll number %d.\n", courseCode, rollNo);
    }

    return rst;
}

Course* searchCourse(int rollNo, int courseCode) {
    MainDLL* temp = studentHead;
    while (temp != NULL && temp->studentData->rollNo != rollNo) {
        temp = temp->next;
    }

    if (temp == NULL) {
        fprintf(stderr, "Student with roll number %d not found while searching for a course.\n", rollNo);
        return NULL;
    }

    CoursesDLL* tempCourse = temp->studentData->headCourse;
    while (tempCourse != NULL && tempCourse->courseData->courseCode != courseCode) {
        tempCourse = tempCourse->next;
    }

    if (tempCourse == NULL) {
        return NULL;
    }

    return tempCourse->courseData;
}
