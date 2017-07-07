#include "Text.h"
#include "iostream"
#include "assert.h"

namespace tsgl {

  void Text::error_check(int num=-1) {
    if (error != false){
      fprintf(stderr, "Freetype error: %d at place: %d\n", error, num);
      exit(-1);
    }
  }

  Text::Text(std::wstring text, TextureHandler &loader, int x, int y, unsigned int fontsize, const ColorFloat &color) {
  }

  Text::Text() {

    num_chars     = strlen( text );
    angle         = ( 25.0 / 360 ) * 3.14159 * 2; /* use 25 degrees     */
    // target_height = HEIGHT;

    error = FT_Init_FreeType( &library );
    error_check(); assert(error==false);

    error = FT_New_Face( library, filename, 0, &face );
    error_check(1); assert(error==false);

    error = FT_Set_Char_Size( face, 0, 12*64, 300, 300 );
    error_check(2); assert(error==false);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    }


    void Text::testRender() {
      // Current char from the FT lib
      slot = face->glyph;

      //TODO replace with something better
      float cursor = 0.0;

      for ( n = 0; n < num_chars; n++ )
      {

        error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        error_check(3); assert(error==false);

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        error_check(4); assert(error==false);

        printf("Letter %c: %x\n", text[n], slot->bitmap);

        printf("Bitmap width: %d\n", slot->bitmap.width);
        printf("Bitmap rows: %d\n", slot->bitmap.rows);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);

        printf("Pixel Mode: %d\n", slot->bitmap.pixel_mode);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // glTranslatef( x, y, 0.f );
        glTexImage2D(	GL_TEXTURE_2D,
                     	0,
                     	1, /* internal color number */
                     	slot->bitmap.width,
                     	slot->bitmap.rows,
                     	0,
                     	GL_RED,
                     	GL_UNSIGNED_BYTE,
                     	slot->bitmap.buffer);

        // printf("Bitmap buffer size:  %u\n", slot->bitmap.buffer[1000^4096^1024]);
        // exit(-1);

        // glColor4f(1.0,1.0,0.0,1.0);
        glBegin(GL_QUADS);
          glTexCoord2f( 0.f, 0.f ); glVertex2f(cursor + 0.0, 0.0);
          glTexCoord2f( 1.0, 0.f ); glVertex2f(cursor + slot->bitmap.width, 0.0);
          glTexCoord2f( 1.0, 1.0 ); glVertex2f(cursor + slot->bitmap.width, slot->bitmap.rows);
          glTexCoord2f( 0.f, 1.0 ); glVertex2f(cursor + 0.0, slot->bitmap.rows);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        // break;

        cursor += slot->bitmap.width;
    }

  }



  void Text::draw() {
    // float *vertices = new float[32];                                        // Allocate the vertices
    //
    // vertices[0]  = myX;                                                     // Pre-init the array with the start coords
    // vertices[1]  = myY;
    //
    // vertices[2] = vertices[10] = vertices[18] = vertices[26] = myColor.R;   // Texture color of the coords
    // vertices[3] = vertices[11] = vertices[19] = vertices[27] = myColor.G;   // (Default to opaque white)
    // vertices[4] = vertices[12] = vertices[20] = vertices[28] = myColor.B;
    // vertices[5] = vertices[13] = vertices[21] = vertices[29] = myColor.A;
    //
    // vertices[6]  = vertices[7] = 0.0f;          // Texture coords of top left
    // vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    // vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    // vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right
    //
    // myLoader->drawText(myString, myFontSize, vertices);
    //
    // delete[] vertices;
  }

}
