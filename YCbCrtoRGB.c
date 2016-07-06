#include <stdio.h>
#include <stdint.h>

#include "YCbCrtoRGB.h"

#define DEST_WIDTH 640
#define DEST_HEIGHT 480

//useful examples are found in /home/frodo/public/ugls_lab/SPL/share/SPL/demo
int commandLineArgsValid(int argc, char** argv){
	//check that the correct number of command line arguments were passed in 
	if(argc != 3){
		printf("ERROR:Wrong number of command line arguments\n"); 
		return 0;
	}
	  
	return 1;
}

int main(int argc, char** argv){

	//call the command line argument checker to validate arguments
	if(!commandLineArgsValid(argc, argv)){
		printf("Command line arguments incorrect. Exiting.\n");
		return 1;
	}
	
    FILE *src_file, *dest_file;

    src_file = fopen(argv[1], "rb");

    if (!src_file) {

        printf("Unable to open %s for reading\n", argv[1]);
        return 1;

    }

    dest_file = fopen(argv[2], "wb");
    
    if (!dest_file) {

        printf("Unable to open %s for writing\n", argv[2]);

    }

    uint32_t src1[DEST_WIDTH/2];
    uint32_t src2[DEST_WIDTH/2];

    uint32_t dest1[DEST_WIDTH];
    uint32_t dest2[DEST_WIDTH];


    uint32_t ycy1, ycy2, rgba;
    float y1,y2,y3,y4,cr,cb,r,g,b;

    int i,j;
    for (i = 0; i < DEST_HEIGHT; i+=2) {

        fread(&src1[0], sizeof(uint32_t), DEST_WIDTH/2, src_file);
        fread(&src2[0], sizeof(uint32_t), DEST_WIDTH/2, src_file);

        for (j = 0; j < DEST_WIDTH; j+=2) {

            ycy1 = src1[j];

            // apply scaling here too
            y1 = (float)(uint8_t)ycy1 - 16.0;
            cb = (float)(uint8_t)(ycy1 >> 8) - 128.0;
            y2 = (float)(uint8_t)(ycy1 >> 16) - 16.0;

            ycy2 = src2[j];

            // also apply scaling here too            
            y3 = (float)(uint8_t)ycy2 - 16.0;
            cr = (float)(uint8_t)(ycy2 >> 8) - 128.0;
            y4 = (float)(uint8_t)(ycy2 >> 16) - 16.0;

            // get first rgba
            r = 1.164*y1 + 0.000*cb + 1.596*cr;
            g = 1.164*y1 - 0.392*cb - 0.813*cr;
            b = 1.164*y1 + 2.017*cb + 0.000*cr;

            rgba = ((uint8_t)r << 24) | ((uint8_t)g << 16) | ((uint8_t)b << 8);

            dest1[j] = rgba;

            // get second rgba
            r = 1.164*y2 + 0.000*cb + 1.596*cr;
            g = 1.164*y2 - 0.392*cb - 0.813*cr;
            b = 1.164*y2 + 2.017*cb + 0.000*cr;

            rgba = ((uint8_t)r << 24) | ((uint8_t)g << 16) | ((uint8_t)b << 8);

            dest1[j+1] = rgba;
           
            // get third rgba
            r = 1.164*y3 + 0.000*cb + 1.596*cr;
            g = 1.164*y3 - 0.392*cb - 0.813*cr;
            b = 1.164*y3 + 2.017*cb + 0.000*cr;

            rgba = ((uint8_t)r << 24) | ((uint8_t)g << 16) | ((uint8_t)b << 8);

            dest2[j] = rgba;

            // get fourth rgba
            r = 1.164*y4 + 0.000*cb + 1.596*cr;
            g = 1.164*y4 - 0.392*cb - 0.813*cr;
            b = 1.164*y4 + 2.017*cb + 0.000*cr;

            rgba = ((uint8_t)r << 24) | ((uint8_t)g << 16) | ((uint8_t)b << 8);

            dest2[j+1] = rgba;

        }

        fwrite(&dest1[0], sizeof(uint32_t), DEST_WIDTH, dest_file);
        fwrite(&dest2[0], sizeof(uint32_t), DEST_WIDTH, dest_file);

    }
    
    fclose(src_file);
    fclose(dest_file);

	return 0;

}
