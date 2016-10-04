#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* MARK NAME Eduardo Dorneles Ferreira de Souza */
/* MARK NAME Nome de Outro Integrante Aqui */
/* MARK NAME E Etc */

/****************************************************************
 * Shell xv6 simplificado
 *
 * Este codigo foi adaptado do codigo do UNIX xv6 e do material do
 * curso de sistemas operacionais do MIT (6.828).
 ***************************************************************/

#define MAXARGS 10

/* Todos comandos têm um tipo.  Depois de olhar para o tipo do
 * comando, o código converte um *cmd para o tipo específico de
 * comando. */
struct cmd {
    int type; /* ' ' (exec)
               '|' (pipe)
               '<' or '>' (redirection) */
};

struct execcmd {
    int type;              // ' '
    char *argv[MAXARGS];   // argumentos do comando a ser executado
};

struct redircmd {
    int type;          // < ou >
    struct cmd *cmd;   // o comando a rodar (ex.: um execcmd)
    char *file;        // o arquivo de entrada ou saída
    int mode;          // o modo no qual o arquivo deve ser aberto
    int fd;            // o número de descritor de arquivo que deve ser usado
};

struct pipecmd {
    int type;          // |
    struct cmd *left;  // lado esquerdo do pipe
    struct cmd *right; // lado direito do pipe
};

int fork1(void);  // Fork mas fechar se ocorrer erro.
/* Protótipo da função *parsecmd */
struct cmd *parsecmd(char*); // Processar o linha de comando.

/* Executar comando cmd.  Nunca retorna. */
void
runcmd(struct cmd *cmd)
{
    int p[2], r;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if(cmd == 0)
        exit(0);

    switch( cmd->type ){
    default:
        fprintf( stderr, "tipo de comando desconhecido\n");
        exit(-1);

    case ' ':
        ecmd = (struct execcmd*)cmd;
        if(ecmd->argv[0] == 0)
        exit(0);
        /* MARK START task2
         * TAREFA2: Implemente codigo abaixo para executar
         * comandos simples. */
        fprintf(stderr, "exec nao implementado\n");
        /* MARK END task2 */
    break;

    case '>':
    case '<':
        rcmd = (struct redircmd*)cmd;
        /* MARK START task3
         * TAREFA3: Implemente codigo abaixo para executar
         * comando com redirecionamento. */
        fprintf(stderr, "redir nao implementado\n");
        /* MARK END task3 */
        runcmd(rcmd->cmd);
    break;

    case '|':
        pcmd = (struct pipecmd*)cmd;
        /* MARK START task4
         * TAREFA4: Implemente codigo abaixo para executar
         * comando com pipes. */
        fprintf(stderr, "pipe nao implementado\n");
        /* MARK END task4 */
        break;
    }
    exit(0);
}

int
getcmd(char *buf, int nbuf)
{
    /* fileno retorna o inteiro que identifica o descritor de arquivo*/
    /* e a função isatty é responsável por verificar se o inteiro que identifica
    um descritor de arquivo, referencia a um terminal, portanto o seguinte
    if possui principal responsabilidade de validar as condições para execução do
    shell, e imprimir um "$ " */
    if ( isatty(fileno(stdin)) )
        fprintf(stdout, "$ ");
    /* memset(str, c, n), copia c, em n posições de str
        no caso abaixo, inicializa o buffer com 0 (NULL), ou seja valores NULL, pois
        buf é char [] */
    memset(buf, 0, nbuf);
    /*fgets lê do descritor de arquivo(por isso fgets() ) stdin (entrada padrão) e
    copia para buf */
    fgets( buf, nbuf, stdin );
    /* O if abaixo é satisfeito se um ctrl+D foi pressionado */
    if(buf[0] == 0) // EOF
        return -1;
    return 0;
}

int
main(void)
{
    static char buf[100];
    int r;

    // Ler e rodar comandos.
    while( getcmd(buf, sizeof(buf) ) >= 0){
        /* MARK START task1 */
        /* TAREFA1: O que faz o if abaixo e por que ele é necessário?
         * Insira sua resposta no código e modifique o fprintf abaixo
         * para reportar o erro corretamente. */
         /*MARK RESPOSTA task1:
                O if abaixo verifica se o comando dado como
                entrada é um comando de mudança de diretório.
         */
        if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
            /* Atribui um valor NULL ao último elemento do buffer, pois ele é uma string */
            buf[strlen(buf)-1] = 0;
            /* chrdir() realiza mudança de diretório para o local especificado,
                se não for possível mudar de diretório, retorna -1.
                O if abaixo executa chdir recebendo a terceira posição de buf,
                pois ele pulará o conteúdo de buf "cd ", o restante da string deverá
                ser o nome de um diretório. Se retorno de chdir for -1 um erro é
                printado e retorna-se ao while.
            */
            if( chdir(buf+3) < 0 ){
                fprintf( stderr, "Não foi possível alterar de diretório\n" );
                continue;
            }/* END if */
        }/* END if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ') */
        /* Verifica se o comando entrado é um comando "ls" */

        /*  APAGAR ESSE TRECHO DE CÓDIGO!!!
        if( buf[0] == 'l' && buf[1] == 's' ){
            printf("Li o ls!\n");
        }*/
        /* MARK END task1 */
        if(fork1() == 0)
            runcmd( parsecmd(buf) );
        wait(&r);
    }
    exit(0);
}

