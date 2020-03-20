#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

//Ejecute en Linux en el directorio en el que se encuentre, como: ./Client 127.0.0.1 9898
//Nota Importante!, ejecute el servidor primero y despues el cliente!

//(domain)AF_INET -> IPV4, (Type)SOCC_STREAM -> TCP, SOCK_DGRAM ->UDP, 
//(Protocolo) 0 -> default TCP
/* filename server_ip address portno
 * argv[0] -> Nombre del archivo
 * argv[1] -> Direccion IP del servidor
 * argv[2] -> Numero del puerto
 * */
int main(int argc, char* argv[]){
	
	int sender_sock, port_number, n, i;
	struct sockaddr_in server_address;
	struct hostent* server; //Es usada para almacenar informacion a cerca de un host dado (hostname y direccion IPV4).
	char buffer[255];
	
	
	if(argc < 3) {
		fprintf(stderr, "Uso del puerto de hostname %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	port_number = atoi(argv[2]);
	
	//Crear un socket 
	if((sender_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error al abrir socket!");
		exit(EXIT_FAILURE);
	}
	
	if((server = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr, "Error!, no existe tal host!");
		exit(EXIT_FAILURE);
	}
	
	//Enlazar el socket a una IP / puerto
	bzero((char*) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port_number);
	if((connect(sender_sock, (struct sockaddr*) &server_address, sizeof(server_address))) < 0){
		perror("Conexion fallida!");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		if((n = write(sender_sock, buffer, strlen(buffer))) < 0) {
			perror("Problema al escribir los datos!");
			exit(EXIT_FAILURE);
		}
		
		bzero(buffer, 255);
		if((n = read(sender_sock, buffer, 255)) < 0) {
			perror("Problema al leer los datos!");
			exit(EXIT_FAILURE);
		}
		
		printf("Server: %s\n", buffer);
		
		if((i = strncmp("Adios", buffer, 5)) == 0) {
			break;
		}
	}
	close(sender_sock);
	return 0;
	//int bytesRecv =recv(client_socket, buf, 4096, 0);
}
