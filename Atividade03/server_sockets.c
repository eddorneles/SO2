/****************************************************************************
Sistemas Operacionais II
ATIVIDADE 03 - Implementação de comunicação por sockets
Ver instruções no arquivo sockets.pdf
Dupla:
    -Eduardo Dorneles
    -Igor Cavalcanti

O modelo abordado nessa atividade é o modelo cliente/servidor, portanto
teremos um código para o cliente e um código para o servidor.
ESTE CÓDIGO ESTÁ DESTINADO PARA O SERVIDOR

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* contém memset()*/
#include <unistd.h> /* contém close() */
#include <sys/types.h> /* contém definições de tipos de dados usados nas system calls */
#include <sys/socket.h> /* inclui estruturas necessárias para os sockets */
#include <netinet/in.h> /* contém constantes e estruturas necessárias para endereços
                            de domínio da internet */

#define ONE_KB 1024
#define ITERATIONS 1000
#define BUFFER_LENGTH 256

void startingExecution( int *socket_file_descriptor, int *port_number,
    struct sockaddr_in *server_address, struct sockaddr_in * client_address );

int main( int argc , char **argv ){
    int socket_server_id = -1; /* Será o descritor que identifica o socket do servidor */
    int msg_length,
        port_number = 0, /* armazena o número que o servidor escutar */
        client_addr_len, /* tamanho do endereço do cliente */
        nbytes_read; /* número de bytes lidos ou escritos */
    char buffer[BUFFER_LENGTH];
    struct sockaddr_in /* struct que contém endereço de rede */
        server_address , client_address;


    startingExecution( &socket_server_id , &port_number, &server_address, &client_address );
    /* Comentários temporário para teste
    printf( "Entre com o número de kbytes de cada mensagem, zero para um byte: " );
    scanf( "%d" , &msg_length );
    */
    if( msg_length == 0 ){
        msg_length = 1;
    }else{
        msg_length *= ONE_KB;
    }
    if( socket_server_id != -1 ){
        close( socket_server_id );
    }
    printf("Terminou a execução com sucesso!\n" );
}

void startingExecution( int *socket_file_descriptor, int *port_number,
    struct sockaddr_in *server_address, struct sockaddr_in *client_address ){

    *socket_file_descriptor = socket( PF_INET , SOCK_STREAM , 0 );

    /* Se não foi possível criar um socket */
    if( socket < 0 ){
        perror("ERRO ao abrir socket\n");
        exit(1);
    }
    /* memset seta todos os valores de server_address para 0 */
    memset( (void *) server_address , 0 , sizeof(server_address) );
    printf( "Entre com a porta que o servidor deverá escutar: ");
    scanf( "%d" , port_number );
    server_address->sin_family = PF_INET; /* Especifica que será comunicação pela rede */


}
