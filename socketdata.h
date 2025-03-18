#ifndef SOCKETDATA_H
#define SOCKETDATA_H

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

typedef enum{
    addStudent1,
    modifyStudent1,
    deleteStudent1,
    searchStudent1,
    printStudent1,
    addCourse1,
    modifyCourse1,
    deleteCourse1,
    searchCourse1
}operationTypes;

typedef struct {
    int rollNo;
    char name[100];
    float CGPA;
    int NumberOfSubjects;
} studentParameters;

typedef struct {
    int rollNo;
    int courseCode;
    float marks;
    char x;
} courseParameters;

typedef union {
    studentParameters studentPar;
    courseParameters coursePar;
} operationParameters;

typedef struct {
    operationTypes opType;
    operationParameters parameters;
} APICall;


#endif 
