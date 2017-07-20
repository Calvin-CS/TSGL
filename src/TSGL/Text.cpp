#include "Text.h"
#include "iostream"
#include "assert.h"

namespace tsgl {

  Text::Text(std::string t, int x, int y, unsigned int font_size, const ColorFloat &c, std::string fname = "assets/freefont/FreeSans.ttf") {

    discreteRender = true;

    // Convert from the param c++ style string to a c style string for freetype
    text = t.c_str();

    // X and Y coords for the text baseline
    base_x = x;
    base_y = y;

    // Font size and color
    fontsize = font_size;
    color = c;

    // Set the font's file name
    filename = fname.c_str();

    // Calculate the number of characters in the text string
    num_chars     = strlen( text );

    // Init the freetype library
    error = FT_Init_FreeType( &library );
    if (error) {
      fprintf(stderr, "Error while initializing the Freetype library!\n");
      exit(-1);
    }

    // Load/create a new font face from a TTF font file
    error = FT_New_Face( library, filename, 0, &face );
    if (error) {
      fprintf(stderr, "Error while loading a font!\n");
      exit(-1);
    }

    // Set the character size
    error = FT_Set_Char_Size( face, 0, font_size*64*2, 300, 300 );
    if (error) {
      fprintf(stderr, "Error while setting the FreeType font size!\n");
      exit(-1);
    }

    // Generate a new GL texture and get the ID
    glGenTextures(1, &texID);

    generateTextBitmaps();
  }


  void Text::generateTextBitmaps() {

    //TODO release the memory from the vec before restarting this!
    char_vec.clear();

    // Reset the maxBearing variable to zero
    maxBearing = 0;

    int maxCharWidth = 0;

    // Slot is the space where we'll keep the current char TODO make this better
    slot = face->glyph;

    // Loop through each character and generate its bitmap
    for ( n = 0; n < num_chars; n++ )
    {

      // Create a new struct for this character
      character_object* char_obj = new character_object;

      // Check to see if the character is just a space
      if (text[n] == ' ') {
        // printf("Found the space!\n");
        char_obj->isSpace = true;
      }
      else { // Not a space, an actual character
        // printf("Found a char!\t%c\n", text[n]);

        // Load the character from the font face into the slot
        error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        assert(error==false);

        // Render the bitmap of the glyph
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        assert(error==false);

        // Save the character to the object
        char_obj->character = text[n];

        // Save the width, height, and bearing into the struct
        char_obj->width = slot->bitmap.width/2;
        char_obj->texwidth = slot->bitmap.width;
        char_obj->height = slot->bitmap.rows/2;
        char_obj->texheight = slot->bitmap.rows;
        char_obj->advance_x = slot->advance.x /64 /2;
        char_obj->advance_y = slot->advance.y /64 /2;
        char_obj->bearing = slot->metrics.horiBearingY /64 /2;

        // Update maxBearing
        if (char_obj->bearing > maxBearing) maxBearing = char_obj->bearing;

        // Update the max width
        if (char_obj->width > maxCharWidth) maxCharWidth = char_obj->width;

        // // Copy the buffer to a new object
        // size_t bufSize = char_obj->width*char_obj->height;
        // char* newBuffer = new char[bufSize];
        // memcpy(newBuffer, slot->bitmap.buffer, bufSize);
        // char_obj->buffer_len = bufSize;
        // char_obj->bitmap_buffer = newBuffer;

        // Create new buffer object
        size_t bufSize = char_obj->texwidth*char_obj->texheight;
        char* newBuffer = new char[bufSize *4];

        int i,j = 0;
        for (i=0; i<bufSize; i++) {

          newBuffer[j] = slot->bitmap.buffer[i];
          newBuffer[j+1] = slot->bitmap.buffer[i];
          newBuffer[j+2] = slot->bitmap.buffer[i];
          newBuffer[j+3] = slot->bitmap.buffer[i];  //TODO implement a better blending function here

          j+=4;
        }

        char_obj->buffer_len = bufSize;
        char_obj->bitmap_buffer = newBuffer;

        // Set the size of the space char based on the max width
        space_size = maxCharWidth/3;




        // printf("Bearing: %d\n", char_obj->bearing);
        // printf("Letter %c: %x\n", text[n], slot->bitmap);
        // printf("Bitmap width: %d\n", slot->bitmap.width);
        // printf("Bitmap rows: %d\n", slot->bitmap.rows);
        // printf("Pixel Mode: %d\n", slot->bitmap.pixel_mode);
      }

      // Push the character object onto the buffer
      char_vec.push_back(char_obj);

    }
  }


