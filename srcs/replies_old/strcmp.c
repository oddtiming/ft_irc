#include "unistd.h"
#include "stdio.h"



int ft_strcmp(char *s1, char *s2)
{
	int a = 0;

	while(s1[a] == s2[a] && s1[a] && s2[a])
		a++;
	return (s1[a] - s2[a]);

}

int main(int argc, char **argv)
{
	if (ft_strcmp(argv[1], argv[2]) == 0)
		printf("is equal\n");
	else
		printf("is not equal\n");
}