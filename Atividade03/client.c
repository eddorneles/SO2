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
#include <strings.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/time.h>

#define ONE_KB 1024
#define ITERATIONS 1000


void startingClient( int *client_socket , struct sockaddr_in  *server_address, int port_number );

void setupClient( struct sockaddr_in *server_address, int port_number );

char *generateRandomMessage( int msg_length );

void clientCommunication( int client_socket , struct sockaddr_in *server_address , int msg_length );

void error( char * msg );

char content[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Y', 'Z'};

int main(int argc, char *argv[]){
    int socket_fd, port_number , msg_length;
    struct sockaddr_in server_address;
    char ip_server[16];

    if(argc < 4){
        error("Argumentos incorretos! <ip servidor> <porta> <tamanho msg em KB>\n");
    }
    strcpy(ip_server, argv[1]);
    port_number = atoi(argv[2]);
    msg_length = atoi(argv[3]);
    msg_length *= ONE_KB;

    srand(time(NULL));

    startingClient(&socket_fd, &server_address, port_number);

    clientCommunication(socket_fd, &server_address, msg_length);

    close( socket_fd );

    return 0;
}

void startingClient( int *client_socket , struct sockaddr_in  *server_address , int port_number ){

    *client_socket = socket( AF_INET , SOCK_STREAM , 0 );

    if( *client_socket < 0 ){
        error( "Falha ao criar o socket cliente" );
    }

    setupClient( server_address , port_number );

    if( connect( *client_socket , (struct sockaddr*) server_address , sizeof(*server_address)  ) < 0 ){
        error( "Não foi possível se conectar ao servidor");
    }
}

void setupClient( struct sockaddr_in *server_address , int port_number ){

    memset(server_address, 0, sizeof(*server_address));
    (*server_address).sin_family = AF_INET;
    (*server_address).sin_port = htons(port_number);
    (*server_address).sin_addr.s_addr = inet_addr("127.0.0.1");

}

void clientCommunication( int client_socket , struct sockaddr_in *server_address, int msg_length ){
    int cur_iteration=1;
    char *msg = NULL;
    time_t t_start, t_end;

    if( client_socket > 0 && server_address != NULL){
        time(&t_start);
        while( cur_iteration <= ITERATIONS ){
            msg = generateRandomMessage( msg_length );
            if( msg != NULL){
                puts(msg);
                if (send(client_socket, msg, strlen(msg), 0) < 0) {
                    error("Erro ao enviar mensagem!");
                }
                free(msg);
            }
            cur_iteration++;
            fflush(stdout);
        }
        time(&t_end);
        printf("Tempo total apos %d mensagens enviadas: %f", ITERATIONS, difftime(t_end, t_start));
    }
}

char *generateRandomMessage( int msg_length ){
    int i = 0;
    char *msg = NULL;

    msg = (char*) malloc( sizeof(char)*(msg_length) );
    if( msg != NULL ){
        while( i < msg_length-1 ){
            msg[i] = content[rand() % 26];
            i++;
        }
        msg[msg_length-1] = '\0';
        return msg;
    }
    return NULL;
}

void error( char * msg ){
    perror( msg );
    exit(1);
}
