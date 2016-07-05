#include <stdio.h>

#include "RGBtoYCbCr.h"
#include <stdint.h>


#define SRC_WIDTH 640
#define SRC_HEIGHT 480

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


    uint32_t src1[SRC_WIDTH];
    uint32_t src2[SRC_WIDTH];        

    FILE *src_file;

    src_file = fopen(argv[1], "rb");

    if (!src_file) {
        printf("Unable to open %s\n", argv[1]);
        return 1;
    }

    float r,g,b,y,cr,cb;
    uint32_t rgba;

    int i,j;
    for (i = 0; i < SRC_HEIGHT; i+=2) {

        fread(&src1[0], sizeof(uint32_t), SRC_WIDTH, src_file);
        fread(&src2[0], sizeof(uint32_t), SRC_WIDTH, src_file);

        for (j = 0; j < SRC_WIDTH; j+=2) {

            rgba = src1[j];
            
            r = (float)(uint8_t)(rgba >> 24);
            g = (float)(uint8_t)(rgba >> 16);
            b = (float)(uint8_t)(rgba >> 8);

            printf("(%f, %f, %f)", r, g, b);

            y = 16.0 + 0.257*r + 0.504*g + 0.098*b;
            cb = 128.0 - 0.148*r - 0.291*g + 0.439*b;
            cr = 128.0 + 0.439*r - 0.368*g - 0.071*b;

            printf(" --> (%f, %f, %f)\n", y, cb, cr);

    
        }

    }
    
	
	return 1;

}
