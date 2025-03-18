#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "socketdata.h"
#include "student.h"
#include "course.h"
#include "mainDLL.h"

#define BUFFER 4096
#define LISTENQ 10

const char* outputFile = "2024_2102_2124.out";
void printStudentToFile(const char* outputFile);
int readfd;
int flagDecide = 1;

int receiveAPICall(APICall *apiCall, char* response) {
    char* returnMsg;
    switch (apiCall->opType) {
        case addStudent1:
            returnMsg = addStudent(apiCall->parameters.studentPar.rollNo,
                       apiCall->parameters.studentPar.name,
                       apiCall->parameters.studentPar.CGPA,
                       apiCall->parameters.studentPar.NumberOfSubjects);
            break;
        case deleteStudent1:
            returnMsg = deleteStudent(apiCall->parameters.studentPar.rollNo);
            break;
        case modifyStudent1:
            returnMsg = modifyStudent(apiCall->parameters.studentPar.rollNo,
                          apiCall->parameters.studentPar.CGPA);
            break;
        case addCourse1:
            returnMsg = addCourse(apiCall->parameters.coursePar.rollNo,
                      apiCall->parameters.coursePar.marks,
                      apiCall->parameters.coursePar.courseCode,
                      apiCall->parameters.coursePar.x);
            break;
        case deleteCourse1:
            returnMsg = deleteCourse(apiCall->parameters.coursePar.rollNo,
                       apiCall->parameters.coursePar.courseCode);
            break;
        case modifyCourse1:
            returnMsg = modifyCourse(apiCall->parameters.coursePar.rollNo,
                       apiCall->parameters.coursePar.courseCode,
                       apiCall->parameters.coursePar.marks);
            break;
        default:
            printf("Unknown function type received.\n");
            flagDecide = 0;
            break;
    }

    char* resultant = strstr(returnMsg,"Failed");

    if (flagDecide && resultant==NULL) {
        strcpy(response, strcat(returnMsg," :: Operation successful.\n\n"));
    } else {
        strcpy(response, strcat(returnMsg," :: Operation not successful.\n\n"));
    }

    return flagDecide;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Enter the correct number of command line arguments.\n");
        return 1;
    }

    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[BUFFER];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // Initialize server address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port=atoi(argv[1]);
    servaddr.sin_port = htons(port);

    // Bind the socket to the address
    bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    // Start listening
    listen(listenfd, LISTENQ);

    // Initialize client array
    maxfd = listenfd;            /* initialize */
    maxi = -1;                   /* index into client[] array */
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;          /* -1 indicates available entry */

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    printf("Server listening on port %d\n", port);

    for (;;) {
        rset = allset; /* structure assignment */

        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) { /* new client connection */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);

            printf("New connection from %s:%d, socket fd is %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)),ntohs(cliaddr.sin_port),connfd);

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {
                    client[i] = connfd; /* save descriptor */
                    break;
                }

            if (i == FD_SETSIZE) {
                // err_quit("too many clients");
                // fprintf(stderr, "Enter the correct number of command line arguments.\n");
            }

            FD_SET(connfd, &allset); /* add new descriptor to set */

            if (connfd > maxfd)
                maxfd = connfd; /* for select */

            if (i > maxi)
                maxi = i; /* max index in client[] array */

            if (--nready <= 0)
                continue; /* no more readable descriptors */
        }

        // Check all clients for data
        for (i = 0; i <= maxi; i++) {
            if ( (sockfd = client[i]) < 0)
                continue;

        APICall apiCall;
        char response[BUFFER];

            if (FD_ISSET(sockfd, &rset)) {
                n = read(sockfd, &apiCall, sizeof(APICall));
                if (n == 0) {
                    /* connection closed by client */
                    printf("Client on socket %d disconnected.\n", sockfd);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else if (n < 0) {
                    perror("read error");
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else if (n == sizeof(APICall)) {
                    printf("Received APICall from client %d.\n", sockfd);
                    if (receiveAPICall(&apiCall, response)) {
                        printStudentToFile(outputFile);
                    }
                    // Send response to client
                    if (write(sockfd, response, strlen(response)) < 0) {
                        perror("write error");
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        client[i] = -1;
                    } else {
                        printf("Sent to client %d: %s", sockfd, response);
                    }
                } else {
                    // Handle partial reads or unexpected data
                    printf("Unexpected data size from client %d.\n", sockfd);
                }

                if (--nready <= 0)
                    break; /* no more readable descriptors */
            }
        }
    }
    
}
