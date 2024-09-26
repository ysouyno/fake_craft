// fake_craft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "modern.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdio.h>

typedef struct {
  unsigned int frames;
  double timestamp;
} FPS;

void update_fps(FPS* fps) {
  fps->frames++;
  double now = glfwGetTime();
  double elapsed = now - fps->timestamp;
  if (elapsed >= 1) {
    int result = fps->frames / elapsed;
    fps->frames = 0;
    fps->timestamp = now;
    printf("%d\n", result);
  }
}

void update_matrix(float* matrix) {
  int width, height;
  glfwGetWindowSize(&width, &height);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, width, height);
  perspective_matrix(matrix, 65.0, (float)width / height, 0.1, 60.0);
}

int main(int argc, char** argv) {
  if (!glfwInit()) {
    return -1;
  }
  if (!glfwOpenWindow(600, 400, 8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
    return -1;
  }
  glfwSwapInterval(0);
  glfwDisable(GLFW_MOUSE_CURSOR);
  glfwSetWindowTitle("Modern GL");

  if (glewInit() != GLEW_OK) {
    return -1;
  }

  GLfloat vertex_data[108];
  GLfloat texture_data[72];
  make_cube(vertex_data, texture_data, 0, 0, 0, 0.5);
  GLuint vertex_buffer = make_buffer(
    GL_ARRAY_BUFFER,
    sizeof(vertex_data),
    vertex_data);

  GLuint texture_buffer = make_buffer(
    GL_ARRAY_BUFFER,
    sizeof(texture_data),
    texture_data);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glfwLoadTexture2D("texture.tga", 0);

  GLuint program = load_program("vertex.glsl", "fragment.glsl");
  GLuint matrix_loc = glGetUniformLocation(program, "matrix");
  GLuint timer_loc = glGetUniformLocation(program, "timer");
  GLuint rotation_loc = glGetUniformLocation(program, "rotation");
  GLuint sampler_loc = glGetUniformLocation(program, "sampler");
  GLuint position_loc = glGetAttribLocation(program, "position");
  GLuint uv_loc = glGetAttribLocation(program, "uv");

  FPS fps = { 0, 0 };
  float matrix[16];
  float rx = 0;
  float ry = 0;
  float m = 0.15;
  int mx, my, px, py;
  glfwGetMousePos(&px, &py);
  glEnable(GL_CULL_FACE);
  while (glfwGetWindowParam(GLFW_OPENED)) {
    update_fps(&fps);
    update_matrix(matrix);

    glfwGetMousePos(&mx, &my);
    rx += (mx - px) * m;
    ry += (my - py) * m;
    ry = ry < -90 ? -90 : ry;
    ry = ry > 90 ? 90 : ry;
    px = mx;
    py = my;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, matrix);
    glUniform1f(timer_loc, glfwGetTime());
    glUniform2f(rotation_loc, rx, ry);
    glUniform1i(sampler_loc, 0);

    glEnableVertexAttribArray(position_loc);
    glEnableVertexAttribArray(uv_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 729);

    glfwSwapBuffers();
  }

  glfwTerminate();
  return 0;
}
