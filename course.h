#ifndef COURSE_H
#define COURSE_H

typedef struct course {
    int courseCode;
    float marks;
} Course;

typedef struct coursesDLL {
    struct coursesDLL* prev;
    struct coursesDLL* next;
    Course* courseData;
} CoursesDLL;

char* addCourse(int rollNo, float marks, int courseCode, char x);
char* deleteCourse(int rollNo, int courseCode);
char* modifyCourse(int rollNo, int courseCode, float marks);
Course* searchCourse(int rollNo, int courseCode);

// void addCourse(struct mainDLL* headStudent, int rollNo, float marks, int courseCode);
// void deleteCourse(struct mainDLL* headStudent, int rollNo, int courseCode);
// void modifyCourse(struct mainDLL* headStudent, int rollNo, int courseCode, float marks);
// CourseDLL* searchCourse(struct mainDLL* headStudent, int rollNo, int courseCode);

#endif
