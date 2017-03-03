#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


typedef struct{
	int red;
	int green;
	int blue;
}Image;

Image** rotateLeft(Image **array, int x, int y);               

Image** rotateRight(Image **array, int x, int y){               
	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
	for(int i = 0; i<y; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * x);
	}
	array = rotateLeft(array, x, y);
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array2[i][x-j].red = array[i][j].red;
			array2[i][x-j].green = array[i][j].green;
			array2[i][x-j].blue = array[i][j].blue;
		}
	}

	return array2;
}

Image** rotateLeft(Image **array, int x, int y){               
	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
	for(int i = 0; i<y; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * x);
	}

	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array2[j][i].red = array[i][j].red;
			array2[j][i].green = array[i][j].green;
			array2[j][i].blue = array[i][j].blue;
		}
	}

	return array2;
}

void contrast(Image **array, int x, int y, int max, double cp){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			if(array[i][j].red <= (max/2)){     
				array[i][j].red -= (max *cp);
			}
			else{
				array[i][j].red += (max *cp);
			}

			if(array[i][j].green <= (max/2)){     
				array[i][j].green -= (max *cp);
			}
			else{
				array[i][j].green += (max *cp);
			}
			if(array[i][j].blue <= (max/2)){     
				array[i][j].blue -= (max *cp);
			}
			else{
				array[i][j].blue += (max *cp);
			}
		}
	}
}

void invert(Image **array, int x, int y, int max){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].red = max - array[i][j].red;
			array[i][j].green = max - array[i][j].green;
			array[i][j].blue = max - array[i][j].blue;
		}
	}

}

void extractRed(Image **array, int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].green = 0;
			array[i][j].blue = 0;
		}
	}	
}


void extractGreen(Image **array, int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].blue = 0;
			array[i][j].red = 0;
		}
	}	
}

void extractBlue(Image **array, int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].red = 0;
			array[i][j].green = 0;
		}
	}	
}



void printArray(Image **array, int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			printf("%d %d %d ", array[i][j].red, array[i][j].green, array[i][j].blue);
		}
	}
}

void finish(Image **array, char *line1, char *line2, char *line3, int x, int y){
	printf("%s", line1);
	printf("%s", line2);
	printf("%s", line3);
	printArray(array, x, y);
}


int main(int argc, char *argv[]){
	char line1[50];
	char line2[50];
	char line3[50];
	char *line2s;
	fgets(line1, 50, stdin);
	fgets(line2, 50, stdin);	
	line2s = (char*)strdup(line2);
	char *token = strtok(line2, " ");		
	token = strtok(NULL, " ");
	fgets(line3, 50, stdin);
	int max = atoi(line3);	
	
	int x = atoi(line2);
	int y = atoi(token);
	
	long test = ftell(stdin);
	fseek(stdin, 0, SEEK_END);
	long fsize = ftell(stdin);
	rewind(stdin);
	fseek(stdin, test, SEEK_CUR); 
	
	unsigned char *buffer = (unsigned char *)malloc(fsize);	
	int bytes_read = read(0, buffer, fsize);
	
	fprintf(stderr, "bytes read %d\n", bytes_read);

	Image **array = (Image **)malloc(sizeof(Image *) * x);
	for(int i = 0; i<x; i++){
		array[i] = (Image *) malloc(sizeof(Image) * y);
	}

	char *tok = strtok(buffer, " ");        
	int save;
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			save = atoi(tok);
			array[i][j].red = save;
			tok = strtok(NULL, " ");	
			save = atoi(tok);
			array[i][j].green = save;
			tok = strtok(NULL, " ");	
			save = atoi(tok);
			array[i][j].blue = save;
			tok = strtok(NULL, " ");	
		}
	}
	fprintf(stderr, "%d %d %d\n", array[0][0].red, array[0][0].green, array[0][0].blue);
	fprintf(stderr, "%d %d %d\n", array[511][511].red, array[511][511].green, array[511][511].blue);

	array = rotateRight(array, x, y);
	finish(array, line1, line2s, line3, x, y);
	
	return 0;
}