int
fork1(void)
{
    int pid;

    /* Tentar realizar a criação de um novo processo */
    pid = fork();
    if( pid == -1 )
        perror("fork");
    return pid;
}

/****************************************************************
 * Funcoes auxiliares para criar estruturas de comando
 ***************************************************************/


/**********************************************************************
execcmd()
Aloca dinamicamente uma struct execcmd*, porém é feito um cast
para retornar uma struct cmd*.
A struct execcmd é responsável por executar comandos.
**********************************************************************/
struct cmd*
execcmd(void)
{
    struct execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = ' ';
    return (struct cmd*)cmd;
}

struct cmd*
redircmd(struct cmd *subcmd, char *file, int type)
{
    struct redircmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = type;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->mode = (type == '<') ?  O_RDONLY : O_WRONLY|O_CREAT|O_TRUNC;
    cmd->fd = (type == '<') ? 0 : 1;
    return (struct cmd*)cmd;
}

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
    struct pipecmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = '|';
    cmd->left = left;
    cmd->right = right;
    return (struct cmd*)cmd;
}

/****************************************************************
 * Processamento da linha de comando
 ***************************************************************/

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>";


/******************************************************************************
gettoken()
RETORNA
    ret: int,
*******************************************************************************/
int
gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s;
    int ret;

    s = *ps;
    /* Enquanto s não é totalmente varrido e há somente whitespace em *s */
    while(s < es && strchr(whitespace, *s))
        s++;
    /* Se q apontará para NULL, não faz nada, quando chamado por parseredirs, q = 0 */
    if(q){
    /* *q apontará para a primeira ocorrência de um caractere diferente de whitespace
        ou NULL diferente do primeiro comando */
        *q = s;
        printf("Valor de s:%c &s:%d\nValor de q:%c &q:%d\n" , *s , s, **q , *q );
    }
    ret = *s;
    switch(*s){
    case 0:
    break;
    case '|':
    case '<':
        s++;
    break;
    case '>':
        s++;
    break;
    default:
        ret = 'a';
        /* Enquanto s não chega ao final da string E não encontro um whitespace
            nem um símbolo, ignora-se outros caracteres */
        while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
          s++;
    break;
    }
    if(eq)
        /* eq apontará para a primeira ocorência de um whitespace, ou NULL */
        *eq = s; /* eq é um ponteiro **, portanto *eq aponta para o endereço s
                    eq é retornado por referência */

    while(s < es && strchr(whitespace, *s))
        s++;
    return ret;
}

/************************************************************************
peek()
Ignora whitespace que possa existir após a entrada de um determinado comando
verifica se o primeiro comando diferente de whitespace é equivalente a algum
elemento de toks.
RECEBE:
    ps: Um ponteiro duplo para o buffer.
    es: Ponteiro que aponta para o último endereço de ps.
    toks: Ponteiro para uma string com tokens.
Quando chamada por parseredirs o terceiro parâmetro é um "<>".
Lembrar que ps é um ponteiro para o buffer (que armazena o comando), por ser **
tem capacidade de alterar para quem ele aponta. A variável es é um ponteiro
para o final do buffer.
RETORNA:
    -TRUE, se ps possui algum elemento semelhante à algum elemento de toks,
    -FALSE, se ps não possui elemento semelhante à toks.
    Sempre alterará o valor para quem *ps aponta(seja para o token, ou
        final de ps).
************************************************************************/
int
peek(char **ps, char *es, char *toks)
{
    char *s;

    s = *ps;
    /* strchr() procura a primeira ocorrência de s em whitespace*/
    /* Enquanto o endereço o qual s aponta for menor que endereço apontado por es
        (lembrando que es aponta para o último endereço que compreende o
        *ps(buffer) ) E houver equivalência entre algum elemento de s com whitespace
        (ver acima quais elementos pertencem à whitespace).
        Ao sair do while o s apontará para NULL ou para alguma caracter diferente
        de whitespace.
     */
    while( s < es && strchr(whitespace, *s))
        s++;
    /* Altera o valor de ps (pra onde ele apontava), ou seja, altera-se para
        quem o buffer apontava inicialmente, agora ele apontará para a primeira
        ocorrência de um caractere diferente de um whitespace, ou apontará para
        o final do buffer */
    *ps = s; /* NÃO ENTENDI PORQUE NÃO ALTERA O VALOR PARA QUEM ps REFERENCIA */
    /* s pode possuir valor NULL ou diferente de whitespace */
    /* quando peek() é chamada por parseredirs() toks tem valor "<>"
        portanto, se o valor para o qual s aponta for igual a algum elemento
        de toks, então ele retornará TRUE, ou seja, o comando se trata de um
        redirecionamento de entra/saída */
    return *s && strchr(toks, *s);
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);

