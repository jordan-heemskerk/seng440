CC = arm-linux-gcc
CFLAGS = -O3 -static 
LDFLAGS =

all: RGBtoYCbCr YCbCrtoRGB MSE

clean: 
	rm -rf *.o bin

RGBtoYCbCr: RGBtoYCbCr.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 

YCbCrtoRGB: YCbCrtoRGB.o 
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 
	
MSE: MSE.o 
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LDFLAGS) 
