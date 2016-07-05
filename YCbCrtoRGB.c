#include <stdio.h>

#include "YCbCrtoRGB.h"

//useful examples are found in /home/frodo/public/ugls_lab/SPL/share/SPL/demo
int commandLineArgsValid(int argc, char** argv){
	//check that the correct number of command line arguments were passed in 
	if(argc != 3){
		printf("Wrong number of command line arguments\n"); 
		return 0;
	}
	/*//check that the second command line argument is a valid input WAV audio file
	std::string wav = ".wav";
	if(!(((std::string)argv[1]).find(wav) != std::string::npos)){
		std::cerr << "Incorrect input filename format. Must end with .wav\n";
		return false;
	}
	//check that the third command line argument is a valid output WAV audio file
	if(!(((std::string)argv[2]).find(wav) != std::string::npos)){
		std::cerr << "Incorrect output filename format. Must end with .wav\n";
		return false;
	}*/
	  
	return 1;
}

int main(int argc, char** argv){

	//call the command line argument checker to validate arguments
	if(!commandLineArgsValid(argc, argv)){
		printf("Command line arguments incorrect. Exiting.\n");
		return 0;
	}
	
	return 1;

}
