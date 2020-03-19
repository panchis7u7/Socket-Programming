#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

//(domain)AF_INET -> IPV4, (Type)SOCC_STREAM -> TCP, SOCK_DGRAM ->UDP, 
//(Protocolo) 0 -> default TCP
int main(int argc, char* argv[]){
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
	//Acepta una llamada
	struct sockaddr_in client;
	struct socklen_t client_size = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int client_socket = accept(listening, (struct sockaddr*)&client, &client_size);
	if(client_socket < 0){
		perror("Problema con el cliente!");
		exit(EXIT_FAILURE);
	}
	//Cierra el socket que escucha
	close(listening);
	
	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);
	
	int result = getnameinfo((struct sockaddr*) &client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	if(result){
		printf("%s conectado en %s\n", host, svc);
	} else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		printf("%s conectado en %d\n", host, ntohs(client.sin_port));
	}
	
	//Minetras se recibe, despliega el mensaje
	char buf[4096];
	while(1){
		//Limpia el buffer
		memset(buf,0,4096);
		//Espera el mensaje
		int bytesRecv =recv(client_socket, buf, 4096, 0);
		if(bytesRecv < 0){
			perror("Hubo problemas en la conexion");
			exit(EXIT_FAILURE);
		} else if (bytesRecv == 0) {
			printf("El cliente se desconecto!\n");
			break;
		}
		
		 printf("Recibido: %s", );
		//Despliega el mensaje
		
		//Reenvia en mensaje
	}
	//Cierra el socket
	return 0;
}
