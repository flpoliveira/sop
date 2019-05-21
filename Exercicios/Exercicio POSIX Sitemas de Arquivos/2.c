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
	struct dirent * estruturaDiretorio;
	printf("tipo    |tipo de arquivo    |Nome Arquivo\n");
	printf("-----------------------------------------\n");
	while(estruturaDiretorio = readdir(diretorio))
	{
		printf("%7u |%19d|%s\n", estruturaDiretorio->d_type,50,estruturaDiretorio->d_name);
	}
	closedir(diretorio);

	return 0;
}
