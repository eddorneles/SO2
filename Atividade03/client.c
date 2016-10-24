/****************************************************************************
Sistemas Operacionais II
ATIVIDADE 03 - Implementação de comunicação por sockets
Ver instruções no arquivo sockets.pdf
Dupla:
    -Eduardo Dorneles
    -Igor Cavalcanti

O modelo abordado nessa atividade é o modelo cliente/servidor, portanto
teremos um código para o cliente e um código para o servidor.
ESTE CÓDIGO ESTÁ DESTINADO PARA O CLIENTE
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* contém memset()*/
#include <unistd.h> /* contém close() */
#include <sys/types.h> /* contém definições de tipos de dados usados nas system calls */
#include <sys/socket.h> /* inclui estruturas necessárias para os sockets */
#include <netinet/in.h> /* contém constantes e estruturas necessárias para endereços
                            de domínio da internet */
#include <netdb.h> /* define a struct hostent que será usada */
#include <strings.h>
#include <time.h>

#include <arpa/inet.h>

#define ONE_KB 1024
#define ITERATIONS 1000


void startingClient( int *client_socket , struct sockaddr_in  *server_address , char *buffer , int *msg_length );

void setupClient( struct hostent *server , struct sockaddr_in *server_address, int port_number );

void finishingClientExecution( int client_socket , char *buffer );

char *generateRandomMessage( int msg_length );

void error( char * msg );

int main(int argc, char *argv[]){
    int socket_fd, port_number , msg_length;
    struct sockaddr_in server_address;

    char *buffer;

    startingClient(&socket_fd, &server_address, buffer, &msg_length);

    buffer = (char *) malloc( sizeof(char) * (msg_length) );

    if( buffer == NULL ){
        error( "Não foi possível criar o buffer de mensagens");
    }

    clientCommunication(socket_fd, &server_address, buffer, msg_length);

    finishingClientExecution(socket_fd, buffer);

    return 0;
}

void startingClient( int *client_socket , struct sockaddr_in  *server_address ,
    char *buffer , int *msg_length ){
    int port_number;
    struct hostent server;

    printf( "Entre com a porta do servidor que o cliente se conectará: " );
    scanf( "%d" , &port_number );
    if( port_number < 1 ){
        error( "Porta inválida");
    }
    printf("Entre com o tamanho da mensagem em KB\n",
            "Entre com zero para mensagens de um byte: " );
    scanf("%d", msg_length );
    if( *msg_length == 0 ){
        *msg_length = 1;
    }else if( *msg_length > 0 ){
        *msg_length *= ONE_KB;
    }else{
        error("Tamanho de mensagem inválido");
    }

    *client_socket = socket( AF_INET , SOCK_STREAM , 0 );

    if( *client_socket < 0 ){
        error( "Falha ao criar o socket cliente" );
    }

    setupClient( &server , server_address , port_number );

    if( connect( *client_socket , (struct sockaddr*) server_address , sizeof(*server_address)  ) < 0 ){
        error( "Não foi possível se conectar ao servidor");
    }
}

void setupClient( struct hostent *server , struct sockaddr_in *server_address ,
        int port_number ){

    memset(server_address, 0, sizeof(server_address));
    (*server_address).sin_family = AF_INET;
    (*server_address).sin_port = htons(port_number);
    (*server_address).sin_addr.s_addr = inet_addr("127.0.0.1");

    //server = gethostbyname( "localhost" );
    //memset( server_address , 0 , sizeof(server_address) );
    //memcpy( (void*) server->h_addr ,  (void*)server_address->sin_addr.s_addr , server->h_length );
    //server_address->sin_port = htons( port_number );
}

void clientCommunication( int client_socket , struct sockaddr_in *server_address ,
        char *buffer, int msg_length ){
    int cur_iteration=1;
    char *msg=NULL;

    if( client_socket > 0 && server_address != NULL && buffer != NULL ){
        while( cur_iteration <= 1 ){
            msg = generateRandomMessage( msg_length );
            if( msg != NULL){
                puts(msg);
                //write( client_socket , msg , msg_length );
                if (send(client_socket, msg, strlen(msg), 0) < 0) {
                    error("Erro ao enviar mensagem!");
                }
                memset( buffer , 0 , msg_length );
                read( client_socket , buffer , msg_length );
                free(msg);
            }
            puts(msg);
            //printf( "Mensagem %d: %s" , cur_iteration , buffer );
            cur_iteration++;
        }
    }
}

char *generateRandomMessage( int msg_length ){
    int i = 0;
    char *msg=NULL;
    srand(time(NULL));

    msg = (char*) malloc( sizeof(char)*(msg_length) );
    if( msg != NULL ){
        while( i < msg_length-1 ){
            msg[i] = rand() % 256;
            i++;
        }
        msg[msg_length-1] = '\0';
        return msg;
    }
    return NULL;
}

void finishingClientExecution( int client_socket , char *buffer ){
    if( client_socket > 0 ){
        close( client_socket);
    }
    if( buffer != NULL ){
        free(buffer);
    }
}

void error( char * msg ){
    perror( msg );
    exit(1);
}
