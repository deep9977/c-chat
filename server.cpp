#include<stdio.h>
#include<iostream>
#include<sys/socket.h> //socket 
#include<errno.h>
#include<netinet/in.h> //connect ipv4 
#include<arpa/inet.h> //network to binary 
#include<pthread.h>
#include<string.h> //memset

void* receive(void* arg){
    int socketfd = *(int *) arg ;
    char buffer11[1000];
    while(true){
        int recvfd = recv(socketfd , (void *) &buffer11, (sizeof(buffer11)/2) , NULL);
        if(recvfd == -1){
            perror("recv ");
            exit(EXIT_FAILURE);
        }
        printf("%s ", buffer11);
        memset(buffer11 , 0 , (sizeof(buffer11)/2));
    }
}

void* send11(void* arg ){
    int socketfd = *(int *) arg;
    char buffer11[1000];
    char newline[] = "\n";
    while(true){
        scanf("%s", &buffer11 );
        int sendfd = send(socketfd , buffer11 , (sizeof(buffer11)/2) , NULL );
        if(sendfd == -1){
            perror("send main ");
            exit(EXIT_FAILURE);
        }
        int sendnfd = send(socketfd , newline , (sizeof(newline)/2) , NULL );
        if(sendnfd == -1){
            perror("send newline ");
            exit(EXIT_FAILURE);
        }
 
        memset(buffer11 , 0 , (sizeof(buffer11)/2));
    }

}

int main(){
//SOCKET 
int socketfd =  socket(AF_INET , SOCK_STREAM , 0 );
if(socketfd == -1 ){
    perror("socket ");
    exit(EXIT_FAILURE);
}
//BIND
struct sockaddr_in server_addr {AF_INET , htons(6969) , 0 };
inet_pton(AF_INET , "127.0.0.1" , &server_addr.sin_addr);
int bindfd = bind(socketfd , (struct sockaddr*) &server_addr , sizeof(server_addr));
if(bindfd == -1){
    perror("bind ");
    exit(EXIT_FAILURE);
}
//LISTEN
int listenfd = listen(socketfd , 0);
if(listenfd == -1){
    perror("listen ");
    exit(EXIT_FAILURE);
}
//ACCEPT
struct sockaddr client_addr{0};
unsigned int a = sizeof(client_addr); 
int acceptfd = accept(socketfd , (struct sockaddr*) & client_addr , &a);
if(acceptfd == -1){
    perror("accept ");
    exit(EXIT_FAILURE);
}
//RECV THREAD 
pthread_t t1 ;
pthread_create(&t1 , NULL , receive , &acceptfd);
//SEND THREAD 
pthread_t t2 ;
pthread_create(&t2 , NULL , send11 , &acceptfd);
//join thread 
pthread_join(t1 , 0);
pthread_join(t2 , 0);


return 0;
}
