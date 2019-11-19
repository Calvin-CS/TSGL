#include <TriangleStrip.h>

namespace tsgl {

/*!
 *  \brief Explicitly construct a new TriangleStrip with monocolored fill or outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param color The color of the TriangleStrip
 *      \param filled Whether the TriangleStrip should be filled
 *          (set to true by default).
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat color, bool filled) : ConvexPolygon(numVertices, filled, !filled) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with multicolored fill or outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param color An array of colors for the TriangleStrip
 *      \param filled Whether the TriangleStrip should be filled
 *          (set to true by default).
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat color[], bool filled) : ConvexPolygon(numVertices, filled, !filled) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with different monocolored fill and outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param fillColor The color of the TriangleStrip's fill.
 *      \param outlineColor The color of the TriangleStrip's outline.
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with multicolored fill and monocolored outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param fillColor An array of colors for the TriangleStrip's fill.
 *      \param outlineColor The color of the TriangleStrip's outline.
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with monocolored fill and multicolored outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param fillColor The color of the TriangleStrip's fill.
 *      \param outlineColor An array of colors for the TriangleStrip's outline.
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor[i]);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with different multicolored fill and outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param fillColor An array of colors for the TriangleStrip's fill.
 *      \param outlineColor An array of colors for the TriangleStrip's outline.
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor[i]);
    }
}
}
