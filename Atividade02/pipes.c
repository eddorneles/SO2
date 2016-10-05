/*******************************************************************************
Atividade de SO2
Laboratório 1 - Pipes
Integrantes: Eduardo Dorneles e Igor Cavalcanti
*******************************************************************************/
/*
DESCRIÇÃO
Nessa atividade, o processo filho lê entradas da entrada padrão, escreve
em um pipe, até ler -1. O processo pai deverá ler do pipe, no momento em que ler
-1, deverá somar todos os valores entrados e enviar para o processo filho.
Uma vez que o processo filho receber o resultado, os dois processos
deverão encerrar.
*******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX 10

int taskChildProcess( int *fd_child_parent , int *fd_parent_child , pid_t process );
int taskParentProcess( int *fd_child_parent, int *fd_parent_child , pid_t process );

int main(){
    int file_descriptor1_chipar[2], file_descriptor2_parchi[2];
    pid_t parent_process, child_process;

    /* Se o pipe foi criado, então então retorna 0, senão -1. -1 || -1 == TRUE  */
    if( pipe( file_descriptor1_chipar ) || pipe( file_descriptor2_parchi ) ){
        perror( "pipe" );
        exit(-1);
    }
    /* file_descriptor[0]: reading, file_descriptor[1]: writing */
    parent_process = getpid();/* Retorna o PID do processo atual */
    child_process = fork(); /* Retorna 0 para o processo criado, e um valor > 0 para o processo pai */
    /* Deve-se fechar os arquivos que não serão usados,
    nesse caso um pipe será de comunicação para filho-pai e outro para pai-filho */

    if( child_process == 0 ){
        /* Esse é o processo filho criado */
        fputs( "Estamos no processo filho\nO processo filho irá ler da "
            "entrada padrão e vai escrever no pipe.\n\0" , stdout );
            /* O processo filho não precisa ler do pipe1, SOMENTE ESCREVER, então fechar */
        close( file_descriptor1_chipar[0] );
        /* O processo filho não precisa escrever do pipe2, SOMENTE LER, então fechar */
        close( file_descriptor2_parchi[1] );
        taskChildProcess( file_descriptor1_chipar , file_descriptor2_parchi ,getpid() );
        close( file_descriptor1_chipar[1] );
        close( file_descriptor2_parchi[0] );
        exit(1);
    }else if( child_process > 0  ){
        /* Esse é o processo pai */
        fputs( "Estamos no processo pai\n\0" , stdout );
        /* O processo pai não vai escrever no pipe1, SOMENTE LER, portanto fechar */
        close( file_descriptor1_chipar[1]);
        /* O processo pai não precisa ler do pipe2, SOMENTE ESCREVER, então fechar */
        close( file_descriptor2_parchi[0]);
        taskParentProcess( file_descriptor1_chipar , file_descriptor2_parchi ,getpid() );
        close( file_descriptor1_chipar[0] );
        close( file_descriptor2_parchi[1] );

    }else{
        /* Ocorreu algum erro, o processo filho não pôde ser criado */
        perror("fork");
        fputs( "Não foi possível criar um processo filho\n\0" , stdout );
        exit(-1);
    }

    close( file_descriptor2_parchi[1] );
}

int taskChildProcess( int *fd_child_parent , int *fd_parent_child , pid_t process ){
    int n=0 , sum;
    char c;
    char buf[MAX];

    if( fd_child_parent != NULL && fd_parent_child != NULL && process > -1 ){
        /* */
        while( n != -1 ){
            scanf( "%d" , &n );
            c = (char) n;
            write( fd_child_parent[1] , &c , 1 );
        }
        fprintf( stdout, "O processo filho [%d] está encerrando a escrita no pipe\n\0"
            "Agora o processo filho receberá uma resposta do processo pai", process );
        read( fd_parent_child[0] , &sum , MAX );
        fprintf( stdout, "A resposta recebida do processo pai foi: %d\n\0", sum );
        return sum;
    }
    return -1;
}

int taskParentProcess( int *fd_child_parent, int *fd_parent_child , pid_t process ){
    int sum=0;
    int n=0;
    char c;

    if( fd_child_parent != NULL && fd_parent_child != NULL && process > -1 ){
        /*sum = readFromPipe( fd_child_parent , process );*/
        /* A tarefa inicial do processo pai será ler o arquivo de fd_child_parent,
        portanto ele deverá ler fd_child_parent[0] */
        while( n != -1 ){
            sum += n;
            read( fd_child_parent[0] , &c , 1 );
            n = (int) c; /* Cast para normalizar o valor que c leu */
        }
        fprintf( stdout, "O processo pai [%d] está encerrando leitura do pipe\n"
            "e irá enviar para o processo o somatório dos número lidos %d\n\0", process, sum);
        write( fd_parent_child[1] , &sum , MAX );
        return 1;
    }
    return -1;
}
