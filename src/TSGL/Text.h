/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
//TODO move the includes up in the stack so they can be used by the canvas??

#include "Drawable.h"          // For extending our Drawable object
#include "TextureHandler.h"

namespace tsgl {

class Text : public Drawable {
 private:

    FT_Library    library;
    FT_Face       face;

    FT_GlyphSlot  slot;
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;

    int base_x;
    int base_y;

    ColorFloat color;
    unsigned int fontsize;
    int space_size;

    bool useKerning = true;

    struct character_object {
      char character;
      bool isSpace = false;
      int width;
      int height;
      int texwidth;
      int texheight;
      int advance_x;
      int advance_y;
      double bearing;
      unsigned long int buffer_len;
      char* bitmap_buffer;
    };

    std::vector<character_object*> char_vec; // Hold pointers to the buffers for the character objects

    int maxBearing;

    const char*         filename;
    const char*         text;

    double angle;
    int target_height;
    int n, num_chars;

    GLuint texID = 0;

 public:

   /**
    * \brief Explicitly construcucts a new Text object
    * \details This creates a new Text item.
    *   \param t The string to display.
    *   \param x The x coordinate of the base of the Text.
    *   \param y The y coordinate of the base of the Text.
    *   \param font_size The size of the Text's font.
    *   \param c Reference of the color for the Text.
    *   \param fname The location of the font's file. (Defaults to FreeSans.)
    */
    Text(std::string t, int x, int y, unsigned int font_size, const ColorFloat &c, std::string fname = "assets/freefont/FreeSans.ttf");
    void generateTextBitmaps();
    void render();

    //Getters
    /**
     * \brief Accessor for the string displayed by the Text
     * \return The text string.
     */
    std::string getString();

    /**
     * \brief Accessor for the x coordinate.
     * \return The base x coordinate.
     */
    int getX();

    /**
     * \brief Accessor for the y coordinate.
     * \return The base y coordinate.
     */
    int getY();

    /**
     * \brief Accessor for the font size.
     * \return The int value of the font size.
     */
    unsigned int getFontSize();

    /**
     * \brief Accessor for the Text color.
     * \return The ColorFloat the Text is drawn with.
     */
    ColorFloat getColor();

    /**
     * \brief Accessor for the path to the font file.
     * \return A string showing the path to font used.
     */
    std::string getFontFile();

    /**
     * \brief Accessor for the width of the displayed string.
     * \return The int width of the text, in pixels.
     */
    int getStringWidth();

    /**
     * \brief Accessor for the height of the displayed string.
     * \return The int height of the text, in pixels.
     */
    int getStringHeight();

    //Setters
    /**
     * \brief Sets the text to a new string.
     * \details Takes a new string and updates the visible text.
     *    \param t The new text to display.
     */
    void setString(std::string t);

    /**
     * \brief Moves the text to a new location.
     * \details Displays the text at different x, y coordinates for its base.
     *    \param x The new x coordinate for the left side of the text.
     *    \param y The new y coordinate for the bottom of the text.
     */
    void setLocation(int x, int y);

    /**
     * \brief Centers the text at a new location.
     * \details Displays the text at different x, y coordinates for its center.
     *    \param x The new x coordinate for the center of the text.
     *    \param y The new y coordinate for the center of the text.
     */
    void setCenter(int x, int y);

    /**
     * \brief Changes the font size.
     * \details Updates the size of the Text's font.
     *    \param font_size The new size for the font.
     */
    void setFontSize(unsigned int font_size);

    /**
     * \brief Changes the Text's color.
     * \details Sets the Text to a new color.
     *    \param c The reference to the Text's new ColorFloat.
     */
    void setColor(const ColorFloat &c);

    /**
     * \brief Changes the Text's font.
     * \details Regenerates the font with a new font.
     *    \param fname Path to the ttf file for the new font.
     */
    void setFontFile(std::string fname); //TODO: test

};

}

#endif /* TEXT_H_ */
