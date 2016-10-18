#Algumas Dicas para implementação

##Passos para comunicação por sockets do SERVIDOR:
    1. Criar socket com socket()
    2. Associar o socket a um endereço com bind(), portanto
    3. Esperar por conexões com listen()
    4. Aceitar uma conexão com accept(), essa chamada é bloqueante

##Passos para comunicação por sockets do CLIENTE:
    1. Criar socket
    2. Conectar o socket ao servidor usando connect()
    3. Enviar e receber dados usando read() e write()


A função socket() cria uma interface de comunicação
    RECEBE três parâmetros:
        family: PF_INET -> Comunicação sobre a internet, usa endereço de rede (mesmo AF_INET)
                PF_UNIX -> Comunicação local na máquina, usa endereço de arquivos
        type: Tipo da comunicação
                SOCK_STREAM -> TCP, 2 caminhos
                SOCK_DGRAM -> UDP
        protocol: Geralmente recebe 0
    RETORNA:
        -> um inteiro, descritor de arquivo do socket
        -> -1, ocorreu falha

    Portas acima de 2000 são geralmente disponíveis

##Sobre a struct sockaddr_in
struct sockaddr_in
{
    short   sin_family; /* must be AF_INET
    u_short sin_port;
    struct  in_addr sin_addr;
    char    sin_zero[8]; /* Not used, must be zero
};
