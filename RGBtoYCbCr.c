#include <stdio.h>

#include "RGBtoYCbCr.h"
#include <stdint.h>
//#include <assert.h>


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


// Multiply two Q88 fixed point numbers to half precision
inline uint32_t fpMulQ88(uint32_t a, uint32_t b) {

    // Remove for efficiency
    //assert(a < 0xffffffff);
    //assert(b < 0xffffffff);

    uint32_t res = a * b;

    return res >> 8; // discard LSByte for half precision

}

int main(int argc, char** argv){

	//call the command line argument checker to validate arguments
	if(!commandLineArgsValid(argc, argv)){
		printf("Command line arguments incorrect. Exiting.\n");
		return 0;
	}


    uint32_t src1[SRC_WIDTH];
    uint32_t src2[SRC_WIDTH];        

    uint32_t dest1[SRC_WIDTH/2];
    uint32_t dest2[SRC_WIDTH/2];

    FILE *src_file;
    FILE *dest_file;


    src_file = fopen(argv[1], "rb");

    if (!src_file) {
        printf("Unable to open %s\n", argv[1]);
        return 1;
    }

    dest_file = fopen(argv[2], "wb");

    if (!dest_file) {

        printf("Unable to open %s for writing \n", argv[2]);
        return 1;

    }

    uint32_t r,g,b,y1,y2,y3,y4,cr,cb;
    uint32_t rgba, ycy;

    int i,j;
    for (i = 0; i < SRC_HEIGHT; i+=2) {

        fread(&src1[0], sizeof(uint32_t), SRC_WIDTH, src_file);
        fread(&src2[0], sizeof(uint32_t), SRC_WIDTH, src_file);

        for (j = 0; j < SRC_WIDTH; j+=2) {

            // We represent the arithemtic using Q8.8 = 8 bits integer, 8 bits fractional
            //
            // The formula to convert a pure fractional number x to its Q8.8
            // value, y, is
            //
            //  y = int(x * 2^8 + 0.5)
            //
            //  Converting the pure fractional values in the
            //  conversion matrix yields the following
            //      0.257 = 66
            //      0.504 = 129
            //      0.098 = 25
            //      0.148 = 38
            //      0.291 = 74
            //      0.439 = 112
            //      0.368 = 94
            //      0.071 = 18
            //
            rgba = src1[j];

            r = (uint32_t)(uint8_t)(rgba >> 24);
            g = (uint32_t)(uint8_t)(rgba >> 16);
            b = (uint32_t)(uint8_t)(rgba >> 8);

            // Convert to Q88
            r <<= 8;
            g <<= 8;
            b <<= 8;

            y1 = (16  << 8) + fpMulQ88(66 , r) + fpMulQ88(129, g) + fpMulQ88(25 , b);
            cb = (128 << 8) - fpMulQ88(38 , r) - fpMulQ88(74 , g) + fpMulQ88(112, b);
            cr = (128 << 8) + fpMulQ88(112, r) - fpMulQ88(94 , g) - fpMulQ88(18 , b);

            rgba = src1[j+1];
            
            r = (uint32_t)(uint8_t)(rgba >> 24);
            g = (uint32_t)(uint8_t)(rgba >> 16);
            b = (uint32_t)(uint8_t)(rgba >> 8);

            // Convert to Q88
            r <<= 8;
            g <<= 8;
            b <<= 8;

            y2 = (16  << 8) + fpMulQ88(66 , r) + fpMulQ88(129, g) + fpMulQ88(25 , b);
            cb += (128 << 8) - fpMulQ88(38 , r) - fpMulQ88(74 , g) + fpMulQ88(112, b);
            cr += (128 << 8) + fpMulQ88(112, r) - fpMulQ88(94 , g) - fpMulQ88(18 , b);
            
            rgba = src2[j];
            
            r = (uint32_t)(uint8_t)(rgba >> 24);
            g = (uint32_t)(uint8_t)(rgba >> 16);
            b = (uint32_t)(uint8_t)(rgba >> 8);

            // Convert to Q88
            r <<= 8;
            g <<= 8;
            b <<= 8;

            y3 = (16  << 8) + fpMulQ88(66 , r) + fpMulQ88(129, g) + fpMulQ88(25 , b);
            cb += (128 << 8) - fpMulQ88(38 , r) - fpMulQ88(74 , g) + fpMulQ88(112, b);
            cr += (128 << 8) + fpMulQ88(112, r) - fpMulQ88(94 , g) - fpMulQ88(18 , b);
            
            rgba = src2[j+1];
            
            r = (uint32_t)(uint8_t)(rgba >> 24);
            g = (uint32_t)(uint8_t)(rgba >> 16);
            b = (uint32_t)(uint8_t)(rgba >> 8);

            // Convert to Q88
            r <<= 8;
            g <<= 8;
            b <<= 8;

            y4 = (16  << 8) + fpMulQ88(66 , r) + fpMulQ88(129, g) + fpMulQ88(25 , b);
            cb += (128 << 8) - fpMulQ88(38 , r) - fpMulQ88(74 , g) + fpMulQ88(112, b);
            cr += (128 << 8) + fpMulQ88(112, r) - fpMulQ88(94 , g) - fpMulQ88(18 , b);

            // This average doesn't change under Q88, becuase we just average in fixed point space
            cb /= 4;
            cr /= 4;

            // Convert Q8.8 to back to real life
            y1 >>= 8;
            y2 >>= 8;
            y3 >>= 8;
            y4 >>= 8;
            cb >>= 8;
            cr >>= 8;

            ycy = (uint32_t)(uint8_t)y1;
            ycy |= (uint32_t)((uint8_t)cb << 8);
            ycy |= (uint32_t)((uint8_t)y2 << 16);

            dest1[j/2] = ycy;
 
            ycy = (uint32_t)(uint8_t)y3;
            ycy |= (uint32_t)((uint8_t)cr << 8);
            ycy |= (uint32_t)((uint8_t)y4 << 16);

            dest2[j/2] = ycy;            

        }

        fwrite(&dest1[0], sizeof(uint32_t), SRC_WIDTH/2, dest_file);
        fwrite(&dest2[0], sizeof(uint32_t), SRC_WIDTH/2, dest_file);

    }
    
	
	return 1;

}
