#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<unistd.h>

char* firstword(char* str)
{
	for(int i = 0; i < strlen(str); i++)
	{
		if(str[i] == ' ')
		{
			str[i] = '\0';
			break;
		}
	}
	return str;
}

char** strFactoring(char* command, int* count, char delim[])
{
	char cmd_cpy[200];
	strcpy(cmd_cpy, command);

	char *ptr = strtok(command, delim);

	*count = 0;
	while(ptr != NULL)
	{
		ptr = strtok(NULL, delim);
		(*count)++;
	}

	char** cmd_arr = (char** )malloc(sizeof(char*)*(*count));


	ptr = strtok(cmd_cpy, delim);
	int i = 0;
	while(ptr != NULL)
	{
		cmd_arr[i] = (char* )malloc(sizeof(char)*strlen(ptr));
		strcpy(cmd_arr[i], ptr);
		printf("%s\n", cmd_arr[i]);
		ptr = strtok(NULL, delim);
		i++;
	}
	return cmd_arr;	

}

int main()
{
	FILE* fp = fopen("file.txt", "w+");
	FILE* temp = fopen("temp.txt", "w+");

	if(fork())
	{
	}
	else
	{
		if(fork())
		{
			while(wait(NULL) != -1);
			//
				fclose(temp);
				temp = fopen("temp.txt", "w+");

				fseek(fp, 0, SEEK_SET);
				char c = fgetc(fp);
				printf("Hello, grep2: \n");
				    while (c != EOF)
				    {
					fputc(c, temp);
					c = fgetc(fp);
					printf("%c", c);
				    }

				    fclose(fp);
				    fp = fopen("file.txt", "w+");
					fseek(temp, 0, SEEK_SET);
				    dup2(fileno(fp), 1);
				    dup2(fileno(temp), 0);

				    strFactoring("")
				execlp("/bin/grep", "grep", "Prob", NULL);

		}
		else
		{
			if(fork())
			{
				while(wait(NULL) != -1);
				fclose(temp);
				temp = fopen("temp.txt", "w+");
				
				fseek(fp, 0, SEEK_SET);
				char c = fgetc(fp);
				    while (c != EOF)
				    {
					fputc(c, temp);
					c = fgetc(fp);
					printf("%c", c);
				    }

				    fclose(fp);
				    fp = fopen("file.txt", "w+");
					fseek(temp, 0, SEEK_SET);
				    dup2(fileno(fp), 1);
				    dup2(fileno(temp), 0);
				execlp("/bin/grep", "grep", "-v", "/", NULL);
			}
			else
			{
				printf("Hello ls\n");
				dup2(fileno(fp), 1);
				execlp("/bin/ls", "ls", "-l", NULL);
			}
		}
	}
}
