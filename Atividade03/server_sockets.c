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
int stablishConnection( int socket_fd , struct sockaddr_in *client_address );
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
    stablishConnection( socket_server_id , &client_address);
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

/*******************************************************************************
startingExecution()
Cria um socket para o servidor e prepara o endereço de rede do servidor
chamando a função setUpNetworkAddress(), ao término da execução o servidor
está apto para a função stablishConnection()

RETORNA
    int *socket_fd (REFERÊNCIA): inteiro que identifica o descritor de arquivo
        do socket do servidor
    int *port_number (REFERÊNCIA): número da porta que o servidor se comunicará
    struct sockaddr_in *server_address(REFERÊNCIA): struct com endereço de rede
        do servidor de configurado para comunicação via sockets
*******************************************************************************/
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


/******************************************************************************
stablishConnection()
Estabelece uma conexão entre um cliente e o servidor. Ao estabelecer a conexão
o socket_fd (socket que escuta no lado servidor), irá direcionar a um novo
socket que de fato manterá a comunicação por mensagens entre o cliente e o
servidor (slave_socket).
RECEBE
    int socket_fd: socket do servidor que escuta novas requisições de
        conexão.
    struct sockaddr_in *client_address: Endereço do cliente que conectará,
        é retornado por referêcia
RETORNA
    int slave_socket: O inteiro que representa o novo socket pelo qual a
        comunicação será mantida
        -1 se ocorreu algum erro no momento do accept()
******************************************************************************/
int stablishConnection( int socket_fd , struct sockaddr_in *client_address ){
    int client_length; /* tamanho da struct de endereço do cliente */
    int slave_socket=-1;

    if( socket_fd > 0 && client_address != NULL ){
        /* listen faz o socket se tornar um socket passivo para aguardar
        comunicações em socket_fd, o segundo parâmetro é a quantidade de
        conexões que o processo pode aguardar */
        listen( socket_fd , 5 );
        client_length = sizeof( *client_address );
        /* A chamada accept bloqueia o processo até que uma comunicação seja
            estabelecida com sucesso, ele retornará o descritor de arquivo
            do novo socket, pois toda a comunicação será feita a partir desse
            novo socket */
        printf( "Aguarda requisição de conexão, para então criar novo socket\n");
        slave_socket = accept( socket_fd , (struct sockaddr *) client_address, &client_length);
        if( slave_socket < 0 ){
            error( "Erro ao aceitar nova comunicação");
        }
    }
    /* retorna o inteiro que representa o descritor do novo socket */
    return slave_socket;

}

/******************************************************************************
error()
Recebe uma mensagem de error imprime ela e encerra a execução do programa
******************************************************************************/
void error( char * msg ){
    perror( msg );
    exit(1);
}
