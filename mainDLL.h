#ifndef MAINDLL_H
#define MAINDLL_H

#include "student.h"

typedef struct MainDLL {
    struct MainDLL* prev;
    struct MainDLL* next;
    struct student* studentData;
} MainDLL;

#endif 
