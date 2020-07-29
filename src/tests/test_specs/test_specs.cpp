#include <tsgl.h>

using namespace tsgl;

int main(int argc, char* argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(100, 100, "", NULL, NULL);
  if (!window) {
    fprintf(stderr, "GLFW window creation failed.");
    exit(100);
  }
  glfwMakeContextCurrent(window);
  const GLubyte* gfxVendor = glGetString(GL_VENDOR);
  std::string gfx(gfxVendor, gfxVendor + strlen((char*)gfxVendor));

  FT_Library flib = NULL;
  FT_Init_FreeType(&flib);
  FT_Int fmaj = 0, fmin = 0, fpatch = 0;
  FT_Library_Version(flib,&fmaj,&fmin,&fpatch);

  printf("Vendor:           %s %s\n", gfx.c_str(), glGetString(GL_RENDERER));
  printf("OpenGL version:   %s\n", glGetString(GL_VERSION));
  printf("GLFW version:     %s\n", glfwGetVersionString());
  printf("GLEW version:     %s\n", glewGetString(GLEW_VERSION));
  printf("Freetype version: %d.%d.%d\n", (int)fmaj,(int)fmin,(int)fpatch);

  glfwTerminate();
}
