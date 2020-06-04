#ifndef GETBMP_H
#define GETBMP_H

struct imageFile
{
	int width;
	int height;
	unsigned char *data;
};

imageFile *getBMP(std::string fileName);

#endif
