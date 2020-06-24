#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

//Ejecute en Linux en el directorio en el que se encuentre, como: ./Server 9898
//Nota Importante!, ejecute el servidor primero y despues el cliente!

//argv -> port number
//(domain)AF_INET -> IPV4, (Type)SOCC_STREAM -> TCP, SOCK_DGRAM ->UDP, 
//(Protocolo) 0 -> default TCP
int main(int argc, char* argv[]){
	if(argc < 2){
		fprintf(stderr, "Numero de puerto no proporcionado, Termino del programa!");
		exit(1);
	}
	
	int listening_sock, new_listening_sock, port_number, n, i;
	struct sockaddr_in server_address, client_address;
	socklen_t client_length;
	char buffer[255];
	 
	//Crear un socket 
	if((listening_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Error al crear Socket!");
		exit(EXIT_FAILURE);
	}
	
	bzero((char*) &server_address, sizeof(server_address)); //Limpia cualquier dato 
	//tambien se puede usar la funcion memset().
	port_number = atoi(argv[1]);
	
	//Enlazar el socket a una IP / puerto
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY; 
	/*Permite que el servidor trabaje sin conocer la direccion IP
	sin_addr is the IP address in the socket, 
	s_addr is variable that holds the information about the address 
	we agree to accept. So, in this case i put INADDR_ANY because 
	i would like to accept connections from any internet address. 
	This case is used about server example. 
	In a client example i could NOT accept connections from ANY ADDRESS.*/
	server_address.sin_port = htons(port_number); //little endian en el puerto gracias al manejo de memoria arriba de 255 en i386
	
	if((bind(listening_sock, (struct sockaddr*) &server_address, sizeof(server_address))) < 0){
		perror("Enlazamiento fallido!");
		exit(EXIT_FAILURE);
	}
	
	client_length = sizeof(client_address);
	
	//Marcar el socket para escuchar (listening)
	if((listen(listening_sock, SOMAXCONN)) < 0){
		perror("No se puede escuchar!");
		exit(EXIT_FAILURE);
	} 
	
	if((new_listening_sock = accept(listening_sock, (struct sockaddr*) &client_address, &client_length)) < 0){
		perror("Problema con el cliente!");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		bzero(buffer, 255);
		if((n = read(new_listening_sock, buffer, 255)) < 0){
			perror("Error al leer los datos!");
			exit(EXIT_FAILURE);
		}
		printf("Client: %s\n", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		
		if((n = write(new_listening_sock, buffer, strlen(buffer))) < 0) {
			perror("Problema al escribir los datos!");
			exit(EXIT_FAILURE);
		}
		
		if((i = strncmp("Adios", buffer, 5)) == 0){
			break;
		} 
	}
	close(new_listening_sock);
	close(listening_sock);
	//pton -> AF_INET formato que se dara,  
	//inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //converts number to an array of integers
	return 0;
}
