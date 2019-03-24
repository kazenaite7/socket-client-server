//Calculator
//Agota Kazenaite 2group

#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
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
#include <math.h>

int solution(int num1, int num2, char action, int result) {
   		if(action == '+'){
			result = num1+num2;
		}
		if(action == '-'){
			result = num1-num2;
		}
		if(action == '/'){
			result = num1/num2;
		}
		if(action == '*'){
			result = num1*num2;
		}
		return result;
}


int main(int argc, char *argv []){
#ifdef _WIN32
    WSADATA data;
#endif
    unsigned int port;
    int l_socket; // socket waiting for sign-in
    int c_socket; // client sign-in socket

    struct sockaddr_in servaddr; // Server address structure
    struct sockaddr_in clientaddr; //Signed-in client address structure
    socklen_t clientaddrlen;

    int s_len;
    int r_len;
    char buffer[1024];
    
    if (argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    if ((port < 1) || (port > 65535)){
        printf("ERROR #1: invalid port specified.\n");
        exit(1);
    }

#ifdef _WIN32
    WSAStartup(MAKEWORD(2,2),&data);    
#endif

    //Socket creation
    if ((l_socket = socket(AF_INET, SOCK_STREAM,0))< 0){
        fprintf(stderr,"ERROR #2: cannot create listening socket.\n");
        exit(1);
    }
    
    //Server structure is cleaned and filled
    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)

	//Specifies the IP address which will be waiting for customers
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); // nurodomas portas
 
	//server adress linked to socket
    if (bind (l_socket, (struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        fprintf(stderr,"ERROR #3: bind listening socket.\n");
        exit(1);
    }

	//Specifying  that socket l_socket will wait to sign-up up to 5 client in waiting queue
    if (listen(l_socket, 5) <0){
        fprintf(stderr,"ERROR #4: error in listen().\n");
        exit(1);
    }

    for(;;){
		//Cleans the buffer and client address structure
        memset(&clientaddr,0, sizeof(clientaddr));
        memset(&buffer,0,sizeof(buffer));

		//waits for clients login
        clientaddrlen = sizeof(struct sockaddr);
        if ((c_socket = accept(l_socket,
            (struct sockaddr*)&clientaddr,&clientaddrlen))<0){
            fprintf(stderr,"ERROR #5: error occured accepting connection.\n");
            exit(1);
        }

		//When the client logs in, the data sent by the client is read
		
		//program logic
        s_len = recv(c_socket,buffer,sizeof(buffer),0);
		int firstNumber;
		sscanf(buffer, "%d", &firstNumber);
		printf("First: %d\n",firstNumber);
		
		s_len = recv(c_socket,buffer,sizeof(buffer),0);
		char action = buffer[0];
		printf("Action: %c\n",action);
		
		s_len = recv(c_socket,buffer,sizeof(buffer),0);
		int secondNumber;
		sscanf(buffer, "%d", &secondNumber);
		printf("Second: %d\n",secondNumber);
		
		int result = solution(firstNumber,secondNumber,action,result);
		
		
		//second part
		s_len = recv(c_socket,buffer,sizeof(buffer),0);
		
		if(buffer[0] == '1'){
		printf("Thrid action starts \n");
		
		s_len = recv(c_socket,buffer,sizeof(buffer),0);
		char action = buffer[0];
		printf("Action %c\n",action);
		
		s_len = recv(c_socket,buffer,sizeof(buffer),0);
		int thirdNumber;
		sscanf(buffer, "%d", &thirdNumber);
		
		printf("Third %d\n",thirdNumber);
		
		result = solution(result,thirdNumber,action,result);
		}

		printf("RESULT: %d \n", result);

		//Sending answer to client
		
		//Result to array
		sprintf(buffer,"%ld", result); 
		
        r_len = send(c_socket,buffer,s_len,0);

        printf("IP: %s Sent: %d Received: %d\n",inet_ntoa(clientaddr.sin_addr),
            s_len, r_len
        );
        
		//Client disconnected
        close(c_socket);
    }

    return 0;
}
