/*
 * TextLoader.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: mbv26
 */
#include <iostream>
#include "TextLoader.h"

TextLoader::TextLoader() {
    if (FT_Init_FreeType(&library)) {
        fprintf(stderr, "An error occurred during freetype font library initialization\n");
    }
    face = nullptr;
}

TextLoader::~TextLoader() {
    delete face;
}

bool TextLoader::load_font(const std::string& filename) {
    FT_Face tmp_face;
    int error = FT_New_Face(library, filename.c_str(), 0, &tmp_face);
    if (error == FT_Err_Unknown_File_Format) {
        fprintf(stderr, "The font file could be opened and read, but it appears that its"
                "font format is unsupported\n");
        return false;
    } else if (error) {
        fprintf(stderr, "The font file could not be opened and read\n");
        return false;
    }

    delete face;
    face = tmp_face;

    return true;
}

GLuint TextLoader::load_char(const char char_, uint32_t font_size, unsigned int &width, unsigned int &height) {
    FT_Bitmap* bitmap = &(face->glyph->bitmap);

    bool error = FT_Set_Pixel_Sizes(face, 0, font_size);
    if (error) fprintf(stderr, "FT_Set_Pixel_Sizes\n");

    error = FT_Load_Char(face, char_, FT_LOAD_RENDER);
    if (error) fprintf(stderr, "FT_Load_Char\n");

    width = bitmap->width;
    height = bitmap->rows;

    GLuint texture = 0;
    int color = 4;

    if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
        color = 1;

    ImageHandler::createGLTextureFromBuffer(texture, bitmap->buffer, width, height, color);

    return texture;
}
