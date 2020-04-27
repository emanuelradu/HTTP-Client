#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *url_params)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *linie = calloc(LINELEN, sizeof(char));

    int ok = 0;

    char cookies[5000];
    char data[5000];
    char jwt[5000];

    if (url_params != NULL)
    {
        //parsez cookiurile
        char* string = strsep(&url_params,",");
        strcpy(cookies, string);

        //parsez data
        string = strsep(&url_params,",");
        strcpy(data, string);

        //parsez jwt-ul
        string= strsep(&url_params,",");
        strcpy(jwt, string);

        sprintf(linie, "GET %s?%s HTTP/1.1", url, data);

        ok = 1;
    }
    else
    {
        sprintf(linie, "GET %s HTTP/1.1", url);
    }

    compute_message(message, linie);

    if (ok == 1)
    {
        if (strlen(cookies))
        {
            sprintf(linie, "Cookie: %s", cookies);
            compute_message(message, linie);
        }
        if (strlen(jwt))
        {
            sprintf(linie, "Authorization: Bearer %s", jwt);
            compute_message(message, linie);
        }
    }

    sprintf(linie, "Host: %s", host);

    compute_message(message, linie);

    compute_message(message, "");

    return message;
}
char *compute_post_request(char *host, char *url, char *form_data)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *linie = calloc(LINELEN, sizeof(char));

    //parsez cookiurile
    char cookies[500];
    char* tempString = strsep(&form_data,",");
    strcpy(cookies,tempString);

    //parsez data
    char data[500];
    tempString = strsep(&form_data,",");
    strcpy(data, tempString);

    sprintf(linie, "POST %s HTTP/1.1", url);
    compute_message(message, linie);

    sprintf(linie, "Host: %s", host);
    compute_message(message, linie);

    if (strlen(cookies))
    {
        sprintf(linie, "Cookie: %s", cookies);
        compute_message(message, linie);
    }

    sprintf(linie, "Content-Type: application/x-www-form-urlencoded");
    compute_message(message, linie);
    sprintf(linie, "Content-Length: %ld", strlen(data));
    compute_message(message, linie);

    compute_message(message, "");

    compute_message(message, data);

    return message;
}
