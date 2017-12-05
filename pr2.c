/*
Tyler Pitsch 
TCSE 333 Assignment 2
10/20/17
Tested and compiled on Ubuntu 16.04 LTS Desktop 32-bit
*/

#include <stdio.h>
#include <string.h>

#define HEADER_SIZE 54
#define CONTRAST_RATIO 2.9695

int main(void){

	char file[50];//name of the file we want to use, read as a string from user
	int width;
	int height;

	//get the file name and adds the .bmp file extension
	printf("Enter the name of the file you wish to alter: ");
	scanf(" %s", file);
	strcat(file,".bmp");

	//grabs the first number as the height and the second number as the width, throws thewitespace 		//away
	printf("Enter the height and width of the image: ");
	scanf("%d", &height);
	getchar();
	scanf("%d", &width);

	FILE *infile = fopen(file, "rb");//base image
	
	char header[HEADER_SIZE];
	unsigned char pixels[height][width * 3];//image in a 2D array 
	unsigned char pixelsCopy[height][width * 3];//copy of the original needed to operate on

	
	//read in the header and the image
	fread(header,1,HEADER_SIZE,infile);
	fread(pixels,1,height*width*3,infile);

	//copies the original image into the the other 2D array
	int r,c;
	for(r = 0; r < height; r++){
		memcpy(pixelsCopy[r],pixels[r],sizeof(pixels[r]));
	}


	FILE *outfile = fopen("copy1.bmp","wb");
	/*
	Brigentens the image, checks to see if adding with break the numeric max before adding
	if it goes above 255 with set it to 255, otherwise adds 50 to the color value
	*/
	for(r = 0; r < height; r++){
		for(c = 0;c<width*3;c++){
			if(pixelsCopy[r][c] > 205){
				pixelsCopy[r][c] = 255;
			}else{
				pixelsCopy[r][c] += 50;
			}

		}
	}
	//wirtes the new image to its own file and then closes that file
	fwrite(header,sizeof(char),HEADER_SIZE,outfile);
	fwrite(pixelsCopy,sizeof(char),height*width*3,outfile);
	fclose(outfile);
	//resets the image to the original
	for(r = 0; r < height; r++){
		memcpy(pixelsCopy[r],pixels[r],sizeof(pixels[r]));
	}
	
	FILE *copy2 = fopen("copy2.bmp","wb");
	/*Works on the contrast of the image,makes sure that values will not break 255 or dip
	below 0 before assignment. 
	*/
	int r1,c1;
	for(r1 = 0; r1 < height; r1++){
		for(c1 = 0;c1<width*3;c1++){
			if((CONTRAST_RATIO*(pixels[r1][c1]-128)+128) > 255){
				pixelsCopy[r1][c1] = 255;
			}else if((CONTRAST_RATIO*(pixels[r1][c1]-128)+128) <0) {
				pixelsCopy[r1][c1] = 0;
			}else{
				pixelsCopy[r1][c1] = CONTRAST_RATIO*(pixels[r1][c1]-128)+128;
			}

		}
	}

	//writes and closes the file
	fwrite(header,sizeof(char),HEADER_SIZE,copy2);
	fwrite(pixelsCopy,sizeof(char),height*width*3,copy2);
	fclose(copy2);
	
	//resets the 2D array back to the original image
	for(r = 0; r < height; r++){
		memcpy(pixelsCopy[r],pixels[r],sizeof(pixels[r]));
	}
	
	FILE *copy3 = fopen("copy3.bmp","wb");
	/*rorates the image 180 so it will appear upside down, fills in the other array from the bottom 
	right with the contents of the originals top left, needs to offset so the colors remain in 
	BGR and not get reset to RGB
	*/
	int rr,cc;
	int con = 2;
	for(rr = height-1;rr>=0;rr--){
		for(cc = width*3-1;cc >=0;cc--){
			pixelsCopy[rr][cc] = pixels[height-1-rr][width*3-1-cc+con];
			if(con == -2){
				con = 2;
			}else{
				con -= 2;
			}
		}
	}
	//writes and closes the file
	fwrite(header,sizeof(char),HEADER_SIZE,copy3);
	fwrite(pixelsCopy,sizeof(char),height*width*3,copy3);
	fclose(copy3);

	for(r = 0; r < height; r++){
		memcpy(pixelsCopy[r],pixels[r],sizeof(pixels[r]));
	}
	
	FILE *copy4 = fopen("copy4.bmp","wb");
	//scales the image down per the example given, makes a 1/4 size copy in the bottom left
	int r2,c2;
	int pix = 0;
	for(r2 = 0; r2 < height;r2++){
		for(c2 = 0;c2 < width*3;c2+=3){
			pixelsCopy[r2/2][(pix/2)*3] = pixels[r2][c2];
			pixelsCopy[r2/2][(pix/2)*3+1] = pixels[r2][c2+1];
			pixelsCopy[r2/2][(pix/2)*3+2]= pixels[r2][c2+2];
			pix++;
		}
		pix = 0;
		
	}
		

	//coppies the image into the other corners
	int i,j;
	for(i = 0; i < height/2;i++){
		for(j = 0;j < width*3/2;j++){
			pixelsCopy[i+height/2][j+width*3/2] = pixelsCopy[i][j];
			pixelsCopy[i+height/2][j] = pixelsCopy[i][j];
			pixelsCopy[i][j+width*3/2] = pixelsCopy[i][j];
		}		
	}
	
	
	
	
	
	
	printf("\nCheck the generates files!\n");
	

	//write the image to a file and closes it
	fwrite(header,sizeof(char),HEADER_SIZE,copy4);
	fwrite(pixelsCopy,sizeof(char),height*width*3,copy4);

	fclose(infile);
	fclose(copy4);
}

