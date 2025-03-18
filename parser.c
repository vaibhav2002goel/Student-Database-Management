#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "socketdata.h"

void parseAddStudents(FILE *inputF);
void parseAddCourses(FILE *inputF);
void parseModifyStudent(FILE *inputF);
void parseModifyCourse(FILE *inputF);
void parseDeleteStudent(FILE *inputF);
void parseDeleteCourse(FILE *inputF);
void parseInitialData(FILE *inputF, char *line);
void sendAPICall(APICall *apiCall);

void parseFile(const char* inputFile) 
{
    FILE *inputF=fopen(inputFile,"r");
    if (inputF==NULL) 
    {
        perror("Error opening input file");
        return;
    }

    char line[4096];
    while (fgets(line,sizeof(line),inputF)) {
        if (line[0]=='#') 
        {
            if (strncmp(line,"# add students",14)==0) 
            {
                parseAddStudents(inputF);
            } 
            else if(strncmp(line,"# add course",12) == 0) 
            {
                parseAddCourses(inputF);
            } 
            else if(strncmp(line,"# modify student",16)==0) 
            {
                parseModifyStudent(inputF);
            } 
            else if(strncmp(line,"# modify course",15)==0) 
            {
                parseModifyCourse(inputF);
            } 
            else if(strncmp(line,"# delete student",16)==0) 
            {
                parseDeleteStudent(inputF);
            } 
            else if(strncmp(line,"# delete course",15)==0) 
            {
                parseDeleteCourse(inputF);
            }
        } 
        else 
        {
            parseInitialData(inputF, line);
        }
    }

    fclose(inputF);
}

void parseAddStudents(FILE *inputF) 
{
    char line[4096];
    long prev;
    while(fgets(line,sizeof(line),inputF)) 
    {
        if (line[0]=='#') 
        {
            fseek(inputF,prev,SEEK_SET);
            break;
        }
        int rollNo, NumberOfSubjects;
        char name[100];
        float CGPA;

        APICall apiCall;
        apiCall.opType=addStudent1;
        sscanf(line, "%d, %99[^,], %f, %d", &rollNo, name, &CGPA, &NumberOfSubjects);
        apiCall.parameters.studentPar.rollNo=rollNo;
        strcpy(apiCall.parameters.studentPar.name,name);
        apiCall.parameters.studentPar.CGPA=CGPA;
        apiCall.parameters.studentPar.NumberOfSubjects=NumberOfSubjects;
        sendAPICall(&apiCall);

        //addStudent(rollNo, name, CGPA, NumberOfSubjects);
        prev = ftell(inputF);
    }
}

void parseAddCourses(FILE *inputF) 
{
    char line[4096];
    long prev;
    while (fgets(line,sizeof(line),inputF)) 
    {
        if (line[0]=='#') 
        {
            fseek(inputF, prev, SEEK_SET);
            break;
        }
        int rollNo, courseCode;
        float marks;
        APICall apiCall;
        apiCall.opType=addCourse1;
        sscanf(line, "%d, %d, %f", &rollNo, &courseCode, &marks);
        apiCall.parameters.coursePar.rollNo=rollNo;
        apiCall.parameters.coursePar.courseCode=courseCode;
        apiCall.parameters.coursePar.marks=marks;
        apiCall.parameters.coursePar.x='a';
        sendAPICall(&apiCall);
        //addCourse(rollNo, marks, courseCode, 'a');
        prev = ftell(inputF);
    }
}

void parseModifyStudent(FILE *inputF) 
{
    char line[4096];
    long prev;
    while(fgets(line,sizeof(line),inputF)) 
    {
        if(line[0]=='#') 
        {
            fseek(inputF, prev, SEEK_SET);
            break;
        }
        int rollNo;
        float CGPA;
        APICall apiCall;
        apiCall.opType=modifyStudent1;
        sscanf(line, "%d, %f", &rollNo, &CGPA);
        apiCall.parameters.studentPar.rollNo=rollNo;
        apiCall.parameters.studentPar.CGPA=CGPA;
        sendAPICall(&apiCall);
        //modifyStudent(rollNo, CGPA);
        prev = ftell(inputF);
    }
}

