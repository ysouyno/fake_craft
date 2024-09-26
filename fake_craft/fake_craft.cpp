// fake_craft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "modern.h"
#include <GL/glew.h>
#include <GL/glfw.h>

static GLfloat g_vertex_buffer_data[24];

static const GLushort g_element_buffer_data[] = {
  0, 3, 2,
  0, 1, 3,
  4, 7, 5,
  4, 6, 7,
  2, 3, 7,
  2, 7, 6,
  0, 4, 5,
  0, 5, 1,
  1, 5, 7,
  1, 7, 3,
  0, 6, 4,
  0, 2, 6,
};

void set_3d(float* matrix) {
  int width;
  int height;
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
  glfwSetWindowTitle("Modern GL");
  if (glewInit() != GLEW_OK) {
    return -1;
  }
  make_cube(g_vertex_buffer_data, 0, 0, 0, 0.5);
  GLuint vertex_buffer = make_buffer(
    GL_ARRAY_BUFFER,
    sizeof(g_vertex_buffer_data),
    g_vertex_buffer_data
  );
  GLuint element_buffer = make_buffer(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(g_element_buffer_data),
    g_element_buffer_data
  );
  GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, "vertex.glsl");
  GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, "fragment.glsl");
  GLuint program = make_program(vertex_shader, fragment_shader);
  float matrix[16];
  while (glfwGetWindowParam(GLFW_OPENED)) {
    glClearColor(0.5, 0.69, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_3d(matrix);

    glUseProgram(program);
    glUniformMatrix4fv(
      glGetUniformLocation(program, "matrix"),
      1, GL_FALSE, matrix);
    glUniform1f(
      glGetUniformLocation(program, "timer"),
      glfwGetTime());

    GLuint index = glGetAttribLocation(program, "position");
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0, 81);
    glDisableVertexAttribArray(index);

    glfwSwapBuffers();
  }
  return 0;
}
