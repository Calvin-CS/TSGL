#include "ImageLoader.h"

#define GL_GLEXT_PROTOTYPES

ImageLoader::~ImageLoader() {
    for (TextureMap::iterator it = loadedTextures.begin(); it != loadedTextures.end(); ++it) {
        glDeleteTextures(1, &(it->second));
    }
}

struct my_error_mgr {
    struct jpeg_error_mgr pub;  // "public" fields

    jmp_buf setjmp_buffer;      // for return to caller
};

void ImageLoader::createGLTextureFromBuffer(GLuint &texture, unsigned char* buffer, unsigned int &width, unsigned int &height,
                               int components) {
    // Generate the OpenGL texture object
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (components == 3)
        components = GL_RGB;
    else if (components == 4)
        components = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, components, GL_UNSIGNED_BYTE, buffer);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint ImageLoader::loadTexture(std::string filename, unsigned int &width, unsigned int &height,
                                GLuint &texture) {
    if (loadedTextures.find(filename) == loadedTextures.end()) {  // Load the image if we haven't already
        std::string extension = filename.substr(filename.find_last_of('.'));
        if (extension == ".png")
            loadedTextures[filename] = loadTextureFromPNG(filename.c_str(), width, height, texture);
        else if (extension == ".jpg" || extension == ".jpeg")
            loadedTextures[filename] = loadTextureFromJPG(filename.c_str(), width, height, texture);
        else if (extension == ".bmp")
            loadedTextures[filename] = loadTextureFromBMP(filename.c_str(), width, height, texture);
        else {
            fprintf(stderr, "File extension not found\n");
            return 0;
        }
    } else {
        texture = loadedTextures[filename];
    }

    return texture;
}

GLuint ImageLoader::loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
                                       GLuint &texture) const {
    // Adapted from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#Loading__BMP_images_yourself

    // Data read from the header of the BMP file
    unsigned char header[54];  // Each BMP file begins by a 54-bytes header
//    unsigned int dataPos;      // Position in the file where the actual data begins
    unsigned int imageSize;    // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
#ifdef _WIN32
    FILE* file;
    fopen_s(&file, filename, "rb");
#else
    FILE* file = fopen(filename, "rb");
#endif

    if (!file) {
        fprintf(stderr, "Can't open %s: no such file\n", filename);
        return 0;
    }

    if (fread(header, 1, 54, file) != 54) {  // If not 54 bytes read : problem
        fprintf(stderr, "%s: not a correct BMP file: header incorrect size\n", filename);
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        fprintf(stderr, "%s: not a correct BMP file: header did not specify BMP type\n", filename);
        fclose(file);
        return 0;
    }

    imageSize = width = height = 0;

    // Get info out of header as 4 byte unsigned ints
    for (int i = 3; i >= 0; i--)
        imageSize = (imageSize << 8) | header[0x22 + i];
    for (int i = 3; i >= 0; i--)
        width = (width << 8) | header[0x12 + i];
    for (int i = 3; i >= 0; i--)
        height = (height << 8) | header[0x16 + i];

    int components = imageSize / width / height;

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 4;  // 4 : one byte for each Red, Green, Blue, and Alpha component

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    if (fread(data, 1, imageSize, file) != imageSize) {  // If not imageSize bytes read : problem
        fprintf(stderr, "%s: file ended unexpectedly\n", filename);
        fclose(file);
        return 0;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);

    char tmp;
    //Reverse the endian-ness of the colors
    if (components == 4) {
        for (unsigned int i = 0; i < imageSize; i += 4) {
            tmp = data[i];
            data[i] = data[i + 3];
            data[i + 3] = tmp;
            tmp = data[i + 1];
            data[i + 1] = data[i + 2];
            data[i + 2] = tmp;
        }
    } else if (components == 3) {
        for (unsigned int i = 0; i < imageSize; i += 3) {
            tmp = data[i];
            data[i] = data[i + 1];
            data[i + 1] = tmp;
        }
    }

    // Flip the image vertically, since BMPs are loaded bottom to top
    for (unsigned int j = 0; j < height - (height / 2); j++) {
        for (unsigned int i = 0; i < components * width; i++) {
            int s1 = components * width * j + i;
            int s2 = components * width * (height - 1 - j) + i;  // This needs to be height *MINUS ONE* minus j
            tmp = data[s1];
            data[s1] = data[s2];
            data[s2] = tmp;
        }
    }

    createGLTextureFromBuffer(texture, data, width, height, components);

    return texture;
}

