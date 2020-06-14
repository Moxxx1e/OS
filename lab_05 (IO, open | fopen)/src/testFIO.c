#include <stdio.h>

#define FILE_NAME "file.txt"

int main()
{
	FILE* fd1 = fopen(FILE_NAME, "w");
	FILE* fd2 = fopen(FILE_NAME, "w");
	
	for (char letter = 'a'; letter <= 'z'; letter++)
	{
		if (letter % 2)
			fprintf(fd1, "%c", letter);
		else
			fprintf(fd2, "%c", letter);
	}
	
	fclose(fd1);
	fclose(fd2);

	return 0;
}
