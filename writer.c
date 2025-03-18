#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "student.h"
#include "course.h"
#include "mainDLL.h"

extern MainDLL* studentHead;

void printStudentToFile(const char *outputFile)
{
    FILE *outputF = fopen(outputFile, "wr");
    if (outputF == NULL) {
        perror("Unable to open file for writing");
        return;
    }
    MainDLL* temp = studentHead;
    while(temp!=NULL)
    {
        fprintf(outputF,"Roll No: %d\n",temp->studentData->rollNo);
        fprintf(outputF,"Name: %s\n",temp->studentData->name);
        fprintf(outputF,"CGPA: %.3f\n",temp->studentData->CGPA);
        fprintf(outputF,"NumberOfSubjects: %d\n",temp->studentData->NumberOfSubjects);
        fprintf(outputF,"Course Information:- \n");

        CoursesDLL* courseTemp = temp->studentData->headCourse;
        while(courseTemp!=NULL){
            fprintf(outputF,"Course Code : %d\n",courseTemp->courseData->courseCode);
            fprintf(outputF,"Marks : %f\n",courseTemp->courseData->marks);

            courseTemp = courseTemp->next;
        }
        fprintf(outputF,"\n");
        temp = temp->next;
    }
    fprintf(outputF,"\n*********************************************\n\n");
    fclose(outputF);
}
