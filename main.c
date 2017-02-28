#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char line1[50];
	char line2[50];
	char line3[50];

	fseek(stdin, 0L, SEEK_END);
        int size = ftell(stdin);
	fseek(stdin, 0L, SEEK_SET);

	fgets(line1, 50, stdin);		
	fgets(line2, 50, stdin);		
	fgets(line3, 50, stdin);	
	
	fprintf(stderr, "read: %s\n", line1);
	fprintf(stderr, "read: %s\n", line2);
	fprintf(stderr, "read: %s\n", line3);
	
	char* buffer = malloc(sizeof(char) * size);	
	int bytes_read = read(0, buffer, size);
	fprintf(stderr, "bytes read %d\n", bytes_read);
//	fprintf(stderr, "read: %s\n", buffer);
	return 0;
}
