#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

//struct to hold the 0,1,2 values of the pixel
typedef struct{
	int red;
	int green;
	int blue;
}Image;

//globals for threads
Image **array;
int x;
int y;
int number;
int max;
float cp;

void rotateLeft();               

//rotates image to right
void rotateRight(){               
	rotateLeft(); //calls rotateleft and then just switches it
	Image **array2 = (Image **)malloc(sizeof(Image *) * x);
	for(int i = 0; i<x; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * y);
	}
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			array2[i][y-j].red = array[i][j].red;
			array2[i][y-j].green = array[i][j].green;
			array2[i][y-j].blue = array[i][j].blue;
		}
	}
	array = array2;
}

//rotates image to left
void rotateLeft(){
	Image **array2 = (Image **)malloc(sizeof(Image *) * y);
	for(int i = 0; i<y; i++){
		array2[i] = (Image *) malloc(sizeof(Image) * x);
	}
	
	//fills in values so it is rotated left
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
	array = array2;
}

//adjusts contrast
void *contrast(void *num){
	int N = (int) num; //this identifies what thread we are are at
        int temp = x/number;
        int startx = temp * (N-1);
	int endx = temp * (N);
	if(N == number){
		endx = x; //and then calculates what part it should be doing
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

//inverts the pixels
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

//extracts red, or turns green and blue off
void *extractRed(void *num){
	int N = (int) num;
	int temp = x/number;
	int startx = temp * (N-1);
	int endx = temp * (N);
	if(N == number){
		endx = x;
	}
	for(int i = startx; i<endx; i++){
		for(int j = 0; j<y; j++){
			array[i][j].green = 0;
			array[i][j].blue = 0;
		}
	}	
	return NULL;
}

//extracts green, or turns red and blue off
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

//extracts blue, or turns red and green off
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


//prints out array
void printArray(){
	for(int i = 0; i<x; i++){
		for(int j = 0; j<y; j++){
			printf("%d %d %d ", array[i][j].red, array[i][j].green, array[i][j].blue);
		}
	}
}

//prints out the starting lines like P3 512 512 and 255. then calls print array
void finish(char *line1, char *line3){
	printf("%s", line1);
	printf("%d %d\n", x, y);
	printf("%s", line3);
	printArray();
}


int main(int argc, char *argv[]){
//	fprintf(stderr, "%d\n", argc);
	if(argc != 3 && argc != 4){//Error checks # of arguments                                                   
        	fprintf(stderr, "Wrong # of arguments given\n");
        	fprintf(stderr, "Correct Usage: a.out num_threads option [arg]\n");
        	exit(EXIT_FAILURE);
        }
	char line1[50];
	char line2[50];
	char line3[50];
	if(fgets(line1, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}
	if(fgets(line2, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}	
	char *token = strtok(line2, " ");		
	token = strtok(NULL, " "); //breaks up line 2 of the file. giving height and width
	if(fgets(line3, 50, stdin) == NULL){
		fprintf(stderr, "fgets error\n");
	}

	max = atoi(line3);	//set globals	
	x = atoi(line2);
	y = atoi(token);

	long test = ftell(stdin);
	fseek(stdin, 0, SEEK_END);
	long fsize = ftell(stdin);
	rewind(stdin);
	fseek(stdin, test, SEEK_CUR);//finds EOF to determine size. then resets position 
	
	unsigned char *buffer = (unsigned char *)malloc(fsize);	
//	int bytes_read = read(0, buffer, fsize);
	int bytes_read = fread(buffer, fsize, 1, stdin);
	if(bytes_read == -1){
		fprintf(stderr, "read error\n");
	}	

	array = (Image **)malloc(sizeof(Image *) * x);//sets global 2d array of images
	for(int i = 0; i<x; i++){
		array[i] = (Image *) malloc(sizeof(Image) * y);
	}

	char *tok = strtok(buffer, " ");        
	int save;
	for(int i = 0; i<x; i++){//assigns values to it from the buffer
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


	int check = 1;
	if(argc == 3 || argc == 4){ //goes through all options
		number = atoi(argv[1]);
		char *option = argv[2];
		pthread_t *threads;
		threads = (pthread_t *) malloc(sizeof(pthread_t) * number);
		int really = 0;
		if(strcmp("-I", option) == 0){ //Invert option
			for(int i = 0; i<number; i++){
				really++;
				if(pthread_create(&threads[i], NULL, invert, (void *) really)!=0){
					fprintf(stderr, "pthread create error\n");
				}
			}
		}	
		if(strcmp("-red", option) == 0){//extract red option
			for(int i = 0; i<number; i++){
				really++;
				if(pthread_create(&threads[i], NULL, extractRed, (void *) really) != 0){
					fprintf(stderr, "pthread create error\n");
				}
			}
		}
		if(strcmp("-green", option) == 0){//extract green option
			for(int i = 0; i<number; i++){
                        	really++;
                        	if(pthread_create(&threads[i], NULL, extractGreen, (void *) really) != 0 ){
					fprintf(stderr, "pthread create error\n");
				}
			}
		}
		if(strcmp("-blue", option) == 0){//extract blue option
			for(int i = 0; i<number; i++){
                        	really++;
                        	if(pthread_create(&threads[i], NULL, extractBlue, (void *) really) != 0){
					fprintf(stderr, "pthread create error\n");
				}
			}
		}
		if(strcmp("-R", option) == 0){//rotate right
			rotateRight();
			check = 0;
		}
		if(strcmp("-L", option) == 0){//rotate left
			rotateLeft();
			check = 0;	
		}
		if(strcmp("-C", option) == 0){//contrast
			cp = atof(argv[3]);
			for(int i = 0; i<number; i++){
				really++;
				if(pthread_create(&threads[i], NULL, contrast, (void *) really) != 0){
					fprintf(stderr, "pthread create error\n");
				}
			}
		}
		if(check == 1){ //joins threads
			for(int i = 0; i<number; i++){
				pthread_join(threads[i], NULL);
			}
		}
		finish(line1, line3);//prints to stdout the modified ppm file
	}
	
	if(check == 1){ //frees allocated memory
		for(int i = 0; i<x; i++){
			free(array[i]);
		}
		free(array);
	}

	return 0;
}

