#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
	char *s;
	size_t size = 20;

	s = (char* )malloc(sizeof(char)*21);
	size_t character = getline(&s, &size, stdin);

	printf("%s\n",s);
}
