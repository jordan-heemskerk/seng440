CC = arm-linux-gcc
CFLAGS = -g -Wall -O3 
LDFLAGS =

all: RGBtoYCbCr YCbCrtoRGB MSE

clean: 
	rm -rf *.o RGBtoYCbCr YCbCrtoRGB MSE

RGBtoYCbCr: RGBtoYCbCr.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 

YCbCrtoRGB: YCbCrtoRGB.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 
	
MSE: MSE.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 
