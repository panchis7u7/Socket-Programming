#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <netdb.h>

#define PUERTO_SERVIDOR 80 //Puerto en el que el servidor escucha (http);
#define BUFFER_MAX 4096 //Buffer.


int main(int argc, char** argv){
	int sockfd, n;
	int bytesEnviados;
	struct sockaddr_in servAddr;
	char sendLine[BUFFER_MAX];
	char recvLine[BUFFER_MAX];
	
	if(argc != 2)
	perror("Donde esta la ip?");
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0))
	perror("Error al abrir socket.");
	
	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PUERTO_SERVIDOR);
	
	if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr) <= 0)
	printf("error para %s", argv[1]);

	if(connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	perror("error al conectar.");
	
	sprintf(sendLine, "GET  HTTP/1.0\r\n\r\n");
	bytesEnviados = strlen(sendLine);
	
	if(write(sockfd, sendLine, bytesEnviados) != bytesEnviados)
	perror("Error en la escritura.");
	
	while(n = read(sockfd, recvLine, BUFFER_MAX-1) > 0) {
		printf("%s", recvLine);
		memset(recvLine, 0, BUFFER_MAX);
	}
	
	if(n < 0)
	perror("Error de lectura.");
	
	exit(0);
	return 0;
}
