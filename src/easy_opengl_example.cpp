/*
  
  Easy OpenGL example
  -------------------

  The capture library provides a wrapper for rendering frames
  using OpenGL. The wrapper tries to use one of the optimal YUV
  pixel formats.

 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ROXLU_USE_MATH
#define ROXLU_USE_OPENGL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>

#define VIDEO_CAPTURE_IMPLEMENTATION
#include <videocapture/CaptureGL.h>
using namespace ca;

// Capture 
void on_signal(int sig);                                /* Signal handler, to correctly shutdown the capture process */

// GLFW callbacks
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

int main() {

  signal(SIGINT, on_signal);

  glfwSetErrorCallback(error_callback);
 
  if(!glfwInit()) {
    printf("Error: cannot setup glfw.\n");
    return false;
  }
 
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* win = NULL;
  int w = 1024;
  int h = 768;

  win = glfwCreateWindow(w, h, "Capture", NULL, NULL);
  if(!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(win, resize_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, char_callback);
  glfwSetCursorPosCallback(win, cursor_callback);
  glfwSetMouseButtonCallback(win, button_callback);
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    printf("Cannot load GL.\n");
    exit(EXIT_FAILURE);
  }

  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------

  CaptureGL capture; 

  capture.cap.listDevices();
  capture.cap.listCapabilities(0);

#define USE_CAPABILITY 0
#if USE_CAPABILITY
  Settings cfg;
  cfg.device = 0;
  cfg.capability = 12;

  if (capture.open(cfg) < 0) {
    printf("Error: cannot open using the given capability.\n");
    exit(EXIT_FAILURE);
  }
#else 
  //if(capture.open(0, 640, 480) < 0) {
  if(capture.open(0, 800, 600) < 0) {
  // if(capture.open(0, 1280, 720) < 0) {
  // if(capture.open(0, 1920, 1080) < 0) {
    printf("Cannot open the capture device.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  if(capture.start() < 0) {
    ::exit(EXIT_FAILURE);
  }

  while(!glfwWindowShouldClose(win)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    capture.update();
    capture.draw();
    capture.draw(10, 10, 320, 240);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  if(action != GLFW_PRESS) {
    return;
  }

  if(action == GLFW_PRESS) { 
  }
  else if(action == GLFW_RELEASE) {
  }
 
  switch(key) {
    case GLFW_KEY_SPACE: {
      break;
    }
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
  };
}
 
void resize_callback(GLFWwindow* window, int width, int height) {
}

void cursor_callback(GLFWwindow* win, double x, double y) {
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  double x,y;

  if(action == GLFW_PRESS || action == GLFW_REPEAT) { 
    glfwGetCursorPos(win, &x, &y);
  }

  if(action == GLFW_PRESS) {
  }
  else if(action == GLFW_RELEASE) {
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

void on_signal(int sig) {
  ::exit(EXIT_FAILURE);
}
