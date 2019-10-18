#include <sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct shm_foundInfo
{
	char flag;
	char path[200];
	char topdir[200];
	char file[100];
};

void changeMarks(char* name, char* marks, struct shm_foundInfo* ptr_shm)
{
	printf("\nChanging Marks....\n");
	//change marks
	FILE* fp = fopen(ptr_shm->path, "r+");
	if(fp == NULL)
	{
		printf("Error, opening file\n");
	}
	char str_t[40];
	fscanf(fp, "%s", &str_t);
	while(strcmp(str_t, name) != 0)
	{
		fscanf(fp, "%s", &str_t);	
	}
	char ch;
	fscanf(fp, "%c", &ch);
	fprintf(fp, marks);
	fclose(fp);

}

char recurseDir(char buffer[], struct shm_foundInfor* ptr_shm)
{
	char flag = 'n';
	char delim[] = "\n";
	char *ptr = strtok(buffer, delim);
	char str2[100];
	while(ptr != NULL)
	{
		if(fork())
		{
			//Flag set
			flag = 'p';
			ptr = strtok(NULL, delim);
		}
		else
		{
			flag = 'c';
			char str_cwd[200];
			getcwd(str_cwd, 200);
			strcat(str_cwd, "/");
			strcat(str_cwd, ptr);
			chmod(str_cwd, S_IRUSR | S_IWUSR | S_IXUSR | S_IXUSR | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
			SearchFile(str_cwd, ptr_shm);
			break;
		}
	}
	return flag;
}

void SearchFile(char* path, struct shm_foundInfo* ptr_shm)
{
	chdir(path);
	int pfds2[2];
	pipe(pfds2);
	char flag = 'n';

	if(fork() == 0)
	{
		int pfds1[2];
		pipe(pfds1);
		close(pfds2[0]);
		if(fork() > 0)
		{
			close(pfds2[1]);
			close(pfds1[0]);
			dup2(pfds1[1], 1);
			close(pfds1[1]);
			execlp("/bin/ls", "ls", "-p", 0);
		}
		else 
		{
			close(pfds1[1]);
			if(dup2(pfds1[0], 0) == -1)
			{
				printf("grep input redirect error!\n");
			}
			close(pfds1[0]);
			int st;
			close(pfds2[0]);
			if(st = dup2(pfds2[1], 1) == -1)
			{
				printf("grep ouput redirect error!\n");
			}
			close(pfds2[1]);

			execlp("/bin/grep", "grep", "-w", ptr_shm->file, 0);
		}
	}
	else 
	{
		close(pfds2[1]);
		while(wait(NULL) != -1);
		//We will read from output of (ls -p | grep filename)
		//and we will search for required file and recurse through directories
		char buffer[100];	
		buffer[0] = '\0';
		
		char file_temp[] = "";
		read(pfds2[0], buffer, sizeof(buffer));
		if(buffer[0] != '\0')
		{
			//File found and set flag, path in shared memory
			char str[200];
			getcwd(str, 200);
			strcpy(ptr_shm->path, str); 
			
			char* str_slash = "/"; 
			strcat(ptr_shm->path, str_slash); 
			strcat(ptr_shm->path, ptr_shm->file);
			ptr_shm->flag = 'g';
			exit(0);
		}
		else
		{
			//File not found
			if(ptr_shm->flag == 'g')
			{
				exit(0);
			}
		}

		close(pfds2[0]);

		int pfds3[2];
		pipe(pfds3);
		if(fork() == 0)
		{
			int pfds1[2];
			pipe(pfds1);
			close(pfds3[0]);
			if(fork() > 0)
			{
				close(pfds3[1]);
				close(pfds1[0]);
				dup2(pfds1[1], 1);
				close(pfds1[1]);
				execlp("/bin/ls", "ls", "-p", 0);
			}
			else 
			{
				close(pfds1[1]);
				if(dup2(pfds1[0], 0) == -1)
				{
					printf("grep input redirect error!\n");
				}
				close(pfds1[0]);

				int st;
				if(st = dup2(pfds3[1], 1) == -1)
				{
					printf("grep ouput redirect error!\n");
				}
				close(pfds3[0]);

				execlp("/bin/grep", "grep", "/", 0);
			}
		}
		else
		{
			close(pfds3[1]);
			while(wait(NULL) != -1);
			//We will read from output of (ls -p | grep "/")
			//and we will search for required file and recurse through directories
			char buffer[300];
			for(int i = 0; i < 300; i++)
				buffer[i] = '\0';	

			wait(NULL);
			read(pfds3[0], buffer, 300); 
			close(pfds3[0]);

			//For rercursing through Directories in currend working directory
			char* my_path;
			flag = recurseDir(buffer, ptr_shm);
		}
		if(flag == 'n')
		{
			//Base case, no further directories present
			exit(0);
		}
		else if(flag == 'c')
		{
			//Program should never enter this state
			printf("Should not enter here\n");
			exit(0);
		}
		if(flag == 'p')
		{
			while(wait(NULL) != -1);
			exit(0);
		}	
	}
}

void search(char filename[100])
{
	pid_t pid_main = getpid();
	key_t key = ftok("/home/prakarsh/OS_Assignment2/shm_foundInfo.txt", 's');                                                                                 
	int shm_id = shmget(key, sizeof(struct shm_foundInfo), 0666|IPC_CREAT);                                                                          
	struct shm_foundInfo* ptr_shm= (struct shm_foundInfo* )shmat(shm_id, NULL, 0);
	
	ptr_shm->flag = 'r';
	
	strcpy(ptr_shm->file, filename);
	
	int count = 0;

	char str[100];
	printf("Top working directory: %s\n", getcwd(str, 100));

	strcpy(ptr_shm->topdir, str);

	if(! fork())
	{
		SearchFile(str, ptr_shm);
	}

	while(wait(NULL) != -1);

	if(ptr_shm->flag == 'g')
	{
		//changeMarks("Prakarsh", "24", ptr_shm);
		printf(ANSI_COLOR_CYAN "File found, Your file is in the directory: %s\n" ANSI_COLOR_RESET, ptr_shm->path);
	}
	else
	{
		printf(ANSI_COLOR_RED"No such file found\n"ANSI_COLOR_RESET);
	}
}
