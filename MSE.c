#include <stdio.h>
#include <stdint.h>

#include "MSE.h"

#define WIDTH 640
#define HEIGHT 480

//useful examples are found in /home/frodo/public/ugls_lab/SPL/share/SPL/demo
int commandLineArgsValid(int argc, char** argv){
	//check that the correct number of command line arguments were passed in 
	if(argc != 3){
		printf( "ERROR:Wrong number of command line arguments\n"); 
		return 0;
	}
	  
	return 1;
}

int main(int argc, char** argv){

	//call the command line argument checker to validate arguments
	if(!commandLineArgsValid(argc, argv)){
		printf("Command line arguments incorrect. Exiting.\n");
		return 0;
	}

    FILE *src_file, *dest_file;

    uint32_t src[WIDTH];
    uint32_t dest[WIDTH];

    float mse_r, mse_g, mse_b;

    src_file = fopen(argv[1], "rb");
    
    if (!src_file) {

        printf("Unable to open file %s for reading\n", argv[1]);
        return 1;

    }

    dest_file = fopen(argv[2], "rb");
    
    if (!dest_file) {

        printf("Unable to open file %s for writing\n", argv[2]);
        return 1;

    }

    uint32_t src_rgba, dest_rgba;

    int16_t chan1,chan2;

    int i,j;

    for (i = 0; i < HEIGHT; i++) {

        fread(&src[0], sizeof(uint32_t), WIDTH, src_file);
        fread(&dest[0], sizeof(uint32_t), WIDTH, dest_file);

        for (j = 0; j < WIDTH; j++) {

            src_rgba = src[j];
            dest_rgba = dest[j];

            // RED
            chan1 = (int16_t)(uint8_t)(src_rgba >> 24);
            chan2 = (int16_t)(uint8_t)(dest_rgba >> 24);

            mse_r += (chan2 - chan1) * (chan2 - chan1);

            // GREEN
            chan1 = (int16_t)(uint8_t)(src_rgba >> 16);
            chan2 = (int16_t)(uint8_t)(dest_rgba >> 16);

            mse_g += (chan2 - chan1) * (chan2 - chan1);

            // BLUE
            chan1 = (int16_t)(uint8_t)(src_rgba >> 8);
            chan2 = (int16_t)(uint8_t)(dest_rgba >> 8);

            mse_b += (chan2 - chan1) * (chan2 - chan1);

        }
    }


    mse_r /= (WIDTH * HEIGHT);
    mse_g /= (WIDTH * HEIGHT);
    mse_b /= (WIDTH * HEIGHT);
    
 
    printf("MSE Summary:\n");
    printf("\t R: %f\n", mse_r);
    printf("\t G: %f\n", mse_g);
    printf("\t B: %f\n", mse_b);

	
	return 0;
	
}
