#include <stdio.h>

#include "RGBtoYCbCr.h"
#include <stdint.h>

#define SRC_WIDTH 640
#define SRC_HEIGHT 480



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
#define convert_asm(rgba, y, cb, cr) \
__asm__( \
     /* Unpack rgb values in Q88 in registers r10->r12 */ \
     "mov   r10, %3, lsr #16 \n" /* get r in Q88 */ \
     "and   r10, #0xFF00 \n" \
     "mov   r11, %3, lsr #8 \n" /* get g in Q88*/ \
     "and   r11, #0xFF00 \n" \
     "mov   r12, %3 \n"  /* get b in Q88 */ \
     "and   r12, #0xFF00 \n" \
\
     /* Calculate Y */ \
     "mov   %0, #0x1000 \n"   /* y = (16 << 8) */\
     "mov   r8, #66 \n" \
     "mul   r9, r10, r8 \n"  /* r * 66 */ \
     "add   %0, %0, r9, lsr #8 \n"   /* y += r*66 */ \
     "mov   r8, #129 \n" \
     "mul   r9, r11, r8 \n"  /* g * 129 */ \
     "add   %0, %0, r9, lsr #8 \n"   /* y += g * 129 */ \
     "mov   r8, #25 \n" \
     "mul   r9, r12, r8 \n"  /* b * 25 */ \
     "add   %0, %0, r9, lsr #8 \n"   /* y += b * 25 */  \
\
     /* Calculate Cb */ \
     "add   %1, #0x8000 \n"   /* cb = (128 << 8) */ \
     "mov   r8, #38 \n" \
     "mul   r9, r10, r8 \n"  /* r * 38 */ \
     "sub   %1, r9, lsr #8 \n"       /* cb -= r * 38 */ \
     "mov   r8, #74 \n" \
     "mul   r9, r11, r8 \n"  /* g * 74  */\
     "sub   %1, r9, lsr #8 \n"       /* cb -= g * 74 */ \
     "mov   r8, #112 \n" \
     "mul   r9, r12, r8 \n"  /* b * 112 */ \
     "add   %1, %1, r9, lsr #8 \n"   /* y += b * 112 */ \
\
     /* Calculate Cr */ \
     "add   %2, #0x8000 \n"    /* cr = (128 << 8) */ \
     "mov   r8, #112 \n" \
     "mul   r9, r10, r8 \n"   /* r * 112 */ \
     "add   %2, %2, r9, lsr #8 \n"    /* cr += r * 112 */ \
     "mov   r8, #94 \n" \
     "mul   r9, r11, r8 \n"   /* g * 94 */ \
     "sub   %2, r9, lsr #8 \n"        /* cr -= g * 94 */ \
     "mov   r8, #18 \n" \
     "mul   r9, r12, r8 \n"   /* b * 18 */ \
     "sub   %2, r9, lsr #8 \n"        /* cr -= b * 18 */ \
\
: "=r" (y), "+r" (cb), "+r" (cr) : "r" (rgba) : "r12", "r11", "r10", "r9", "r8")

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

    uint32_t r,g,b,y,cr,cb;
    uint32_t rgba, ycy1, ycy2;

    int i,j;
    for (i = 0; i < SRC_HEIGHT; i+=2) {

        fread(&src1[0], sizeof(uint32_t), SRC_WIDTH, src_file);
        fread(&src2[0], sizeof(uint32_t), SRC_WIDTH, src_file);

        for (j = 0; j < SRC_WIDTH; j+=2) {

            // Read upper left pixel
            rgba = src1[j];

            // we accumulate these in convert_asm
            cb = 0;
            cr = 0;

            convert_asm(rgba, y, cb, cr);

            y >>= 8;
            ycy1 = (uint32_t)(uint8_t)y;

            // Read upper right pixel
            rgba = src1[j+1];
           
            convert_asm(rgba, y, cb, cr);
 
            y >>= 8;
            ycy1 |= (uint32_t)((uint8_t)y << 16);

            // Read lower left pixel
            rgba = src2[j];

            convert_asm(rgba, y, cb, cr);

            y >>= 8;
            ycy2 = (uint32_t)(uint8_t)y;

            // Read lower right pixel
            rgba = src2[j+1];

            convert_asm(rgba, y, cb, cr);            

            y >>= 8;
            ycy2 |= (uint32_t)((uint8_t)y << 16);

            // Average the chrominance samples
            // This average doesn't change under Q88, becuase we just average in fixed point space
            cb /= 4;
            cr /= 4;

            // Convert Q8.8 to back to 0->255 integer
            cb >>= 8;
            cr >>= 8;

            // Pack into output format 4:2:0
            ycy1 |= (uint32_t)((uint8_t)cb << 8);
            ycy2 |= (uint32_t)((uint8_t)cr << 8);

            dest1[j/2] = ycy1;
            dest2[j/2] = ycy2;

        }

        fwrite(&dest1[0], sizeof(uint32_t), SRC_WIDTH/2, dest_file);
        fwrite(&dest2[0], sizeof(uint32_t), SRC_WIDTH/2, dest_file);

    }
    
	
	return 1;

}
