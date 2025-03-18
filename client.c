#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>  // For using 'bool' type
#include "socketdata.h"

#define BUFFER 4096

int sockfd;

void parseFile(const char* inputFile);

void sendAPICall(APICall *apiCall) {
    if (sockfd < 0) {
        perror("Socket is not open");
        exit(1);
    }
    if (write(sockfd, apiCall, sizeof(APICall)) != sizeof(APICall)) {
        perror("write error");
    }
}

void handle_sigpipe(int signum) {
    fprintf(stderr, "Server is not running or has closed the connection (SIGPIPE received).\n");
    exit(2);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <Server_IP> <Port> <Input_File>\n", argv[0]);
        return 1;
    }

    const char* inputFile = argv[3];
    char recvline[BUFFER];
    ssize_t n;
    fd_set readfds;
    struct timeval timeout;

    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    int port = atoi(argv[2]);
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address! Address not supported.\n");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", argv[1], port);

    // Parse and send initial input file
    APICall *apiCalls = NULL;
    parseFile(inputFile);

    // Receive response after sending the file
    //int bef = 0;
    //int aft = 1;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        //printf("bef:%d\n",bef);
        //n = read(sockfd, recvline, BUFFER - 1);
        //printf("aft:%d\n",aft);
        //bef++;
        //aft++;
        if (activity == -1) {
            perror("select error");
            break;
        } else if (activity == 0) {
            // Timeout occurred, no data received within 20 seconds
            printf("No data received for 20 seconds. Exiting...\n");
            break;
        }
        else{
            n = read(sockfd, recvline, BUFFER - 1);
        if (n > 0) {
            recvline[n] = '\0';  // Null-terminate the response
            printf("%s\n", recvline);

            // Check if the response contains the "EOF" marker to signal end of response
            // if (strstr(recvline, "EOF\n") != NULL) {
            //     printf("End of response detected (EOF received).\n");
            //     break; // Exit the loop when "EOF" is found
            // }
        } else if (n == 0) {
            // The server closed the connection
            printf("Server closed the connection.\n");
            break;
        } else {
            perror("read error");
            exit(1);
        }
        }
    }
    free(apiCalls);

    // User interaction loop
    while (1) {
        char choice[10];
        printf("Enter 'QUIT' to exit or 'FILE' to send a new input file: ");
        if (scanf("%s", choice) != 1) {
            fprintf(stderr, "Input error.\n");
            break;
        }

        if (strcasecmp(choice, "QUIT") == 0) {
            printf("Exiting.\n");
            break;
        } else if (strcasecmp(choice, "FILE") == 0) {
            char newFile[256];
            printf("Enter the new input file path: ");
            if (scanf("%s", newFile) != 1) {
                fprintf(stderr, "Input error.\n");
                continue;
            }

            // Parse and send the new input file
            APICall *newApiCalls = NULL;
            parseFile(newFile);

            // Receive server response
            while (1) {
                n = read(sockfd, recvline, BUFFER - 1);
                if (n > 0) {
                    //recvline[n] = '\0';  // Null-terminate the response
                    printf("Server response: %s\n", recvline);

                    // Check if the response contains the "EOF" marker
                    if (strstr(recvline, "EOF\n") != NULL) {
                        printf("End of response detected (EOF received).\n");
                        break; // Exit the loop when "EOF" is found
                    }
                } else if (n == 0) {
                    printf("Server closed connection.\n");
                    close(sockfd);
                    exit(0);
                } else {
                    perror("read error");
                    exit(1);
                }
            }
            free(newApiCalls);
        } else {
            printf("Invalid choice. Please enter 'QUIT' or 'FILE'.\n");
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}