void parseModifyCourse(FILE *inputF) 
{
    char line[4096];
    long prev;
    while(fgets(line, sizeof(line), inputF)) 
    {
        if (line[0]=='#') 
        {
            fseek(inputF,prev,SEEK_SET);
            break;
        }
        int rollNo, courseCode;
        float marks;
        APICall apiCall;
        apiCall.opType=modifyCourse1;
        if (sscanf(line, "%d, %d, %f", &rollNo, &courseCode, &marks) == 3) {
            apiCall.parameters.coursePar.rollNo=rollNo;
            apiCall.parameters.coursePar.courseCode=courseCode;
            apiCall.parameters.coursePar.marks=marks;
            sendAPICall(&apiCall);
            //modifyCourse(rollNo, courseCode, marks);
        }
        prev = ftell(inputF);
    }
}

void parseDeleteStudent(FILE *inputF) 
{
    char line[4096];
    long prev;
    while (fgets(line,sizeof(line),inputF)) 
    {
        if (line[0] == '#') 
        {
            fseek(inputF,prev,SEEK_SET);
            break;
        }
        int rollNo;
        APICall apiCall;
        apiCall.opType=deleteStudent1;
        sscanf(line, "%d", &rollNo);
        apiCall.parameters.studentPar.rollNo=rollNo;
        sendAPICall(&apiCall);
        //deleteStudent(rollNo);
        prev = ftell(inputF);
    }
}

void parseDeleteCourse(FILE *inputF) 
{
    char line[4096];
    long prev;
    while (fgets(line,sizeof(line),inputF)) 
    {
        if (line[0]=='#') 
        {
            fseek(inputF, prev, SEEK_SET);
            break;
        }
        int rollNo,courseCode;
        APICall apiCall;
        apiCall.opType=deleteCourse1;
        sscanf(line, "%d, %d", &rollNo, &courseCode);
        apiCall.parameters.coursePar.rollNo=rollNo;
        apiCall.parameters.coursePar.courseCode=courseCode;
        sendAPICall(&apiCall);
        //deleteCourse(rollNo, courseCode);
        prev = ftell(inputF);
    }
}

void parseInitialData(FILE *inputF,char *line) 
{
    int rollNo, NumberOfSubjects, courseCode;
    char name[100];
    float CGPA, marks;
    APICall apiCall;
    apiCall.opType=addStudent1;

    APICall apiCall1;
    apiCall1.opType=addCourse1;


    char *nospaceline=line;
    while(isspace(*nospaceline)) 
    {
        nospaceline++;
    }

    if (sscanf(nospaceline, "%d, %99[^,], %f, %d", &rollNo, name, &CGPA, &NumberOfSubjects) == 4) 
    {
        apiCall.parameters.studentPar.rollNo=rollNo;
        strcpy(apiCall.parameters.studentPar.name,name);
        apiCall.parameters.studentPar.CGPA=CGPA;
        apiCall.parameters.studentPar.NumberOfSubjects=NumberOfSubjects;
        sendAPICall(&apiCall);
        //addStudent(rollNo, name, CGPA, NumberOfSubjects);
        int subjectsParsed = 0;
        while (subjectsParsed<NumberOfSubjects && fgets(line,sizeof(line),inputF)) 
        {
            nospaceline=line;
            while(isspace(*nospaceline)) 
            {
                nospaceline++;
            }
            if (sscanf(nospaceline, "%d, %f", &courseCode, &marks)==2) 
            {
                apiCall1.parameters.coursePar.rollNo=rollNo;
                apiCall1.parameters.coursePar.courseCode=courseCode;
                apiCall1.parameters.coursePar.marks=marks;
                apiCall1.parameters.coursePar.x='i';
                sendAPICall(&apiCall1);
                //addCourse(rollNo, marks, courseCode,'i');
                subjectsParsed++;
            }
        }
    }
}

