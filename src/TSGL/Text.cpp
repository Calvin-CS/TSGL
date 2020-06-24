#include "Text.h"
#include "iostream"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Text instance.
 * \details This is the constructor for the Text class.
 *      \param x The center x coordinate of the text.
 *      \param y The center y coordinate of the text.
 *      \param z The center z coordinate of the text.
 *      \param text The string to draw.
 *      \param fontFilename The path of the filename detailing the font the Text will use.
 *      \param fontsize The size of the text in pixels.
 *      \param yaw The yaw of the Text in 3D space.
 *      \param pitch The pitch of the Text in 3D space.
 *      \param roll The roll of the Text in 3D space.
 *      \param color A reference to the ColorFloat to use.
 * \return A new Text instance with the specified string, position, and color.
 */
Text::Text(float x, float y, float z, std::string text, std::string fontFilename, unsigned int fontsize, float yaw, float pitch, float roll, const ColorFloat &color)
 : Drawable(x,y,z,yaw,pitch,roll) {
    shaderType = TEXT_SHADER_TYPE;
    myString = text;
    myFont = fontFilename;
    myFontSize = fontsize;
    myColor = color;
    myXScale = myYScale = myZScale = 1;

    // FreeType
    // --------
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        TsglErr("ERROR::FREETYPE: Could not init FreeType Library");

    // load font as face
    if (FT_New_Face(ft, fontFilename.c_str(), 0, &face))
        TsglErr("ERROR::FREETYPE: Failed to load font");

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, myFontSize);

    populateCharacters();

    vertices = new float[30];                                        // Allocate the vertices

    // z and texture coordinates never change
    vertices[2] = 0; vertices[3] = vertices[4] = 0.0f;
    vertices[7] = 0; vertices[8] = 0.0f; vertices[9] = 1.0f;
    vertices[12] = 0; vertices[13] = vertices[14] = 1.0f;
    vertices[17] = 0; vertices[18] = vertices[19] = 0.0f;
    vertices[22] = 0; vertices[23] = vertices[24] = 1.0f;
    vertices[27] = 0; vertices[28] = 1.0f; vertices[29] = 0.0f;

    init = true;
}

/*!
 * \brief Draw the Text.
 * \details This function actually draws the Text to the Canvas. 
 *  \param shader Pointer to appropriate instance of Shader being used to render the Text.
 */
void Text::draw(Shader * shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(myRotationPointX, myRotationPointY, myRotationPointZ));
    model = glm::rotate(model, glm::radians(myCurrentYaw), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(myCurrentPitch), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(myCurrentRoll), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ));
    model = glm::scale(model, glm::vec3(myXScale, myYScale, myZScale));

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform4f(glGetUniformLocation(shader->ID, "textColor"), myColor.R, myColor.G, myColor.B, myColor.A);

    glEnable(GL_TEXTURE_2D);

    float mouseX = -myWidth / 2;
    float mouseY = -myHeight / 2;
    std::string::const_iterator c;
    for (c = myString.begin(); c != myString.end(); c++) {
        Character ch = Characters[*c];

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        GLuint texture;
        glGenTextures(1, &texture);
        // create and render a new texture based off the data in Characters
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            ch.Bitmap.width, 
            ch.Bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            ch.Bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // update vertices for each character
        float xpos = mouseX + ch.Bearing.x;
        float ypos = mouseY - (ch.Bitmap.rows - ch.Bearing.y);

        float w = ch.Bitmap.width;
        float h = ch.Bitmap.rows;

        //triangle 1
        vertices[0] = xpos; vertices[1] = ypos + h;
        vertices[5] = xpos; vertices[6] = ypos; 
        vertices[10] = xpos + w; vertices[11] = ypos;
        //triangle 2
        vertices[15] = xpos; vertices[16] = ypos + h;
        vertices[20] = xpos + w; vertices[21] = ypos;
        vertices[25] = xpos + w; vertices[26] = ypos + h;

        // actually draw stuff
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDeleteTextures(1, &texture);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        mouseX += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glDisable(GL_TEXTURE_2D);
}

/*!
 * \brief Alter the Text's string
 * \details This function changes myString to the parameter text
 *  \param text The text to change myString to.
 * \warning The center of the text will not change despite any differences in rendered string length.
 */
void Text::setText(std::string text) {
    attribMutex.lock();
    init = false;
    std::string::const_iterator c;
    for (c = myString.begin(); c != myString.end(); c++) {
        FT_Bitmap_Done(ft, &Characters[*c].Bitmap);
    }
    myString = text;
    populateCharacters();
    init = true;
    attribMutex.unlock();
}

/*!
 * \brief Alter the Text's font size
 * \details This function changes myFontSize to the parameter fontsize.
 *  \param fontsize The new fontsize.
 * \warning The center of the text will not change despite any differences in rendered string length.
 */
void Text::setFontSize(unsigned int fontsize) {
    attribMutex.lock();
    std::string::const_iterator c;
    for (c = myString.begin(); c != myString.end(); c++) {
        FT_Bitmap_Done(ft, &Characters[*c].Bitmap);
    }
    init = false;
    myFontSize = fontsize;

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, myFontSize);

    populateCharacters();
    init = true;
    attribMutex.unlock();
}

/*!
 * \brief Alter the Text's font
 * \details This function changes myLoader's font to the parameter font.
 *  \param filename The new font file name.
 * \warning The center of the text will not change despite any differences in rendered string length.
 */
void Text::setFont(std::string filename) {
    attribMutex.lock();
    init = false;
    std::string::const_iterator c;
    for (c = myString.begin(); c != myString.end(); c++) {
        FT_Bitmap_Done(ft, &Characters[*c].Bitmap);
    }
    FT_Done_Face(face);
    myFont = filename;
    // load font as face

    if (FT_New_Face(ft, myFont.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, myFontSize);

    populateCharacters();
    init = true;
    attribMutex.unlock();
}

/*!
 * \brief Alter the Text's color
 * \details This function changes myColor to the parameter ColorFloat
 *  \param color The ColorFloat to change myColor to.
 */
void Text::setColor(const ColorFloat& color) {
    attribMutex.lock();
    myColor = color;
    attribMutex.unlock();
}

/*!
 * \brief Private helper method for calculating Text dimensions.
 * \details This function assigns values to myWidth and myHeight based on 
 *  the glyphs loaded based on myFontSize, myFont, and myString.
 */
void Text::populateCharacters() {
    // note: this will leak memory if you don't free the bitmaps within first. see most mutators.
    Characters.clear();
    myWidth = 0;
    myHeight = 0;

    FT_Bitmap * ftbmps = new FT_Bitmap[myString.size()];

    for (int i = 0; i < myString.size(); i++) {
        // Load character glyph 
        if (FT_Load_Char(face, myString[i], FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        FT_Bitmap_Init(&ftbmps[i]);
        FT_Bitmap_Copy(ft, &face->glyph->bitmap, &ftbmps[i]);

        Character character = {
            ftbmps[i],
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        
        Characters.insert(std::pair<char, Character>(myString[i], character));

        myWidth += face->glyph->advance.x >> 6;
        if (face->glyph->bitmap.rows > myHeight)
            myHeight = face->glyph->bitmap.rows;
    }

    delete [] ftbmps;
}

Text::~Text() {
    // destroy FreeType once we're finished
    std::string::const_iterator c;
    for (c = myString.begin(); c != myString.end(); c++) {
        FT_Bitmap_Done(ft, &Characters[*c].Bitmap);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}



}