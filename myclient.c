/*
Unix programming assignment 3
All aspects of code work and 5 random questions are succesffuly asked and answered
Score is given at end of quiz
Provided iclient.c and iserver.c files weere used as templates
*/
//Client programme

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define BUFSIZE 1024
#define MAXSIZE 32

int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
       fprintf(stderr, "Usage: %s <IP address of server> <port number>.\n",
               argv[0]);
       exit(-1);
    }

    struct sockaddr_in serverAddress;
   
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
       fprintf(stderr, "socket() error.\n");
       exit(-1);
    }

    int rc = connect(cfd, (struct sockaddr *)&serverAddress, 
                     sizeof(struct sockaddr));
    if (rc == -1) {
       fprintf(stderr, "connect() error, errno %d.\n", errno);
       exit(-1);
    }

    char buf[BUFSIZE];

    char* bufr = buf;
    const char* bufw = buf;
    ssize_t numRead = read(cfd, bufr, BUFSIZE);
        if (numRead == -1) {
                fprintf(stderr, "Read error, Errno %d.\n", errno);
            }

    printf("%s\n", buf);

    int flag = 0;
    char c[MAXSIZE];
    char ans[MAXSIZE];
    for(;;)
    {
        if (fgets(c, MAXSIZE*sizeof(char), stdin) == NULL) {
        printf("Input error.\n");
        exit(1);
        }

        if(strcmp(c, "q\n") == 0){
            exit(0);
        } 
        if(strcmp(c, "Y\n") != 0){
            printf("Error: invalid key entered\nTo start the quiz, press Y and <enter>.\nTo quit the quiz, press q and <enter>.\n");
        } 
        else if(strcmp(c, "Y\n") == 0){
            break; //User wants to play
        }   
    }

    strcpy(buf, c);
    ssize_t numWritten = write(cfd, bufw, BUFSIZE); //Write user response so server knows user wants to play
    if (numWritten <= 0) {
            fprintf(stderr, "Write error.\n");
            exit(EXIT_FAILURE);
        }
    
    for(int j = 0; j < 5; j++) //For loop for 5 questions
    {
        numRead = read(cfd, bufr, BUFSIZE); //Read in question
        if (numRead == -1) {
            fprintf(stderr, "Read error, Errno %d.\n", errno);
        }
        printf("%s\n", buf);

        if (fgets(ans, MAXSIZE*sizeof(char), stdin) == NULL) { //Get answer to question from user
            printf("Input error.\n");
            exit(1);
        }

        strcpy(buf, ans);
        numWritten = write(cfd, bufw, BUFSIZE); //Write answer to server
            if (numWritten <= 0) {
                fprintf(stderr, "Write error.\n");
                exit(EXIT_FAILURE);
            }

        numRead = read(cfd, bufr, BUFSIZE); //Read in question
        if (numRead == -1) {
            fprintf(stderr, "Read error, Errno %d.\n", errno);
        }
        printf("%s\n", buf);
    }

    numRead = read(cfd, bufr, BUFSIZE); //Read in score
    if (numRead == -1) {
            fprintf(stderr, "Read error, Errno %d.\n", errno);
        }
    printf("%s\n", buf);
        

    if (close(cfd) == -1) /* Close connection */
    {
        fprintf(stderr, "close error.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
