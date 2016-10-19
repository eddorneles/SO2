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
        struct sockaddr_in *server_address );
void setUpNetworkAddress( struct sockaddr_in *address , int port_number );
void stablishConnection( int socket_fd , int port_number ,
    struct sockaddr_in *server_address, struct sockaddr_in *client_address );
void error( char * msg );

int main( int argc , char **argv ){
    int socket_server_id = -1; /* Será o descritor que identifica o socket do servidor */
    int msg_length,
        port_number = 0, /* armazena o número que o servidor escutar */
        client_addr_len, /* tamanho do endereço do cliente */
        nbytes_read; /* número de bytes lidos ou escritos */
    char buffer[BUFFER_LENGTH];
    /* struct sockaddr_in contém endereço de rede */
    struct sockaddr_in server_address , client_address;

    startingExecution( &socket_server_id , &port_number, &server_address );
    stablishConnection( socket_server_id , port_number, &server_address, &client_address);
    /* Comentários temporários para teste
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

void startingExecution( int *socket_fd , int *port_number,
        struct sockaddr_in *server_address ){

    *socket_fd = socket( PF_INET , SOCK_STREAM , 0 );

    /* Se não foi possível criar um socket */
    if( socket < 0 ){
        perror("ERRO ao abrir socket\n");
        exit(1);
    }
    /* memset seta todos os valores de server_address para 0 */
    memset( server_address , 0 , sizeof(server_address) );
    printf( "Entre com a porta que o servidor deverá escutar: ");
    scanf( "%d" , port_number );
    setUpNetworkAddress( server_address , *port_number );
    /* bind() associa o endereço (server_address) ao socket, também chamado de
        atribuição de nomeação ao socket, o sizeof deve ser com o VALOR de
        server_address, pois sizeof( sever_address ) é o tamanho de um endereço de memória */
    if( bind( *socket_fd , (struct sockaddr *) server_address, sizeof( *server_address ) ) < 0 ){
        error( "ERRO ao atribuir nome ao socket" );
    }
    /* se tudo ocorrer como esperado,  */
    return;
}

/******************************************************************************
settingUpNetworkAddress()
Configura a struct responsável pelo endereço de rede(struct sockaddr_in)
para que seja utilizado corretamente nas system calls de socket
RETORNA
    ->address por REFERÊNCIA, de forma que esteja com todos os campos com os
            valores para o bind()
*******************************************************************************/
void setUpNetworkAddress( struct sockaddr_in *address , int port_number ){
    if( address != NULL ){
        address->sin_family = PF_INET;
         /* htons = host to newtork o número da porta deve ser convertido para
         ordem de bytes de rede, LITTLE ENDIAN (padrão Intel) */
        address->sin_port = htons(port_number);
        /* Contém endereço IP do host, no caso do servidor é o IP dele mesmo
            nesse caso, INADDR_ANY retorna o IP local */
        address->sin_addr.s_addr = INADDR_ANY;
    }
    return;
}

void stablishConnection( int socket_fd , int port_number ,
        struct sockaddr_in *server_address, struct sockaddr_in *client_address ){
    int a;

    if( socket_fd > 0 && server_address != NULL && client_address != NULL ){
        /* listen faz o processo aguardar comunicações no socket_fd, e o
        segundo parâmetro é a quantidade de conexões que o processo pode aguardar */
        printf("Chegou ao listen\n" );
        listen( socket_fd , 5 );
        printf("Passou do listen\n" );
    }

}

/******************************************************************************
error()
Recebe uma mensagem de error imprime ela e encerra a execução do programa
******************************************************************************/
void error( char * msg ){
    perror( msg );
    exit(1);
}
