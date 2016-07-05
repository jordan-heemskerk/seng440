CC = arm-linux-gcc
CFLAGS = -g -Wall -O3 
LDFLAGS =

all: RGBtoYCbCr YCbCrtoRGB MSE

clean: 
	rm -rf *.o RGBtoYCbCr YCbCrtoRGB MSE

RGBtoYCbCr: RGBtoYCbCr.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 

YCbCrtoRGB: YCbCrtoRGB.o 
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 
	
MSE: MSE.o 
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 
