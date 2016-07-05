###SENG 440 2016 - Color Space Conversion 
by Jordan Heemskerk, Jorin Weatherston

#### Introduction

In this project we will be converting from the RGB color space to the YCbCr color space. Included in this project are three main executables. The first converts from RGB to YCbCr and is where the optimization of code has occured as required in the project. The second and third are unoptimized and convert back to RGB from YCbCr and perform MSE comparisons respectively. By using this testing system we can asses the amount of error that our conversion introduces and improve the conversion by analyzing the type of errors we see. 

#### Execution of Programs
Using the terminal it is possible to use these programs. Commands are broken down by executable.

###### RGBtoYCbCr
This program converts from RGB to YCbCr. Navigate to the folder containing the exe. Input and output files must be in the correct video format, and the file names must reflect that file type. This would be an additional parameter.

```
qemu-arm bin/RGBtoYCbCr <inputRGBFileName> <outputYCbCrFileName>
```

Future versions may include the ability to specify the compression of the file to 4 : x : y, where x and y specify the chromatic sampling rates.

###### YCbCrtoRGB
This program converts from YCbCr to RGB. Navigate to the folder containing the exe. Input and output files must be in the correct video format, and the file names must reflect that file type.

```
qemu-arm bin/RGBtoYCbCr <inputYCbCrFileName> <outputRGBFileName>
```

Future versions may include the ability to specify the compression of the file to 4 : x : y, where x and y specify the chromatic sampling rates. This would be an additional parameter.

###### MSE
This program performs Mean Squared Error (MSE) comparisons between the input and output RGB video file content. The larger this error is the lower quality the conversion to YCbCr

```
qemu-arm bin/MSE <inputRGBFileName> <outputRGBFileName>
```

The results of this program will be written out to the console. 


#### Process of Optimization
In optimizing this algorithm we used the following optimizations and analysis tools... 

#### MSE and Conversion Results 
Once we finished optimizing our software we collected the following results... 

#### Hardware Version
We now consider a method of implementing this algorithm in hardware...

#### Conclusion
In summary we have seen that...
