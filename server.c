#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/* Function to handle errors */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    int portno;
    socklen_t clilen;
    char buffer[256];

    struct sockaddr_in serv_addr, cli_addr;

    /* Check if port number is provided */
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* Clear server address structure */
    memset(&serv_addr, 0, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* Bind socket to address */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    /* Listen for incoming connections */
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    /* Accept a client connection */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    /* Read message from client */
    memset(buffer, 0, sizeof(buffer));
    if (read(newsockfd, buffer, 255) < 0)
        error("ERROR reading from socket");

    printf("Here is the message: %s\n", buffer);

    /* Send response to client */
    if (write(newsockfd, "I got your message", 18) < 0)
        error("ERROR writing to socket");

    /* Close sockets */
    close(newsockfd);
    close(sockfd);

    return 0;
}

