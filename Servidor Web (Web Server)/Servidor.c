#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PUERTO 18000
#define BUFFER 4096

char* bin2hex(const unsigned char* , size_t);


int main(int argc, char** argv){
	int listenFd, connFd, n;
	struct sockaddr_in servAddr;
	uint8_t buff[BUFFER+1];
	uint8_t recvLine[BUFFER+1];
	
	if((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("Error al crear socket.");
		
	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PUERTO);
	
	if((bind(listenFd, (struct sockaddr*) &servAddr, sizeof(servAddr))) < 0)
		perror("Error al enlazar.");
		
	if((listen(listenFd, 10)) < 0)
		perror("Error al escuchar.");
		
	for( ; ; ) {
		struct sockaddr_in addr;
		socklen_t addr_len;
		printf("Esperando coneccion en el puerto %d...\n", PUERTO);
		fflush(stdout);
		
		if((connFd = accept(listenFd, (struct sockaddr*) NULL, NULL)) < 0)
			perror("Error al aceptar la conexion.");
		
		memset(recvLine, 0, BUFFER);
		
		while((n = read(connFd, recvLine, BUFFER-1)) > 0){
			fprintf(stdout, "\n%s\n\n%s", bin2hex(recvLine, n), recvLine);
			
		if(recvLine[n-1] == '\n') 
			break;
		}
		
		memset(recvLine, 0, BUFFER);
	}
	
	if (n < 0)
	perror("Error al leer.");
	
	snprintf((char*)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\nHola");
	
	if((write(connFd, (char*)buff, strlen((char*)buff))) < 0)
		perror("Error al escribir.");
	close(connFd);
	return 0;
}

char* bin2hex(const unsigned char* entrada, size_t len){
	if (entrada == NULL || len <= 0)
		return NULL;
		
	char* resultado;
	char* hexits = "0123456789ABCDEF";
	
	int lenResultado = (len*3)+1;
	resultado = malloc(lenResultado);
	bzero(resultado, lenResultado);
	
	int i;
	for (i = 0; i < len; i++)
	{
		resultado[i*3] = hexits[entrada[i]>>4];
		resultado[(i*3)+1] = hexits[entrada[i] & 0x0f];
		resultado[(i*3)+2] = ' ';
	}
	return resultado;
}
