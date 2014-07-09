/*
 * ImageLoader.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: pretzel
 */

#include "ImageLoader.h"

typedef struct tagBITMAPFILEHEADER
{
unsigned short bfType;  //specifies the file type
unsigned long bfSize;  //specifies the size in bytes of the bitmap file
unsigned short bfReserved1;  //reserved; must be 0
unsigned short bfReserved2;  //reserved; must be 0
unsigned long bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
unsigned long biSize;  //specifies the number of bytes required by the struct
long biWidth;  //specifies width in pixels
long biHeight;  //species height in pixels
unsigned short biPlanes; //specifies the number of color planes, must be 1
unsigned short biBitCount; //specifies the number of bit per pixel
unsigned long biCompression;//spcifies the type of compression
unsigned long biSizeImage;  //size of image in bytes
long biXPelsPerMeter;  //number of pixels per meter in x axis
long biYPelsPerMeter;  //number of pixels per meter in y axis
unsigned long biClrUsed;  //number of colors used by th ebitmap
unsigned long biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

/*
GLuint ImageLoader::loadTexture(std::string filename, int &width, int &height, GLuint &texture) {
	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp) {
	 return false;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
	 fclose(fp);
	 return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	   NULL, NULL);
	if (!png_ptr) {
	 fclose(fp);
	 return (false);
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
	 png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
	 fclose(fp);
	 return (false);
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
	 png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
	 fclose(fp);
	 return (false);
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
	 png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	 fclose(fp);
	 return (false);
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
	   NULL, NULL, NULL);

	//update width and height based on png info
	width = twidth;
	height = theight;

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte* image_data = new png_byte[rowbytes * height];
	if (!image_data) {
	 //clean up memory and close stuff
	 png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	 fclose(fp);
	 return false;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
	 row_pointers[height - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Now generate the OpenGL texture object
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
	glBindTexture(GL_TEXTURE_2D,0);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);

	if (texture)
		std::cout << "loaded " << texture << " successfully" << std::endl << std::flush;
	else
		std::cerr << "loading texture failed" << std::endl << std::flush;

	return texture;
}
*/
GLuint ImageLoader::loadTextureFromPNG(const char* file_name, int &width, int &height, GLuint &texture) {
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

//    std::cout << "Color type = " << color_type << std::endl;
//    std::cout << "Bit depth = " << bit_depth << std::endl;

    width = temp_width;
    height = temp_height;

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_bytep *)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
        row_pointers[i] = image_data + i * rowbytes;

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (color_type == PNG_COLOR_TYPE_RGB)
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}

//GLuint ImageLoader::loadTextureFromBMP(const char* filename, int &width, int &height, GLuint &texture) {
//	FILE *filePtr; //our file pointer
//	BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
//	BITMAPINFOHEADER bitmapInfoHeader; //our bitmap info header
//	unsigned char *bitmapImage;  //store image data
//	int imageIdx=0;  //image index counter
//	unsigned char tempRGB;  //our swap variable
//
//	//open filename in read binary mode
//	filePtr = fopen(filename,"rb");
//	if (filePtr == NULL)
//	return 0;
//
//	//read the bitmap file header
//	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);
//
//	//verify that this is a bmp file by check bitmap id
//	if (bitmapFileHeader.bfType !=0x4D42)
//	{
//	fclose(filePtr);
//	return 0;
//	}
//
//	//read the bitmap info header
//	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);
//
//	//move file point to the begging of bitmap data
//	fseek(filePtr, bitmapFileHeader.bOffBits, SEEK_SET);
//
//	//allocate enough memory for the bitmap image data
//	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
//
//	//verify memory allocation
//	if (!bitmapImage)
//	{
//	free(bitmapImage);
//	fclose(filePtr);
//	return 0;
//	}
//
//	//read in the bitmap image data
//	fread(bitmapImage,bitmapInfoHeader.biSizeImage,1,filePtr);
//
//	//make sure bitmap image data was read
//	if (bitmapImage == NULL)
//	{
//	fclose(filePtr);
//	return 0;
//	}
//
//	//swap the r and b values to get RGB (bitmap is BGR)
//	for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage;imageIdx+=3)
//	{
//		tempRGB = bitmapImage[imageIdx];
//		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
//		bitmapImage[imageIdx + 2] = tempRGB;
//	}
//
////	width = bitmapInfoHeader.biWidth / 1920 / 1080;
////	height = bitmapInfoHeader.biHeight;
//
//	width = 1920;
//	height = 1080;
//
//	std::cout << width << "," << height << std::endl;
//
//	//close file and return bitmap image data
//	fclose(filePtr);
//	// Create one OpenGL texture
//	glGenTextures(1, &texture);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//	// Give the image to OpenGL
//	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapImage);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	return texture;
//}

GLuint ImageLoader::loadTextureFromBMP(const char* file_name, int &width, int &height, GLuint &texture) {
	// From http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#Loading__BMP_images_yourself

	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(file_name,"rb");
	if (!file)	{printf("Image could not be opened\n"); return 0;}

	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if ( header[0]!='B' || header[1]!='M' ){
	    printf("Not a correct BMP file\n");
	    return 0;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

//	std::cout << "Image size: " << imageSize << std::endl;
//	std::cout << "width: " << width << std::endl;
//	std::cout << "height: " << height << std::endl;

	int components = imageSize/width/height;

//	std::cout << "components: " << components << std::endl;

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*4; // 4 : one byte for each Red, Green, Blue, and Alpha component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	char tmp;

	//Reverse the order of the colors
	if (components == 4)
		for (int i = 0; i < imageSize; i += 4) {
			tmp = data[i];
			data[i] = data[i+3];
			data[i+3] = tmp;
			tmp = data[i+1];
			data[i+1] = data[i+2];
			data[i+2] = tmp;
		}
	else if (components== 3)
		for (int i = 0; i < imageSize; i += 3) {
			tmp = data[i];
			data[i] = data[i+1];
			data[i+1] = tmp;
		}

	// Flip the image vertically
	for (int j = 0; j < height-(height/2); j++) {
		for (int i = 0; i < components*width; i++) {
			int s1 = components*width*j+i;
			int s2 = components*width*(height-j)+i;
			tmp = data[s1];
			data[s1] = data[s2];
			data[s2] = tmp;
		}
	}

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Give the image to OpenGL
	if (components == 4)
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (components == 3)
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture;
}


