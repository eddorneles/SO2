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
#include <arpa/inet.h>

#define ONE_KB 1024
#define ITERATIONS 1000

void startingExecution( int *socket_fd , struct sockaddr_in *server_address , char *buffer , int *msg_length );
void setUpNetworkAddress( struct sockaddr_in *address , int port_number );
int stablishConnection( int socket_fd , struct sockaddr_in *client_address );
void finishingExecution( int server_socket , int slave_socket , char *buffer );
int communicationService( int slave_socket , struct sockaddr_in *client_address, char *buffer , int msg_length );
void error( char * msg );

int main( int argc , char **argv ){
    int socket_server_id = -1; /* Será o descritor que identifica o socket do servidor */
    int msg_length,
        port_number = 0, /* armazena o número que o servidor escutar */
        client_addr_len, /* tamanho do endereço do cliente */
        nbytes_read; /* número de bytes lidos ou escritos */
    char *buffer;
    /* struct sockaddr_in contém endereço de rede */
    struct sockaddr_in server_address , client_address;

    startingExecution( &socket_server_id , &server_address, buffer , &msg_length );

    int slave_socket;


    slave_socket = stablishConnection( socket_server_id , &client_address );
        /* Se foi criado um socket escravo para comunicar com o cliente */
    if( slave_socket > 0 ){
        while(1){
            buffer = (char *)malloc(msg_length * sizeof(char));
            if(buffer != NULL){
                if((nbytes_read = recv(slave_socket, buffer, (msg_length), 0)) == -1){
                    //error("Erro ao receber mensagem!");
                    break;
                }
                buffer[nbytes_read] = '\0';
                printf(buffer);
            }else{
                error("Erro ao alocar buffer!");
            }
            free(buffer);
        }
    }
    if( socket_server_id != -1 ){
        close( socket_server_id );
    }


    printf( "Terminou a execução com sucesso!\n" );
}

/*******************************************************************************
startingExecution()
Cria um socket para o servidor e prepara o endereço de rede do servidor
chamando a função setUpNetworkAddress(), ao término da execução o servidor
está apto para a função stablishConnection()
RECEBE
    char *buffer: Ponteiro para um buffer que será alocado dinamicamente, é
            retornado por REFERÊNCIA
RETORNA
    int *socket_fd (REFERÊNCIA): inteiro que identifica o descritor de arquivo
        do socket do servidor
    int *port_number (REFERÊNCIA): número da porta que o servidor se comunicará
    struct sockaddr_in *server_address(REFERÊNCIA): struct com endereço de rede
        do servidor de configurado para comunicação via sockets
    char *buffer (REFERÊNCIA): endereço do buffer alocado dinamicamente
    int *msg_length (REFERÊNCIA): endereço da variável que armazena o tamanho
            das mensagens (e do buffer).
*******************************************************************************/
void startingExecution( int *socket_fd , struct sockaddr_in *server_address ,
        char *buffer , int *msg_length ){

    int port_number;
    *socket_fd = socket( AF_INET , SOCK_STREAM , 0 );

    /* Se não foi possível criar um socket */
    if( socket < 0 ){
        error("ERRO ao abrir socket");
    }
    /* memset seta todos os valores de server_address para 0 */
    memset( server_address , 0 , sizeof(server_address) );
    printf( "Entre com o tamanho em kbytes de cada mensagem\n" ,
            "Para mensagem de um byte, entre com zero: " );
    scanf( "%d" , msg_length );
    if( *msg_length == 0 ){
        *msg_length = 1;
    }else if( *msg_length > 0 ){
        *msg_length *= ONE_KB;
        if( buffer == NULL ){
            error( "Não foi possível alocar memória para o buffer" );
        }
    }else{
        error( "Tamanho de messagem inválido" );
    }
    buffer = (char *) malloc( sizeof(char) * (*msg_length)  );
    printf( "Entre com a porta que o servidor deverá escutar: ");
    scanf( "%d" , &port_number );
    if( port_number < 1 ){
        error( "Valor de porta inválido" );
    }
    setUpNetworkAddress( server_address , port_number );
    /* bind() associa o endereço (server_address) ao socket, também chamado de
        atribuição de nomeação ao socket, o sizeof deve ser com o VALOR de
        server_address, pois sizeof( sever_address ) é o tamanho de um endereço de memória */
    if( bind( *socket_fd , (struct sockaddr *) server_address, sizeof( *server_address ) ) < 0 ){
        error( "ERRO ao atribuir nome ao socket" );
    }
    /* se tudo ocorrer como esperado,  */
    return;
}

/*******************************************************************************
settingUpNetworkAddress()
Configura a struct responsável pelo endereço de rede(struct sockaddr_in)
para que seja utilizado corretamente nas system calls de socket
RETORNA
    ->address por REFERÊNCIA, de forma que esteja com todos os campos com os
            valores para o bind()
*******************************************************************************/
void setUpNetworkAddress( struct sockaddr_in *address , int port_number ){
    if( address != NULL ){
        address->sin_family = AF_INET;
         /* htons = host to newtork o número da porta deve ser convertido para
         ordem de bytes de rede, LITTLE ENDIAN (padrão Intel) */
        address->sin_port = htons(port_number);
        /* Contém endereço IP do host, no caso do servidor é o IP dele mesmo
            nesse caso, INADDR_ANY retorna o IP local */
        address->sin_addr.s_addr = inet_addr("127.0.0.1");
        printf("Endereço do servidor: %d", address->sin_addr.s_addr );
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
        é retornado por referência
RETORNA
    int slave_socket: O inteiro que representa o novo socket pelo qual a
        comunicação será mantida
        -1 se ocorreu algum erro no momento do accept()
    struct sockaddr_in *client_address(REFERÊNCIA): Endereço do cliente que conectará,
            é retornado por referência
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

int communicationService( int slave_socket , struct sockaddr_in *client_address,
        char *buffer , int msg_length ){
    int cur_iteration = 1;

    /* Validação dos parâmetros da função */
    if( slave_socket > 0 && client_address != NULL && client_address != NULL && buffer != NULL ){
        while( cur_iteration <= ITERATIONS ){
            read( slave_socket , buffer , msg_length );
            /* Deve enviar a mensagem de volta para o cliente */
            write( slave_socket , buffer , msg_length );
            cur_iteration++;
        }
        /* Comunicação concluída com sucesso */
        return 1;
    }
    return 0;
}
/******************************************************************************
finishingExecution()
Fecha descritores de arquivos abertos e desaloca a memória previamente alocada
para o buffer.
RECEBE:
    int server_socket: Descritor de arquivo do socket principal
    int slave_socket: Descritor de arquivo do socket que troca mensagens com o cliente
    char *buffer: Buffer de mensagens alocado dinamicamente na função
            startingExecution()
******************************************************************************/
void finishingExecution( int server_socket , int slave_socket , char *buffer ){

    if( server_socket > 0 ){
        close( server_socket );
    }
    if( slave_socket > 0 ){
        close( server_socket );
    }
    if( buffer != NULL ){
        free( buffer );
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
