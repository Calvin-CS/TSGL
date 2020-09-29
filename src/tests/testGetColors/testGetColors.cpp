/*
 * testGetColors.cpp
 *
 * Usage: ./testGetColors
 */

#include <tsgl.h>

using namespace tsgl;

bool testArrow(ColorFloat * colors) {
    Arrow * arrow = new Arrow(0,0,0,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    arrow->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("a%d ", i);
        if (cVec[i] != colors[i]) {
            delete arrow;
            return false;
        }
    }
    printf("\n");
    delete arrow;
    return true;
}

bool testCircle(ColorFloat * colors) {
    Circle * circle = new Circle(0,0,0,50,0,0,0,colors); 
    std::vector<ColorFloat> cVec;
    circle->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("b%d ", i);
        if (cVec[i] != colors[i]) {
            delete circle;
            return false;
        }
    }
    printf("\n");
    delete circle;
    return true;
}

bool testCube(ColorFloat * colors) {
    Cube * cube = new Cube(0,0,0,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    cube->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("c%d ", i);    
        if (cVec[i] != colors[i]) {
            delete cube;
            return false;
        }
    }
    printf("\n");
    delete cube;
    return true;
}

bool testCuboid(ColorFloat * colors) {
    Cuboid * cuboid = new Cuboid(0,0,0,50,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    cuboid->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("d%d ", i);    
        if (cVec[i] != colors[i]) {
            delete cuboid;
            return false;
        }
    }
    printf("\n");
    delete cuboid;
    return true;
}

bool testEllipse(ColorFloat * colors) {
    Ellipse * ellipse = new Ellipse(0,0,0,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    ellipse->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("e%d ", i);    
        if (cVec[i] != colors[i]) {
            delete ellipse;
            return false;
        }
    }
    printf("\n");
    delete ellipse;
    return true;
}

bool testEllipsoid(ColorFloat * colors) {
    Ellipsoid * ellipsoid = new Ellipsoid(0,0,0,50,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    ellipsoid->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("f%d ", i);    
        if (cVec[i] != colors[i]) {
            delete ellipsoid;
            return false;
        }
    }
    printf("\n");
    delete ellipsoid;
    return true;
}

bool testPrism(ColorFloat * colors) {
    Prism * prism = new Prism(0,0,0,5,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    prism->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("g%d ", i);
        if (cVec[i] != colors[i]) {
            delete prism;
            return false;
        }
    }
    printf("\n");
    delete prism;
    return true;
}

bool testPyramid(ColorFloat * colors) {
    Pyramid * pyramid = new Pyramid(0,0,0,5,50,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    pyramid->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("h%d ", i);    
        if (cVec[i] != colors[i]) {
            delete pyramid;
            return false;
        }
    }
    printf("\n");
    delete pyramid;
    return true;
}

bool testSphere(ColorFloat * colors) {
    Sphere * sphere = new Sphere(0,0,0,50,0,0,0,colors);
    std::vector<ColorFloat> cVec;
    sphere->getColors(cVec);
    for (int i = 0; i < cVec.size(); i++) {
        printf("i%d ", i);    
        if (cVec[i] != colors[i]) {
            delete sphere;
            return false;
        }
    }
    printf("\n");
    delete sphere;
    return true;
}

bool testOthers(ColorFloat * colors) {
    RegularPolygon * rp = new RegularPolygon(0,0,0,50,5,0,0,0,colors);
    std::vector<ColorFloat> cVec1;
    rp->getColors(cVec1);
    for (int i = 0; i < cVec1.size(); i++) {
        printf("j%d ", i);    
        if (cVec1[i] != colors[i]) {
            delete rp;
            return false;
        }
    }
    printf("\n");
    delete rp;
    std::vector<ColorFloat> cVec2;
    Square * square = new Square(0,0,0,50,0,0,0,colors);
    square->getColors(cVec2);
    for (int i = 0; i < cVec2.size(); i++) {
        printf("k%d ", i);    
        if (cVec2[i] != colors[i]) {
            delete square;
            return false;
        }
    }
    printf("\n");
    delete square;
    return true;
}
 
int main(int argc, char* argv[]) {
    ColorFloat colors[] = { ColorFloat(1,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    tsglAssert(testArrow(colors), "Arrow::getColors assertion failed.");
    tsglAssert(testCircle(colors), "Circle::getColors assertion failed.");
    tsglAssert(testCube(colors), "Cube::getColors assertion failed.");
    tsglAssert(testCuboid(colors), "Cuboid::getColors assertion failed.");
    tsglAssert(testEllipse(colors), "Ellipse::getColors assertion failed.");
    tsglAssert(testEllipsoid(colors), "Ellipsoid::getColors assertion failed.");
    tsglAssert(testPrism(colors), "Prism::getColors assertion failed.");
    tsglAssert(testPyramid(colors), "Pyramid::getColors assertion failed.");
    tsglAssert(testSphere(colors), "Sphere::getColors assertion failed.");
    tsglAssert(testOthers(colors), "Others::getColors assertion failed.");
    printf("All tests passed!\n");
}