  void Text::render() {

    // Bind the texture as the currently active texture
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //TODO replace with something better
    float cursor_x = 0.0;
    float cursor_y = 0.0;

    FT_UInt previous = 0;

    for(std::vector<character_object*>::iterator it = char_vec.begin(); it != char_vec.end(); ++it) {

      if( (*it)->isSpace ) {
        cursor_x+=space_size;
        previous = 0;
        continue;
      }

      glEnable(GL_TEXTURE_2D);
      // glEnable(GL_BLEND);
      // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      glBindTexture(GL_TEXTURE_2D, texID);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glColor4f(color.R, color.G, color.B, color.A);
      glTexImage2D(	GL_TEXTURE_2D,
        0,
        4, /* internal color number */
        (*it)->texwidth,
        (*it)->texheight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        (*it)->bitmap_buffer);


        int x = base_x + cursor_x;
        int y = base_y + cursor_y - ((*it)->bearing);

        if (useKerning && FT_HAS_KERNING(face) && previous) {
          FT_Vector delta;
          FT_UInt glyph_index = FT_Get_Char_Index( face, (*it)->character );
          FT_Get_Kerning(face, previous, glyph_index, FT_KERNING_DEFAULT, &delta);
          cursor_x += (delta.x >> 6);
          //TODO kerning values always return 0?  WHY?????
          // printf("Current index: %d  Previous index: %d\n", glyph_index, previous);
          // printf("Kerning flag: %d\n", FT_HAS_KERNING(face));
          // printf("Kerning used! %f\n", (delta.y >> 6));
        }
        previous = FT_Get_Char_Index( face, (*it)->character );
        // cursor_x += 2;

        glBegin(GL_QUADS);
        glTexCoord2f( 0.f, 0.f ); glVertex2f(x,                 y);
        glTexCoord2f( 1.0, 0.f ); glVertex2f(x + (*it)->width , y);
        glTexCoord2f( 1.0, 1.0 ); glVertex2f(x + (*it)->width,  y + (*it)->height);
        glTexCoord2f( 0.f, 1.0 ); glVertex2f(x,                 y + (*it)->height);
        glEnd();

        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        cursor_x += (*it)->advance_x;
        cursor_y += (*it)->advance_y;
      }
    }

    std::string Text::getString() { return text; }

    int Text::getX() { return base_x; }

    int Text::getY() { return base_y; }

    unsigned int Text::getFontSize() { return fontsize; }

    ColorFloat Text::getColor() { return color; }

    std::string Text::getFontFile() { return filename; }

    //TODO add kernign to the calculation
    int Text::getStringWidth() {
      int totalW = 0;
      for(std::vector<character_object*>::iterator it = char_vec.begin(); it != char_vec.end(); ++it) {
        // printf("Advance_X: %d\n", (*it)->advance_x);
        if ((*it)->isSpace) {
          totalW += space_size;
        }
        else {
          totalW += (*it)->advance_x;  //TODO returns huge value sometimes???
        }
      }
      return totalW;
    }

    int Text::getStringHeight() { //TODO: check that this makes sense/is best, probably change
      int max_height = 0;
      for(std::vector<character_object*>::iterator it = char_vec.begin(); it != char_vec.end(); ++it) {
        if( (*it)->height > max_height ) {
          max_height = (*it)->height;
        }
      }
      return max_height;
    }

    void Text::setString(std::string t) {
      // Convert from the param c++ style string to a c style string for freetype
      text = t.c_str();

      // Calculate the number of characters in the text string
      num_chars     = strlen( text );

      generateTextBitmaps();
    }

    void Text::setLocation(int x, int y) {
      // X and Y coords for the text baseline
      base_x = x;
      base_y = y;
    }

    void Text::setCenter(int x, int y) {
      base_x = x - getStringWidth()/2;
      base_y = y + getStringHeight()/2;
    }

    void Text::setFontSize(unsigned int font_size) {
      fontsize = font_size;

      // Set the character size
      error = FT_Set_Char_Size( face, 0, font_size*64, 300, 300 );
      if (error) {
        fprintf(stderr, "Error while setting the FreeType font size!\n");
        exit(-1);
      }
      generateTextBitmaps();
    }

    void Text::setColor(const ColorFloat &c) {
      color = c;
    }

    void Text::setFontFile(std::string fname) {
      // Set the font's file name
      filename = fname.c_str();

      // Load/create a new font face from a TTF font file
      error = FT_New_Face( library, filename, 0, &face );
      if (error) {
        fprintf(stderr, "Error while loading a font!\n");
        exit(-1);
      }

      generateTextBitmaps();
    }

  }
