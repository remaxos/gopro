# gopro
Temporary interview problem from GoPro

# Introduction

The complete problem definition is:
> Write a C program that solves the problem below. The code needs to be of production quality, it should compile, test and run correctly.
> Please also provide any assumptions made in solving the problem and add them as comments inside the code.
> Given a grayscale image from an image sensor in memory with each 16-bit word equating to a pixel value, write a program to find the 50 pixels (including their location) with the highest pixel values.


## 16-bit Greyscale

The 16-bit greyscale offers 65536 levels of grey. TIFF and PNG formats support 16-bit grayscale natively.

## Approach

The first approach to this problem looks pretty straight forward. The complexity is O(n). More precisely, it is O(wh), where w is the width and h is the height of the picture to be processed.
That means it is necessary a single pass through the pixel map. The only issue here is how well this O(n) problem can be optimized. There are two basic improvements here: the structure for the results and, the most important by far, how we can use the SMP architecture of the computer and, ultimately, the GPU, if it is available.

For the structure of the result, the optimum 50 elements structure should be chosen. At the first glance, an ordered list looks very flexible, especially because we should also merge the results from different computing units (GPU/CPU cores). The merge of the local optimums can be done in just 50 steps, exactly as the merge step from the merge sort.  

#### Tools used (etc.)

gvim
TODO (git)
GIMP
OpenCV


# Input Image Formats

## Dummy Format (Didactic Format)

In this chosen dummy format we have chosen the most basic format to describe a fixed 16-bit greyscale image: number of rows, number of columns and the bitmap itself (that is width x height x 2 bytes values). Given that this is a custom format, we should create a generator for it. Also, probably no suitable image viewer can help us visualize it, unless it is converted to a well-known format.

## PNG, BMP, TIFF with OpenCV

I assumed that coding/decoding a well-known image format is not the purpose of this exercise. In order to have a smooth transition to a full-feature interface, I used OpenCV to load and store test images to the memory in order to find the white pixels. 



# References
Rafael C. Gonzalez, Richard E. Woods - Digital Image Processing, (Prentice-Hall).
https://mars.nasa.gov/resources/curiositys-1-8-billion-pixel-panorama/
