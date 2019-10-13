#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<unistd.h>

char* inputConsole()
{
	char *str;
	size_t bufsiz= 200;

	str = (char* )malloc(sizeof(char)*201);
	getline(&str, &bufsiz, stdin);

	for(int i = 0; i < strlen(str); i++)
	{
		if(str[i] == '\n')
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
	char* command = inputConsole();	
	int count;
	char** cmd_arr = strFactoring(command, &count, "|");
	
	/*for(int i = 0; i < count; i++)
	{
		printf("%s \n", cmd_arr[i]);
	}*/
}
