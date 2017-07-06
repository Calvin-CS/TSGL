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

    error = FT_Set_Char_Size( face, 50 * 64, 0, 100, 0 );
    error_check(2); assert(error==false);

    }


    void Text::testRender() {
      // Current char from the FT lib
      slot = face->glyph;

      for ( n = 7; n < num_chars; n++ )
      {

        error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        error_check(3); assert(error==false);

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO );
        error_check(4); assert(error==false);

        printf("Letter %c: %x\n", text[n], slot->bitmap);

        printf("Bitmap width: %d\n", slot->bitmap.width);
        printf("Bitmap rows: %d\n", slot->bitmap.rows);

        // int i = 0;
        // int j = 0;
        // for (i = 0; i < slot->bitmap.rows*slot->bitmap.width; i++) {
        //   printf("%d ", slot->bitmap.buffer[n]);
        // }

        // exit(1);

        // glBitmap(slot->bitmap.width, slot->bitmap.rows, 0, 0,
        //   0,0, slot->bitmap.buffer
        // );


        glTexImage2D(	GL_TEXTURE_2D,
                     	0,
                     	GL_RGBA, /* internal color number */
                     	slot->bitmap.width,
                     	slot->bitmap.rows,
                     	0,
                     	GL_R8,
                     	GL_UNSIGNED_BYTE,
                     	slot->bitmap.buffer);

        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0,1.0,0.0,.3);
        glBegin(GL_QUADS);
          glVertex2f(10.0, 10.0);
          glVertex2f(10.0, 30.0);
          glVertex2f(50.0, 30.0);
          glVertex2f(50.0, 10.0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        break;
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
