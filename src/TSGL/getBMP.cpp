// Routine to read an uncompressed 24-bit unindexed color RGB BMP file into a 
// 32-bit color RGBA image file (alpha values all being set to 1).

#include <fstream>

#include "getBMP.h"

imageFile *getBMP(std::string fileName)
{
	int offset, // No. of bytes to start of image data in input BMP file. 
		w, // Width in pixels of input BMP file.
		h; // Height in pixels of input BMP file.

		   // Initialize imageFile objects.
	imageFile *tempStore = new imageFile; // Temporary storage.
	imageFile *outRGB = new imageFile; // RGB output file.
	imageFile *outRGBA = new imageFile; // RGBA output file.

										// Initialize input stream.
	std::ifstream inFile(fileName.c_str(), std::ios::binary);

	// Get start point of image data in input BMP file.
	inFile.seekg(10);
	inFile.read((char *)&offset, 4);

	// Get image width and height.
	inFile.seekg(18);
	inFile.read((char *)&w, 4);
	inFile.read((char *)&h, 4);

	// Determine the length of padding of the pixel rows 
	// (each pixel row of a BMP file is 4-byte aligned by padding with zero bytes).
	int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;

	// Allocate storage for temporary input file, read in image data from the BMP file, close input stream.
	tempStore->data = new unsigned char[(3 * w + padding) * h];
	inFile.seekg(offset);
	inFile.read((char *)tempStore->data, (3 * w + padding) * h);
	inFile.close();

	// Set image width and height and allocate storage for image in output RGB file.
	outRGB->width = w;
	outRGB->height = h;
	outRGB->data = new unsigned char[3 * w * h];

	// Copy data from temporary input file to output RGB file adjusting for padding and performing BGR to RGB conversion.
	int tempStorePos = 0;
	int outRGBpos = 0;
	for (int j = 0; j < h; j++)
		for (int i = 0; i < 3 * w; i += 3)
		{
			tempStorePos = (3 * w + padding) * j + i;
			outRGBpos = 3 * w * j + i;
			outRGB->data[outRGBpos] = tempStore->data[tempStorePos + 2];
			outRGB->data[outRGBpos + 1] = tempStore->data[tempStorePos + 1];
			outRGB->data[outRGBpos + 2] = tempStore->data[tempStorePos];
		}

	// Set image width and height and allocate storage for image in output RGBA file.
	outRGBA->width = w;
	outRGBA->height = h;
	outRGBA->data = new unsigned char[4 * w * h];

	// Copy image data from output RGB file to output RGBA file, setting all A values to 1.
	for (int j = 0; j < 4 * w * h; j += 4)
	{
		outRGBA->data[j] = outRGB->data[(j / 4) * 3];
		outRGBA->data[j + 1] = outRGB->data[(j / 4) * 3 + 1];
		outRGBA->data[j + 2] = outRGB->data[(j / 4) * 3 + 2];
		outRGBA->data[j + 3] = 0xFF;
	}

	// Release temporary storage and the output RGB file and return the RGBA version.
	delete[] tempStore;
	delete[] outRGB;
	return outRGBA;
}
