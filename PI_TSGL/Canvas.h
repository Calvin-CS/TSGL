#include <functional>
#include <iostream>
#include <mutex>
#include "Array.h"
#include "Shape.h"
#include "Texture.h"
#include "Keynums.h"
#include "Color.h"
#include "Util.h"
#include "Timer.h"
#include "Image.h"
#include "Rectangle.h"
#include "Polyline.h"

#include <sstream>
#include <string>
#include <thread>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Canvas {
 private:
  typedef GLFWvidmode const* displayInfo;
  typedef std::function<void(double, double)> doubleFunction;
  typedef std::function<void()> voidFunction;

  float aspect;
  ColorFloat bgcolor;
  voidFunction boundKeys [(GLFW_KEY_LAST+1)*2];
  std::mutex bufferMutex;
  unsigned bufferSize;
  GLint colorLoc;
  Timer* drawTimer;
  GLuint frameBuffer;
  int frameCounter;
  bool hasBackbuffer;
  bool hasEXTFramebuffer;
  bool hasStereo;
  bool isFinished;
  bool keyDown;
  TextureHandler loader;
  bool loopAround;
  int monitorX, monitorY;
  double mouseX, mouseY;
  Array<Shape*>* myBuffer;
  Array<Shape*>* myShapes;
  unsigned int pointBufferPosition, pointLastPosition;
  GLint positionLoc;
  bool readyToDraw;
  int realFPS;
  GLuint renderedTexture;
  std::thread renderThread;
  uint8_t* screenBuffer;
  doubleFunction scrollFunction;
  GLtexture shaderFragment, shaderProgram, shaderVertex;
  bool showFPS;
  bool started;
  std::mutex syncMutex;
  int syncMutexLocked;
  int syncMutexOwner;
  GLtexture textureShaderVertex, textureShaderFragment, textureShaderProgram;
  bool toClose;
  unsigned int toRecord;
  bool toClear;
  GLint uniModel, uniView, uniProj;
  GLtexture vertexArray, vertexBuffer;
  float* vertexData;
  GLFWwindow* window;
  bool windowClosed;
  int winHeight;
  std::string winTitle;
  int winWidth;
  int winWidthPadded;
  
  static int drawBuffer;
  static bool glfwIsReady;
  static std::mutex glfwMutex;
  static displayInfo monInfo;
  static unsigned openCanvases;
  
  static void buttonCallback(GLFWwindow* window, int key,
			     int action, int mods);
  void draw();
  static void errorCallback(int error, const char* string);
  void init(int xx, int yy, int ww, int hh, unsigned int b,
	    std::string title, double timerLength);
  void initGl();
  void initGlew();
  static void initGlfw();
  void initWindow();
  static void keyCallback(GLFWwindow* window, int key, int scancode,
			  int action, int mods);
  void screenShot();
  static void scrollCallback(GLFWwindow* window, double xpos,
			     double ypos);
  static void setDrawBuffer(int buffer);
  void setupCamera();
  static void startDrawing(Canvas* c);
  void textureShaders(bool state);
  
  
 protected:
  bool atiCard;
  void drawShape(Shape* s);
 public:
  Canvas(int x, int y, int width, int height, std::string title,
	 double timerLength = 0.0f);
  virtual void drawRectangle(int x1, int y1, int x2, int y2,
			     ColorFloat color = BLACK, bool filled = true);
  void bindToButton(Key button, Action action, voidFunction function);
  void clear();
  bool isOpen();
  static int getDisplayHeight();
  static int getDisplayWidth();
  void setBackgroundColor(ColorFloat color);
  void sleep();
  int start();

  int wait();

};
