#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<unistd.h>

int main()
{
	FILE* fp = fopen("file.txt", "w+");
	FILE* temp = fopen("temp.txt", "w+");

	if(fork())
	{
		while(wait(NULL) != -1);
		//Read from last cmd in pipe
		    printf("hello main\n");
		fseek(fp, 0, SEEK_SET);
		char c = fgetc(fp); 
		    while (c != EOF) 
		    { 
			printf ("%c", c); 
			c = fgetc(fp); 
		    }
		    fclose(fp);
		    exit(0);
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