GLuint ImageLoader::loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
                                       GLuint &texture) const {
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct my_error_mgr jerr;

    // We want to open the input file before doing anything else.
#ifdef _WIN32
    FILE* file;
    fopen_s(&file, filename, "rb");
#else
    FILE* file = fopen(filename, "rb");
#endif

    if (file == NULL) {
        fprintf(stderr, "Can't open %s: no such file\n", filename);
        return 0;
    }

    JSAMPARRAY buffer;        // Sample output row buffer
    unsigned int row_stride;  // Physical row width in output buffer

    /* Set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        fprintf(stderr, "%s: internal error from jpeg\n", filename);
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return 0;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    (void) jpeg_read_header(&cinfo, TRUE);
    /* We passed TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.doc for more info.
     *
     * We don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

    (void) jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* JPEGS are *APPARENTLY* padded to multiples of 4 bytes */
    int extraBytes = cinfo.image_width % 4;
    row_stride = cinfo.output_width * cinfo.output_components + extraBytes;
    /* Make a one-row-high sample array */
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /* Here we use the library's state variable cinfo.output_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     */
    unsigned int x = 0;
    unsigned char* raw_image = NULL;
    raw_image = (unsigned char*) malloc(row_stride * cinfo.output_height);
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could ask for
         * more than one scanline at a time if that's more convenient.
         */
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */
        for (unsigned int i = 0; i < row_stride; i++, x++) {
            raw_image[x] = buffer[0][i];
        }
    }

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    width = cinfo.output_width;
    height = cinfo.output_height;

    createGLTextureFromBuffer(texture, raw_image, width, height, cinfo.num_components);

    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    /* Check if any errors were created */
    if (jerr.pub.num_warnings != 0) {
        fprintf(stderr, "%s: unknown error from jpeg\n", filename);
        return 0;
    }

    return texture;
}

GLuint ImageLoader::loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
                                       GLuint &texture) const {
    png_byte header[8];

#ifdef _WIN32
    FILE* file;
    fopen_s(&file, filename, "rb");
#else
    FILE* file = fopen(filename, "rb");
#endif

    if (file == 0) {
        fprintf(stderr, "Can't open %s: no such file\n", filename);
        return 0;
    }

    // read the header
    if (fread(header, 1, 8, file) != 8) {  // If not 8 bytes read : problem
        fprintf(stderr, "%s: not a  PNG file: header incorrect size\n", filename);
        fclose(file);
        return 0;
    }

    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "%s: not a PNG file: header incorrectly formated\n", filename);
        fclose(file);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "%s: png_create_read_struct returned NULL\n", filename);
        fclose(file);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "%s: png_create_info_struct returned NULL\n", filename);
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(file);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        fprintf(stderr, "%s: png_create_info_struct returned NULL\n", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(file);
        return 0;
    }

#ifdef PNG_SETJMP_SUPPORTED
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "%s: internal error from libpng\n", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(file);
        return 0;
    }
#endif

    // init png reading
    png_init_io(png_ptr, file);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    long unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes - 1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*) malloc((unsigned) (rowbytes * height * sizeof(png_byte) + 15));
    if (image_data == NULL) {
        fprintf(stderr, "%s: could not allocate memory for PNG image data\n", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(file);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_bytep *) malloc((unsigned) (height * sizeof(png_bytep)));
    if (row_pointers == NULL) {
        fprintf(stderr, "%s: could not allocate memory for PNG row pointers\n", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(file);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < height; i++)
        row_pointers[i] = image_data + i * rowbytes;

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    if (color_type == PNG_COLOR_TYPE_RGB)
        color_type = 3;
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        color_type = 4;

    createGLTextureFromBuffer(texture, image_data, width, height, color_type);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(file);
    return texture;
}

void ImageLoader::my_error_exit(j_common_ptr cinfo) {
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_mgr* myerr = (my_error_mgr*) cinfo->err;

    /* Always display the message. */
    (*cinfo->err->output_message)(cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

bool ImageLoader::saveImageToFile(std::string filename, GLubyte *pixels, int w, int h) const {
    std::string extension = filename.substr(filename.find_last_of('.'));
    bool success = false;
    if (extension == ".png")
        success = saveToPNG(filename.c_str(), pixels, w, h);
    else if (extension == ".jpg" || extension == ".jpeg")
        fprintf(stderr, "JPG saving not implemented yet\n");
    else if (extension == ".bmp")
        fprintf(stderr, "BMP saving not implemented yet\n");
    else
        fprintf(stderr, "File extension not found\n");
    return success;
}

bool ImageLoader::saveToPNG(const char* filename, GLubyte *pixels, int w, int h) const {
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fprintf(stderr, "%s: png_create_write_struct returned NULL\n", filename);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fprintf(stderr, "%s: png_create_info_struct returned NULL\n", filename);
        png_destroy_write_struct(&png, &info);
        return false;
    }

#ifdef _WIN32
    FILE* file;
    fopen_s(&file, filename, "wb");
#else
    FILE* file = fopen(filename, "wb");
#endif

    if (!file) {
        fprintf(stderr, "Was unable to create file: %s\n", filename);
        png_destroy_write_struct(&png, &info);
        return false;
    }

    png_init_io(png, file);
    png_set_IHDR(png, info, w, h, 8 /* depth */, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_colorp palette = (png_colorp) png_malloc(png, (unsigned) (PNG_MAX_PALETTE_LENGTH * sizeof(png_color)));
    if (!palette) {
        fprintf(stderr, "%s: was unable to create png color palette\n", filename);
        fclose(file);
        png_destroy_write_struct(&png, &info);
        return false;
    }
    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);

    png_bytepp rows = (png_bytepp) png_malloc(png, (unsigned) (h * sizeof(png_bytep)));
    for (int i = 0; i < h; ++i)
        rows[i] = (png_bytep) (pixels + (h - i - 1) * w * 3);

    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_destroy_write_struct(&png, &info);

    fclose(file);
    delete[] rows;
    return true;
}
