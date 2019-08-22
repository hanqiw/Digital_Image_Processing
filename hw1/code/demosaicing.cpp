//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 1/22/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	//int Size = 256;3
	int Row;
	int Column;

	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Row = 300] [Column = 390]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
//			Size = atoi(argv[4]);
			//BytesPerPixel = atoi(argv[3]);
			Row = atoi(argv[4]);
			Column = atoi(argv[5]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Row][Column][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);
	cout << "Image read succeed!" <<endl;

	cout << "BytesPerPixel: " << BytesPerPixel <<endl;
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//1. Expand the image matrix.

	cout << "1. Expand the image matrix." <<endl;
	int N = 3; //Window size
	int i = 0;
	int j = 0;
	int k = 0;

	unsigned char Imagedata_Expassion[Row + N - 1][Column + N - 1][BytesPerPixel];

	//fill inside

	for (k = 0; k < BytesPerPixel; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata[i - (N - 1)/2][j - (N - 1)/2][k];
			}
		}
	}
	cout << "Fill inside succeed!" <<endl;

	//fill the expended column.

	for (k = 0; k < BytesPerPixel; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = 0; j < (N - 1)/2 ; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 1 - j][k];

			}
			for (j = (N - 1)/2 + Column; j < N - 1 + Column; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 3 + 2 * Column - j][k];
			}
		}
	}
	cout << "Fill the expended column succeed!" <<endl;
	//fill the expeneded row.

	for(k = 0; k < BytesPerPixel; k++){

		for (i = 0; i < (N - 1)/2 ; i++){
			for(j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 1 -i][j][k];
			}
		}

		for (i = (N - 1)/2 + Row; i < N - 1 + Row; i++){
			for(j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 3 + 2 * Row - i][j][k];
			}
		}
	}


	cout << "Fill the expended row succeed!" <<endl;
	
	//2. Do demosaicing.

	cout << "2. Do demosaicing" <<endl;
	int R_channel = 0;
	int G_channel = 1;
	int B_channel = 2;
	unsigned char Imagedata_output[Row][Column][3];

	for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
		for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

			if (i % 2  == 1 && j % 2 == 1){ //Green1 pixels

				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][R_channel] = 0.5 * (Imagedata_Expassion[i][j - 1][0] + Imagedata_Expassion[i][j - 1][0]);
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][G_channel] = Imagedata_Expassion[i][j][0];
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][B_channel] = 0.5 * (Imagedata_Expassion[i - 1][j][0] + Imagedata_Expassion[i + 1][j][0]);

			}else if(i % 2 == 0 && j % 2 == 1){ //Blue pixels

				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][R_channel] = 0.25 * (Imagedata_Expassion[i - 1][j - 1][0] + Imagedata_Expassion[i + 1][j - 1][0] + Imagedata_Expassion[i - 1][j + 1][0] + Imagedata_Expassion[i + 1][j + 1][0]);
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][G_channel] = 0.25 * (Imagedata_Expassion[i - 1][j][0] + Imagedata_Expassion[i + 1][j][0] + Imagedata_Expassion[i][j - 1][0] + Imagedata_Expassion[i][j + 1][0]);
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][B_channel] = Imagedata_Expassion[i][j][0];

			}else if(i % 2 == 1 && j % 2 == 0 ){ //Red pixels

				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][R_channel] = Imagedata_Expassion[i][j][0];
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][G_channel] = 0.25 * (Imagedata_Expassion[i - 1][j][0] + Imagedata_Expassion[i + 1][j][0] + Imagedata_Expassion[i][j - 1][0] + Imagedata_Expassion[i][j + 1][0]);
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][B_channel] = 0.25 * (Imagedata_Expassion[i - 1][j - 1][0] + Imagedata_Expassion[i + 1][j - 1][0] + Imagedata_Expassion[i - 1][j + 1][0] + Imagedata_Expassion[i + 1][j + 1][0]);

			}else { //Green2 pixels

				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][R_channel] = 0.5 * (Imagedata_Expassion[i - 1][j][0] + Imagedata_Expassion[i + 1][j][0]);
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][G_channel] = Imagedata_Expassion[i][j][0];
				Imagedata_output[i - (N - 1)/2][j - (N - 1)/2][B_channel] = 0.5 * (Imagedata_Expassion[i][j - 1][0] + Imagedata_Expassion[i][j - 1][0]);

			}
		}
	}

	cout << "demosaicing succeed!" <<endl;


	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_output, sizeof(unsigned char), Row*Column*3, file);
	fclose(file);

	return 0;
}