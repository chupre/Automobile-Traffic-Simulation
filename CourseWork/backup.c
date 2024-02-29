//#include <glad/glad.h> 
//#include <glfw/glfw3.h>
//#include <stdio.h>
//#include <stdbool.h>
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
//"}\n";
//
//GLuint WINDOW_WIDTH = 800;
//GLuint WINDOW_HEIGHT = 600;
//GLchar WINDOW_NAME[] = "OpenGL TestWindow";
//
//int main()
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
//    glfwMakeContextCurrent(window);
//
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//    glViewport(0, 0, 800, 600);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    GLfloat vertices[] =
//    {
//     0.5f,  0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//    -0.5f, -0.5f, 0.0f,
//    -0.5f,  0.5f, 0.0f
//    };
//
//    GLuint indices[] =
//    {
//        0, 1, 3,
//        1, 2, 3
//    };
//
//    GLuint EBO;
//    glGenBuffers(1, &EBO);
//
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//
//    GLuint VAO;
//    glGenVertexArrays(1, &VAO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
//
//    GLuint vertexShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader;
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram;
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}