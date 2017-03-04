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
//Image **array2;
int x;
int y;
int number;
int max;
float cp;

void rotateLeft();               

void rotateRight(){               
//	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
//	for(int i = 0; i<y; i++){
//		array2[i] = (Image *) malloc(sizeof(Image) * x);
//	}
	rotateLeft();
	rotateLeft();
	rotateLeft();
//	for(int i = 0; i<x; i++){
//		for(int j = 0; j<y; j++){
//			array2[i][x-j].red = array[i][j].red;
//			array2[i][x-j].green = array[i][j].green;
//			array2[i][x-j].blue = array[i][j].blue;
//		}
//	}
//	array = array2;
}

void rotateLeft(){
	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
	for(int i = 0; i<y; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * x);
	}
	fprintf(stderr, "starting %d %d\n", x, y);
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array2[j][i].red = array[i][j].red;
			array2[j][i].green = array[i][j].green;
			array2[j][i].blue = array[i][j].blue;
		}
	}
	int temp = x;
	x = y;
	y = temp;
	fprintf(stderr, "ending %d %d\n", x, y);
	array = array2;
}

void *contrast(void *num){
	int N = (int) num;
        int temp = x/number;
        int startx = temp * (N-1);
	int endx = temp * (N);
	if(N == number){
		startx = x;
	}
   
	for(int i = startx; i<endx; i++){
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
	return NULL;
}

void invert(void *num){
	int N = (int) num;
	int temp = x/number;
	int startx = temp * (N-1);
	int endx = temp * (N);   
	if(N == number){
		endx = x;
	}
	for(int i = startx; i<endx; i++){
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
	int startx = temp * (N-1);
	int endx = temp * (N);
	fprintf(stderr, "%d %d %d\n", N, number, temp);
	if(N == number){
		endx = x;
	}
	fprintf(stderr, "%d %d\n", startx, endx);
	for(int i = startx; i<endx; i++){
		for(int j = 0; j<y; j++){
			array[i][j].green = 0;
			array[i][j].blue = 0;
		}
	}	
	return NULL;
}


void *extractGreen(void *num){
	int N = (int) num;
        int temp = x/number;
        int startx = temp * (N-1);
        int endx = temp * (N);
	if(N == number){
			endx = x;
	}

	for(int i = startx; i<endx; i++){
		for(int j = 0; j<y; j++){
			array[i][j].blue = 0;
			array[i][j].red = 0;
		}
	}
	return NULL;	
}

void *extractBlue(void *num){
	int N = (int) num;
        int temp = x/number;
        int startx = temp * (N-1);
        int endx = temp * (N);
	if(N == number){
		endx = x;
	}
	for(int i = startx; i<endx; i++){
		for(int j = 0; j<y; j++){
			array[i][j].red = 0;
			array[i][j].green = 0;
		}
	}
	return NULL;
}



void printArray(){
	fprintf(stderr, "problems with printarray");
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			printf("%d %d %d ", array[i][j].red, array[i][j].green, array[i][j].blue);
		}
	}
}

void finish(char *line1, char *line3){
	fprintf(stderr, "problems with finish");
	printf("%s", line1);
	printf("%d %d\n", x, y);
	printf("%s", line3);
	printArray();
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
//	char *line2s;
	if(fgets(line1, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}
	if(fgets(line2, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}	
//	line2s = (char*)strdup(line2);
	char *token = strtok(line2, " ");		
	token = strtok(NULL, " ");
	if(fgets(line3, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}
	max = atoi(line3);	
	
	x = atoi(line2);
	y = atoi(token);

	long test = ftell(stdin);
	fseek(stdin, 0, SEEK_END);
	long fsize = ftell(stdin);
	rewind(stdin);
	fseek(stdin, test, SEEK_CUR); 
	
	unsigned char *buffer = (unsigned char *)malloc(fsize);	
	int bytes_read = read(0, buffer, fsize);
	if(bytes_read == -1){
		fprintf(stderr, "read error\n");
	}	
//	fprintf(stderr, "bytes read %d\n", bytes_read);

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
//	fprintf(stderr, "%d %d %d\n", array[0][0].red, array[0][0].green, array[0][0].blue);
//	fprintf(stderr, "%d %d %d\n", array[511][511].red, array[511][511].green, array[511][511].blue);

	if(argc == 3){
		int check = 1;
		number = atoi(argv[1]);
		char *option = argv[2];
		pthread_t *threads;
		threads = (pthread_t *) malloc(sizeof(pthread_t) * number);
		int pass[2];
		pass[0] = x;
		pass[1] = y;
		int really = 0;
		if(strcmp("-I", option) == 0){
			for(int i = 0; i<number; i++){
				really++;
				pthread_create(&threads[i], NULL, invert, (void *) really);
			}
		}	
		if(strcmp("-red", option) == 0){
			for(int i = 0; i<number; i++){
				really++;
				pthread_create(&threads[i], NULL, extractRed, (void *) really);
			}
		}
		if(strcmp("-green", option) == 0){
			for(int i = 0; i<number; i++){
                        	really++;
                        	pthread_create(&threads[i], NULL, extractGreen, (void *) really);
			}
		}
		if(strcmp("-blue", option) == 0){
			for(int i = 0; i<number; i++){
                        	really++;
                        	pthread_create(&threads[i], NULL, extractBlue, (void *) really);
			}
		}
		if(strcmp("-R", option) == 0){
			rotateRight();
			check = 0;
		}
		if(strcmp("-L", option) == 0){
			rotateLeft();
			check = 0;	
		}
		if(argc == 4){
			cp = atof(argv[3]);
			for(int i = 0; i<number; i++){
				really++;
				pthread_create(&threads[i], NULL, contrast, (void *) really);
			}
		}
		if(check == 1){
		fprintf(stderr, "should not be running");
			for(int i = 0; i<number; i++){
				pthread_join(threads[i], NULL);
			}
		}
		finish(line1, line3);
	}
	
	return 0;
}

