#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
	//Crear un socket 
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if(listening < 0){
		perror("Error al crear Socket!");
		exit(EXIT_FAILURE);
	}
	//Enlazar el socket a una IP / puerto
	struct sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);	//little endian en el puerto 54000
	//pton -> AF_INET formato que se dara,  
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //converts number to an array of integers
	if(bind(listening, (struct sockaddr*) &hint, sizeof(hint)) < 0){
		perror("No se pudo enlazar a la IP / Puerto!");
		exit(EXIT_FAILURE);
	}
	//Marcar el socket para escuchar (listening)
	if(listen(listening, SOMAXCONN) < 0){
		perror("No se puede escuchar!");
		exit(EXIT_FAILURE);
	}
	return 0;
}
