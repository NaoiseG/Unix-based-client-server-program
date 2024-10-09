/*
Unix programming assignment 3
All aspects of code work and 5 random questions are succesffuly asked and answered
Score is given at end of quiz
Provided iclient.c and iserver.c files weere used as templates
*/
//Server programme

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include "quiz.h"

#define SERVERIP "127.0.0.1"
#define BACKLOG 10
#define BUFSIZE 1024
extern char  *QuizQ[43];
extern char* QuizA[43];

int* get_random_numbers();



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
       fprintf(stderr, "Usage: %s <port number>.\n",
               argv[0]);
       exit(-1);
    }

    struct sockaddr_in serverAddress;

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) 
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    memset(&serverAddress, 0, sizeof(struct sockaddr_in));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVERIP);
    serverAddress.sin_port = htons(atoi(argv[1]));

    int optval = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval)) == -1)
       exit(-1);

    // Binding newly created socket to given IP and verification
    if ((bind(lfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr))) != 0) 
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(lfd, BACKLOG)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        fprintf(stdout, "Listening on (%s, %s)\n", SERVERIP, argv[1]);

    /*
    **************************************
    Handling cients
    **************************************
    */

    for(;;)
    {
        fprintf(stdout, "<waiting for clients to connect>\n");
        fprintf(stdout, "<ctrl-C to terminate>\n");

        struct sockaddr_storage claddr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);

        int cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
        if (cfd == -1) // Failed to create socket
        {
        continue;
        }

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        if (getnameinfo((struct sockaddr *) &claddr, addrlen,
                    host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            {
            fprintf(stdout, "Connection from (%s, %s)\n", host, service);
            }
        else
            fprintf(stderr, "Connection from (?UNKNOWN?)");


        char buf[BUFSIZE]; 
        
        strcpy(buf, "Welcome to Unix Programming Quiz!\nThe quiz comprises five questions posed to you one after the other.\nYou have only one attempt to answer a question.\nYour final score will be sent to you after conclusion of the quiz.\nTo start the quiz, press Y and <enter>.\nTo quit the quiz, press q and <enter>.\n");
        const char* bufw = buf;
        char* bufr = buf;
        ssize_t numWritten = write(cfd, bufw, BUFSIZE);
        if (numWritten <= 0) {
            if (numWritten == -1 && errno == EINTR)
                continue;
            else {
                fprintf(stderr, "Write error.\n");
                exit(EXIT_FAILURE);
            }
        }

        ssize_t numRead = read(cfd, bufr, BUFSIZE); //Read in Y or q
        if (numRead == -1) {
                fprintf(stderr, "Read error, Errno %d.\n", errno);
            }
        
        if(strcmp(buf, "Y\n") == 0){ //User wants to play
            int count = 0;
            int* nums = get_random_numbers();
            for(int j=0; j < 5; j++) //Quiz
            {
                strcpy(buf, QuizQ[nums[j]]);
                numWritten = write(cfd, bufw, BUFSIZE); //Write question to client
                if (numWritten <= 0) {
                    fprintf(stderr, "Write error.\n");
                    exit(EXIT_FAILURE);
                }

                numRead = read(cfd, bufr, BUFSIZE); //Read answer from client
                if (numRead == -1) {
                    fprintf(stderr, "Read error, Errno %d.\n", errno);
                }

                buf[strcspn(buf, "\n")] = 0; // removes the trailing newline

                if(strcmp(QuizA[nums[j]], buf) == 0) //Check for correct answer
                {
                    count +=1;
                    strcpy(buf, "Right Answer.");
                }
                else{
                    sprintf(buf, "Wrong Answer. Right answer is <%s>.", QuizA[nums[j]]);
                }

                numWritten = write(cfd, bufw, BUFSIZE); //Write correct or incorrect to client
                if (numWritten <= 0) {
                    fprintf(stderr, "Write error.\n");
                    exit(EXIT_FAILURE);
                }
                
            }
        sprintf(buf, "Your quiz score is %d/5. Goodbye!\n", count); //Write score to client
        numWritten = write(cfd, bufw, BUFSIZE);
        if (numWritten <= 0) {
                    fprintf(stderr, "Write error.\n");
                    exit(EXIT_FAILURE);
         }
        }
            


        if (close(cfd) == -1) /* Close connection */
        {
           fprintf(stderr, "close error.\n");
           exit(EXIT_FAILURE);
        }
        
    }

    return 0;
}

int* get_random_numbers()
{
    static int nums[5] = {-1, -1, -1, -1, -1};  // array to store the random numbers
    int n, j;

    srand(time(NULL));  // set the seed value based on the current time

    for (int i = 0; i < 5; i++) {
        do {
            n = rand() % 43;  // generate a random number between 0 and 43
            // check if the number has already been generated
            for (j = 0; j < i; j++) {
                if (n == nums[j]) {
                    break;
                }
            }
        } while (j < i);  // repeat until original number generated
        nums[i] = n;  // store new number in the array
    }

    return nums;
}