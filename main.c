#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef struct{
	int red;
	int green;
	int blue;
}Image;

Image **array;
int x;
int y;
int number;

Image** rotateLeft(int x, int y);               

Image** rotateRight(int x, int y){               
	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
	for(int i = 0; i<y; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * x);
	}
	array = rotateLeft(x, y);
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array2[i][x-j].red = array[i][j].red;
			array2[i][x-j].green = array[i][j].green;
			array2[i][x-j].blue = array[i][j].blue;
		}
	}

	return array2;
}

Image** rotateLeft(int x, int y){               
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

void contrast(int x, int y, int max, float cp){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			if(array[i][j].red <= (max/2)){     
				array[i][j].red -= (max * cp);
				if(array[i][j].red < 0){   
					array[i][j].red = 0;
				}
			}
			else{
				array[i][j].red += (max * cp);
				if(array[i][j].red > 255){   
					array[i][j].red = 255;
				}
			}

			if(array[i][j].green <= (max/2)){     
				array[i][j].green -= (max * cp);
				if(array[i][j].green < 0){   
                                	array[i][j].green = 0;
                                }
			}
			else{
				array[i][j].green += (max * cp);
				if(array[i][j].green > 255){   
                        		array[i][j].green = 255;
                        	}
			}
			if(array[i][j].blue <= (max/2)){     
				array[i][j].blue -= (max * cp);
				if(array[i][j].blue < 0){   	
                        		array[i][j].blue = 0;
                        	}
			}
			else{
				array[i][j].blue += (max * cp);
				if(array[i][j].blue > 255){   
                                	array[i][j].blue = 255;
                                }
			}
		}
	}
}

void invert(int x, int y, int max){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].red = max - array[i][j].red;
			array[i][j].green = max - array[i][j].green;
			array[i][j].blue = max - array[i][j].blue;
		}
	}

}

void *extractRed(void *num){
	int N = (int) num;
	int temp = x/number;
	int temp2 = y/number;
	int startx = temp * (N-1);
	int endx = temp * (N);
	int starty = temp2 * (N-1);
	int endy = temp2 * (N);
	fprintf(stderr, "%d %d %d %d\n", N, number, temp, temp2);
	for(int i = startx; i<endx; i++){
		for(int j = starty; j<endy; j++){
			array[i][j].green = 0;
			array[i][j].blue = 0;
		}
	}	
	return NULL;
}


void extractGreen(int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].blue = 0;
			array[i][j].red = 0;
		}
	}	
}

void extractBlue(int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array[i][j].red = 0;
			array[i][j].green = 0;
		}
	}	
}



void printArray(int x, int y){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			printf("%d %d %d ", array[i][j].red, array[i][j].green, array[i][j].blue);
		}
	}
}

void finish(char *line1, char *line2, char *line3, int x, int y){
	printf("%s", line1);
	printf("%s", line2);
	printf("%s", line3);
	printArray(x, y);
}


int main(int argc, char *argv[]){
	if(argc != 3 && argc != 4){                                                   
        	fprintf(stderr, "Wrong # of arguments given\n");
        	fprintf(stderr, "Correct Usage: a.out num_threads option [arg]\n");
        	exit(EXIT_FAILURE);
        }
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
	
	x = atoi(line2);
	y = atoi(token);

	long test = ftell(stdin);
	fseek(stdin, 0, SEEK_END);
	long fsize = ftell(stdin);
	rewind(stdin);
	fseek(stdin, test, SEEK_CUR); 
	
	unsigned char *buffer = (unsigned char *)malloc(fsize);	
	int bytes_read = read(0, buffer, fsize);
	
	fprintf(stderr, "bytes read %d\n", bytes_read);

	array = (Image **)malloc(sizeof(Image *) * x);
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

	if(argc == 3){
		number = atoi(argv[1]);
		char *option = argv[2];
		pthread_t *threads;
		threads = (pthread_t *) malloc(sizeof(pthread_t) * number);
		int pass[2];
		pass[0] = x;
		pass[1] = y;

		if(strcmp("-I", option) == 0){
			invert(x, y, max);
		}	
		if(strcmp("-red", option) == 0){
			int really = 0;
			for(int i = 0; i<number; i++){
				really++;
				pthread_create(&threads[i], NULL, extractRed, (void *) really);
			}

//			extractRed(x, y);
		}
		if(strcmp("-green", option) == 0){
			extractGreen(x, y);
		}
		if(strcmp("-blue", option) == 0){
			extractBlue(x, y);
		}
		if(strcmp("-R", option) == 0){
			array = rotateRight(x, y);
		}
		if(strcmp("-L", option) == 0){
			array = rotateLeft(x, y);	
		}
		finish(line1, line2s, line3, x, y);
	}
	if(argc == 4){
		int number = atoi(argv[1]);
		char *option = argv[2];
		float foo = atof(argv[3]);
		fprintf(stderr, "here and foo is: %f\n", foo);
		contrast(x, y, max, foo);
		finish(line1, line2s, line3, x, y);
	}

	
	return 0;
}

