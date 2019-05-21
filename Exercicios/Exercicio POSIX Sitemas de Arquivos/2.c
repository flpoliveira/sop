#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>


int main(int argc, char *argv[]){

	char *nomeDiretorio = argv[1]; //nome arq origem
	DIR * diretorio;

	if(nomeDiretorio == NULL)
	{
		char cwd[PATH_MAX];
		getcwd(cwd, sizeof(cwd));
		diretorio = opendir(cwd);
	}
	else
		diretorio = opendir(nomeDiretorio);
	if(diretorio == NULL)
	{
		if(errno == 2)
			printf("Diretorio inexistente ou não encontrado.\n");
		else
		{
			printf("Um erro foi encontrado:\n");
			printf("#%d - %s\n", errno, strerror(errno));
		}


		return 0;
	}
	struct dirent * estruturaDiretorio;
	printf("tipo    |tipo de arquivo                       |Nome Arquivo\n");
	printf("------------------------------------------------------------\n");
	while(estruturaDiretorio = readdir(diretorio))
	{
		char tipo[40];
		switch(estruturaDiretorio->d_type)
		{
			case DT_BLK:
				strcpy(tipo, "This is a block device.");
				break;
			case DT_CHR:
				strcpy(tipo, "This is a character device.");
				break;
			case DT_DIR:
				strcpy(tipo, "This is a directory.");
				break;
			case DT_FIFO:
				strcpy(tipo, "This is a named pipe (FIFO).");
				break;
			case DT_LNK:
				strcpy(tipo, "This is a symbolic link.");
				break;
			case DT_REG:
				strcpy(tipo, "This is a regular file.");
				break;
			case DT_SOCK:
				strcpy(tipo, "This is a UNIX domain socket.");
				break;
			default:
				strcpy(tipo, "The file type could not be determined.");
				break;
		}
		printf("%7u |%38s|%s\n", estruturaDiretorio->d_type,tipo,estruturaDiretorio->d_name);
	}
	closedir(diretorio);

	return 0;
}
