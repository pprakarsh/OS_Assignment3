#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

int arr[100][100];

struct shm_flag
{
	char flag;
};

int spysearch(int i, int j, int n, struct shm_flag* ptr)
{
	if(i == n || j == n)
	{
		exit(0);
	}
	if(arr[i][j] == 2)
	{
		ptr->flag = 'g';
		printf("\nCell: (%d, %d) ", i, j);
		return;
	}
	else if(arr[i][j] == 1)
	{
		exit(0);
	}
	else
	{
		if(fork())
		{
			if(fork())
			{
				while(wait(NULL) != -1);
				exit(0);
			}
			else
			{
				spysearch(i+1, j, n, ptr);
				printf("\nCell: (%d, %d) ", i, j);
				return;
			}
		}
		else
		{
			spysearch(i, j+1, n, ptr);
			printf("\nCell: (%d, %d) ", i, j);
			return;
		}	
	}
}

int main()
{
	int n;
	scanf("%d", &n);
	
	// ftok to generate unique key 
	key_t key = ftok("/home/prakarsh/OS_Assignment3/shm_flag.txt",65); 
	  
	// shmget returns an identifier in shmid 
	int shmid = shmget(key, sizeof(struct shm_flag), 0666|IPC_CREAT); 
	  
	// shmat to attach to shared memory 
	struct shm_flag* ptr = (struct shm_flag* ) shmat(shmid, NULL, 0);

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			scanf("%d", &arr[i][j]);
		}
	}
	ptr->flag = 'r';

	if(fork())
	{
		while(wait(NULL) != -1);	
		if(ptr->flag == 'r')
		{
			printf("\nRescue not possible :(\n");
		}
	}
	else
	{
		spysearch(0, 0, n, ptr);
		printf("\nRescue possible :)");
	}
}
