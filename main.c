#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */ 
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#define BUFSIZE 128
 

ssize_t readln(int fildes, void *buf, size_t nbyte){
	int rd;
	int lidas=0;
	char *p=buf;
	while ( (rd=read(fildes,p,1))>=0 && *p!='\n' ) {
		if (nbyte<lidas) return lidas;
		if (rd==0) return lidas;
		if (rd<0) return -1;
		lidas++;
		p++;
	}	
	return lidas; 
}

typedef struct comando {
    char *args[BUFSIZE]; 
    int numargs; //numero de args;
    int compipe;  // se compipe==0 tem '$|' senão é '$'
};

struct comando cmds[BUFSIZE];
/*
typedef struct file {
    char *conteudo;
}*ficheiro;






int parse_output(char *cmd) {
    
    int i = 0;
    
    char buf[BUFSIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        sscanf(buf,"%s", &outputs[i].resultado);
        printf("OUTPUT 1 : %s\n", &(outputs[i].resultado));i++;

    }
    

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return 0;
}



char* apagarDollarIcone (char *buffer) {
    int i = 0 ;
    int j = 0;
    char *resultado=malloc(sizeof(char));
   
   
    for (i=0;buffer[i]!='\0';i++) {

        if (buffer[i]!='$') {
            resultado[j++] = buffer[i];

        }


    }
    return resultado;
}

void lerFicheiro (const char* dump_path) {

    int c;
    FILE *file;
    file = fopen(dump_path, "r");
    char buffer[256];
    char *teste;

    
    while(fgets(buffer, 256, file)) {
         if (buffer[0]=='$' && buffer[1] !='|') {
             printf ("%s",apagarDollarIcone("$ ls"));
             printf ("%d\n",parse_output(apagarDollarIcone("$ ls")));
         }
    
    }
    fclose(file);
     
}

*/

int main () {
    int fd = open("exemplo.nb",O_RDONLY,0666);
    char buffer[256];
    char *buf = buffer;
    int rd;
    int numcomandos=0;
    while ( 1 ) {
        rd = readln(fd,buf,256);
      //  printf("rd=%d",rd);
        if (rd<=0) break;
        buf[rd]='\0';
        if(buf[0]=='$') {
        //           printf("buf=%s\n",buf);
            //struct comando cmd;
           // cmd =malloc(sizeof(char*) + sizeof(int) + sizeof(int) );
            cmds[numcomandos].numargs=0;
            if(rd>1 && buf[1]=='|') {
                cmds[numcomandos].compipe = 0;
                buf= buf + 1; // tira o |

            }
            else{
                cmds[numcomandos].compipe = 1;
            }
            buf= buf + 2; // tira o $ e um espaço
            
            const char s[2] = " ";
            char *token;
            token = strtok(buf, s);
            while( token != NULL ) {
              //  printf( " %s\n", token );
                cmds[numcomandos].args[cmds[numcomandos].numargs] =malloc(sizeof(char*));
                strcpy(cmds[numcomandos].args[cmds[numcomandos].numargs],token);
                cmds[numcomandos].numargs++; 
                token = strtok(NULL, s);
            }
            numcomandos++;
        }
    }
    int i,j;
    for( i=0;i<numcomandos;i++){
        for ( j=0; j<cmds[i].numargs;j++) {
            printf("Comando%d,Argumento%d::::%s\n",i,j,cmds[i].args[j]);
        } 
    }
    return 0;
}