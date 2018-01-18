#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <GL/gl.h>

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;

// a call-back function
void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
  g_gl_width = width;
  g_gl_height = height;
}

int main() {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "My spinning cube", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwSetWindowSizeCallback(window, glfw_window_size_callback);
  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  // glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* glversion = glGetString(GL_VERSION); // version as a string
  const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", glversion);
  printf("GLSL version supported %s\n", glslversion);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  // Vertex Shader
  const char* vertex_shader =
  "#version 410 core                                                  \n"
  "                                                                   \n"
  "in vec4 position;                                                  \n"
  "                                                                   \n"
  "out VS_OUT                                                         \n"
  "{                                                                  \n"
  "    vec4 color;                                                    \n"
  "} vs_out;                                                          \n"
  "                                                                   \n"
  "uniform mat4 mv_matrix;                                            \n"
  "uniform mat4 proj_matrix;                                          \n"
  "                                                                   \n"
  "void main(void)                                                    \n"
  "{                                                                  \n"
  "    gl_Position = proj_matrix * mv_matrix * position;              \n"
  "    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);      \n"
  "}";


  // Fragment Shader
  const char* fragment_shader =
  "#version 410 core                                                  \n"
  "                                                                   \n"
  "out vec4 color;                                                    \n"
  "                                                                   \n"
  "in VS_OUT                                                          \n"
  "{                                                                  \n"
  "    vec4 color;                                                    \n"
  "} fs_in;                                                           \n"
  "                                                                   \n"
  "void main(void)                                                    \n"
  "{                                                                  \n"
  "    color = fs_in.color;                                           \n"
    "}";

  // Shaders compilation
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  // Create program, attach shaders to it and link it
  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  // Vertex Array Object
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Cube to be rendered
  const GLfloat vertex_positions[] = {
    -0.25f,  0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f,  0.25f,

    -0.25f,  0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f,  0.25f,  0.25f,

     0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f, -0.25f
  };

  // Vertex Buffer Object(s)

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertex_positions, GL_STATIC_DRAW);

  // Vertex attributes
  // 0: vertex position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // 1: vertex color
  static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
  static const GLfloat one = 1.0f;

  glViewport(0, 0, g_gl_width, g_gl_height);
  glClearBufferfv(GL_COLOR, 0, green);
  glClearBufferfv(GL_DEPTH, 0, &one);

  // Uniforms
  // - Model-View matrix
  // - Projection matrix
  GLint mv_location, proj_location;
  mv_location = glGetUniformLocation(shader_programme, "mv_matrix");
  proj_location = glGetUniformLocation(shader_programme, "proj_matrix");

  // Render loop
  while(!glfwWindowShouldClose(window)) {

    double currentTime = glfwGetTime();
    float f = (float)currentTime * 0.3f;

    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, g_gl_width, g_gl_height);
    glUseProgram(shader_programme);
    glBindVertexArray(vao);

    // Suggestion to manage the model-view and projection
    // transformations:
    // - use routines and structures from either GLM or
    //   sb7's vmath libraries

    // ???::mat4 mv_matrix, proj_matrix;
    // mv_matrix = [...]

    //
    // transfer mv_matrix uniform to shaders:
    // glUniformMatrix4fv [...]

    // proj_matrix = [...]
    //
    // transfer proj_matrix uniform to shaders:
    // glUniformMatrix4fv [...]

    // glDrawArrays [...]

    // update other events like input handling
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();

  return 0;
}
