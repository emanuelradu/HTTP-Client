#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "jsmn.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char *jsonFile;
    int i = 0;
    jsmn_parser p;
    jsmntok_t tokens[128];

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", "/task1/start", NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("response: %s\n", response);

    //parsez datele
    jsonFile = strchr(response,'{');
    jsmn_init(&p);
    //in tokens voi obtine campurile parsate din data,
    //delimitate de ""
    jsmn_parse(&p, jsonFile, strlen(jsonFile), tokens, 256);

    char urlTask1[100];
    char urlTask2[100];
    char urlTask3[100];
    char username[100];
    char password[100];
    char user[100];
    char pass[100];
    char data[1000];
    char id2[100];
    char key2[500];
    char jwt[1000];

    //obtin url-ul
    jsmntok_t key = tokens[4];
    unsigned int length = key.end - key.start;
    memcpy(urlTask1, &jsonFile[key.start], length);
    urlTask1[length] = '\0';

    //obtin campul pentru ce indica usernameul
    key = tokens[11];
    length = key.end - key.start;
    memcpy(user, &jsonFile[key.start], length);
    user[length] = '\0';

    //obtin username-ul
    key = tokens[12];
    length = key.end - key.start;
    memcpy(username, &jsonFile[key.start], length);
    username[length] = '\0';

    //obtin campul ce indica parola
    key = tokens[13];
    length = key.end - key.start;
    memcpy(pass, &jsonFile[key.start], length);
    pass[length] = '\0';

    //obtin parola
    key = tokens[14];
    length = key.end - key.start;
    memcpy(password, &jsonFile[key.start], length);
    password[length] = '\0';

    char user_pass[200];

    strcpy(user_pass, user);
    strcat(user_pass, "=");
    strcat(user_pass, username);
    strcat(user_pass, "&");
    strcat(user_pass, pass);
    strcat(user_pass, "=");
    strcat(user_pass, password);

    //cookiurile pentru taskul 2
    char cookie1_1[500];
    char cookie1_2[500];

    char *iterator = strstr(response,"prajiturica");
    i = 0;

    while (iterator[i] != ';')
    {
        cookie1_1[i]=iterator[i];
        i++;
    }
    cookie1_1[i]='\0';

    iterator = strstr(response,"prajiturica.sig");
    i = 0;

    while(iterator[i]!=';')
    {
        cookie1_2[i]=iterator[i];
        i++;
    }
    cookie1_2[i]='\0';

    //le concatenez
    char preparedCookies1[500];
    strcpy(preparedCookies1, cookie1_1);
    strcat(preparedCookies1, ";");
    strcat(preparedCookies1, cookie1_2);

    //construiesc form_data
    char toRequest[10000];
    strcpy(toRequest, preparedCookies1);
    strcat(toRequest, ",");
    strcat(toRequest, user_pass);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("185.118.200.35",urlTask1, toRequest);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("response: %s\n", response);

    //parsez datele
    jsonFile = strchr(response,'{');
    jsmn_init(&p);
    jsmn_parse(&p, jsonFile, strlen(jsonFile), tokens, 256);

    //obtin url-ul
    key = tokens[4];
    length = key.end - key.start;
    memcpy(urlTask2, &jsonFile[key.start], length);
    urlTask2[length] = '\0';

    //obtin authentication token-ul
    key = tokens[10];
    length = key.end - key.start;
    memcpy(jwt, &jsonFile[key.start], length);
    jwt[length] = '\0';

    //obtin id-ul
    key = tokens[13];
    length = key.end - key.start;
    memcpy(id2, &jsonFile[key.start], length);
    id2[length] = '\0';

    //obtin key-ul
    key = tokens[14];
    length = key.end - key.start;
    memcpy(key2, &jsonFile[key.start], length);
    key2[length] = '\0';

    //cookiurile pentru taskul3
    char cookie2_1[1000];
    char cookie2_2[1000];

    iterator = strstr(response,"prajiturica");
    i = 0;

    while(iterator[i]!=';')
    {
        cookie2_1[i]=iterator[i];
        i++;
    }
    cookie2_1[i]='\0';

    iterator = strstr(response,"prajiturica.sig");
    i = 0;

    while(iterator[i]!=';')
    {
        cookie2_2[i]=iterator[i];
        i++;
    }
    cookie2_2[i]='\0';

    //adaug id-ul si cheia
    strcpy(data,"raspuns1=omul&raspuns2=numele&");
    strcat(data,id2);
    strcat(data,"=");
    strcat(data,key2);

    //le concatenez
    char preparedCookies2[500];
    strcpy(preparedCookies2,cookie2_1);
    strcat(preparedCookies2,"; ");
    strcat(preparedCookies2,cookie2_2);

     //construiesc url_params
    strcpy(toRequest, preparedCookies2);
    strcat(toRequest, ",");
    strcat(toRequest, data);
    strcat(toRequest, ",");
    strcat(toRequest, jwt);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35",urlTask2, toRequest);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("response: %s\n", response);

    //parsez datele
    jsonFile = strchr(response,'{');
    jsmn_init(&p);
    jsmn_parse(&p, jsonFile, strlen(jsonFile), tokens, 256);

    //obtin url-ul
    key = tokens[4];
    length = key.end - key.start;
    memcpy(urlTask3, &jsonFile[key.start], length);
    urlTask3[length] = '\0';
    printf("%s \n", urlTask3);

    //cookiurile pentru taskul 4
    char cookie3_1[1000];
    char cookie3_2[1000];

    iterator = strstr(response,"prajiturica");
    i = 0;

    while(iterator[i]!=';')
    {
        cookie3_1[i]=iterator[i];
        i++;
    }
    cookie3_1[i]='\0';

    iterator = strstr(response,"prajiturica.sig");
    i = 0;

    while(iterator[i]!=';')
    {
        cookie3_2[i]=iterator[i];
        i++;
    }
    cookie3_2[i]='\0';

    //le concatenez
    char preparedCookies3[1000];
    strcpy(preparedCookies3,cookie3_1);
    strcat(preparedCookies3,"; ");
    strcat(preparedCookies3,cookie3_2);

    //construiesc url_params
    strcpy(toRequest, preparedCookies3);
    //adaug doua "," pentru ca aici lipseste stringul data
    strcat(toRequest,",,");
    strcat(toRequest, jwt);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35",urlTask3, toRequest);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("response: %s\n", response);

    return 0;
}
