#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]){

	char *origem = argv[1]; //nome arq origem
	char *destino = argv[2]; // Recebe o nome do arq destino
      // ssize_t read(int fd, void *buf, size_t count);
	struct stat fileStatOrigem;

	if(open(destino, O_RDONLY) > 0)
	{
		printf("Arquivo de destino ja existe\n");

	}
	else
	{


	    if(stat(origem,&fileStatOrigem) < 0)
			printf("Arquivo de origem n existe\n");
		else
		{
			int fd = open(origem, O_RDONLY);
			char buffer[(int)fileStatOrigem.st_blksize];
			//printf("%d - fd \n%ld - BlockSize\n%ld - st_size \n", fd, fileStatOrigem.st_blksize, fileStatOrigem.st_size);
			ssize_t x = read(fd, (void *)buffer, fileStatOrigem.st_size);
			if(x < 0)
			{
				printf("Um erro na leitura do arquivo de origem foi encontrado:\n");
				printf("#%d - %s\n", errno, strerror(errno));

			}
			fd = creat(destino, fileStatOrigem.st_mode);
			ssize_t y  = write(fd, buffer, x);
			if(x == y)
				printf("Copia concluida\n");

		}


	}


	return 0;
}