/* Copiar os caracteres no buffer de entrada, começando de s ate es.
 * Colocar terminador zero no final para obter um string valido. */
char
*mkcopy(char *s, char *es)
{
    int n = es - s; /* Subtrai-se os endereços, sobra o número de elemntos */
    char *c = malloc(n+1); /* Aloca-se n elementos + 1 (um elemento para o NULL(terminador de str) ) */
    assert(c); /* Verifica se a alocação foi bem sucedidade, se não retorna em stderr */
    strncpy(c, s, n); /* Copiará n bytes de s em c  */
    c[n] = 0; /* Atribui NULL para o último elemento de c */
    return c; /* retorna o endereço da string alocada dinamicamente */
}

struct cmd*
parsecmd(char *s)
{
    char *es;
    struct cmd *cmd;

    es = s + strlen(s); /* Faz es apontar para o final de s */
    cmd = parseline( &s, es );
    peek(&s, es, "");
    if(s != es){
        fprintf(stderr, "leftovers: %s\n", s);
        exit(-1);
    }
    return cmd;
}

struct cmd*
parseline( char **ps, char *es )
{
    struct cmd *cmd;
    cmd = parsepipe(ps, es);
    return cmd;
}

struct cmd*
parsepipe(char **ps, char *es)
{
    struct cmd *cmd;

    cmd = parseexec(ps, es);
    if( peek(ps, es, "|")){
        gettoken(ps, es, 0, 0);
        cmd = pipecmd(cmd, parsepipe(ps, es));
    }
    return cmd;
}
/**********************************************************************
parseredirs()
Chamada por parseexec()
**********************************************************************/
struct cmd*
parseredirs( struct cmd *cmd, char **ps, char *es)
{
    int tok;
    char *q, *eq;

    while( peek(ps, es, "<>") ){
        tok = gettoken( ps, es, 0, 0 );
        if(gettoken(ps, es, &q, &eq) != 'a') {
            fprintf(stderr, "missing file for redirection\n");
            exit(-1);
        }
        switch(tok){
        case '<':
            cmd = redircmd(cmd, mkcopy(q, eq), '<');
            break;
        case '>':
            cmd = redircmd(cmd, mkcopy(q, eq), '>');
            break;
        }
    }/* END while */
    return cmd;
}


/*******************************************************************
parseexec()
É chamado por parsepipe.
*/
struct cmd*
parseexec(char **ps, char *es)
{
    char *q, *eq;
    int tok, argc;
    struct execcmd *cmd;
    struct cmd *ret;

    ret = execcmd(); /* Retorna uma struct cmd *, porém aloca uma struct execcmd */
    cmd = (struct execcmd*)ret;

    argc = 0;
    ret = parseredirs(ret, ps, es); /* ret (struct cmd*) que havia sido passado como parâmetro */
    /* Enquanto não houver nenhum elemento em *ps que contenha "|" */
    while( !peek( ps, es, "|") ){
        if( (tok=gettoken(ps, es, &q, &eq) ) == 0)
            break;
        if(tok != 'a') {
            fprintf(stderr, "syntax error\n");
            exit(-1);
        }
        cmd->argv[argc] = mkcopy(q, eq); /* retorna o endereço string do comando */
        argc++;
        printf( "Número de elementos em arc:%d\n%s\n%s\n" , argc ,*ps , es );
        if(argc >= MAXARGS) {
            fprintf(stderr, "too many args\n");
            exit(-1);
        }
        ret = parseredirs(ret, ps, es);
    }/* END while */
    cmd->argv[argc] = 0;
    return ret;
}

// vim: expandtab:ts=2:sw=2:sts=2
