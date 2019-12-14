#ifndef SEARCH_FUN
	#define SEARCH_FUN

	struct shm_foundInfo
	{
		char flag;
		char path[200];
		char topdir[200];
		char file[100];
	};

	void changeMarks(char* name, char* marks, struct shm_foundInfo* ptr_shm);
	char recurseDir(char buffer[], struct shm_foundInfo* ptr_shm);
	void SearchFile(char* path, struct shm_foundInfo* ptr_shm);
	void search(char filename[100]);

#endif
