//Calculator
//Agota Kazenaite 2group

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFLEN 1024

int main(int argc, char *argv[]){
#ifdef _WIN32
    WSADATA data;
#endif    
    unsigned int port;
    int s_socket;
    struct sockaddr_in servaddr; // Server address structure

    char buffer[BUFFLEN];

    if (argc != 3){
        fprintf(stderr,"USAGE: %s <ip> <port>\n",argv[0]);
        exit(1);
    }

    port = atoi(argv[2]);

    if ((port < 1) || (port > 65535)){
        printf("ERROR #1: invalid port specified.\n");
        exit(1);
    }

#ifdef _WIN32
    WSAStartup(MAKEWORD(2,2),&data);    
#endif

	//Socket creation
 
    if ((s_socket = socket(AF_INET, SOCK_STREAM,0))< 0){
        fprintf(stderr,"ERROR #2: cannot create socket.\n");
        exit(1);
    }
                                
	//Server structure is cleaned and filled
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET; // protocol (IP)
    servaddr.sin_port = htons(port); // port
    
	
	//symbol line ip to numerical form and server address is set
#ifdef _WIN32
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
#else
    if ( inet_aton(argv[1], &servaddr.sin_addr) <= 0 ) {
        fprintf(stderr,"ERROR #3: Invalid remote IP address.\n");
        exit(1);
    }
#endif        

    
	//Connecting to the server
    if (connect(s_socket,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
        fprintf(stderr,"ERROR #4: error in connect().\n");
        exit(1);
    }
	
	//program logic
	printf("WELCOME to calculator! \nPress any key to continue");
	fgets(buffer, BUFFLEN, stdin);
	memset(&buffer,0,BUFFLEN);
	
	//start of calculation
    
	printf("Enter first number: ");
    fgets(buffer, BUFFLEN, stdin);
	//sending to the server
    send(s_socket,buffer,strlen(buffer),0);
    memset(&buffer,0,BUFFLEN);  
	
	printf("Enter if you want + - / *: ");
    fgets(buffer, BUFFLEN, stdin);
    //sending to the server
    send(s_socket,buffer,strlen(buffer),0);
    memset(&buffer,0,BUFFLEN);  
	
	printf("Enter second number: ");
    fgets(buffer, BUFFLEN, stdin);
	//sending to the server
    send(s_socket,buffer,strlen(buffer),0);
    memset(&buffer,0,BUFFLEN);
    
	
	//Asking to add third number to result
	printf("Do you want to add to result?\n 1 - YES, 0 - NO: ");
    fgets(buffer, BUFFLEN, stdin);
    //sending to the server
    send(s_socket,buffer,strlen(buffer),0);
	
	if(buffer[0] == '1'){
	memset(&buffer,0,BUFFLEN);
		
	printf("Enter if you want + - / *: ");
    fgets(buffer, BUFFLEN, stdin);
    //sending to the server
    send(s_socket,buffer,strlen(buffer),0);
    memset(&buffer,0,BUFFLEN);  
	
	printf("Enter third number: ");
    fgets(buffer, BUFFLEN, stdin);
	//sending to the server
    send(s_socket,buffer,strlen(buffer),0);
	}
	
    memset(&buffer,0,BUFFLEN);
    
	
	//message from server
    recv(s_socket,buffer,BUFFLEN,0);
    printf("\nResult = %s\n", buffer);
	//end of program logic

	//socket closed
    close(s_socket);
    return 0;
}
