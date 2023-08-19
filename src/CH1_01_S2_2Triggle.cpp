#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// GLAD是用来管理OpenGL的函数指针

// eg1: 使用不同的VAO  VBO 创建的2个三角形
// eg2: 使用了2个不同的片元着色器分别渲染2个三角形

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void initData();

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

const char *fragmentShaderSourceGreen = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   FragColor = vec4(0.0f, 1.0f, 0.2f, 1.0f);\n"
                                        "}\n\0";

int main()
{
    // glfwInit函数来初始化GLFW
    glfwInit();
    // 配置GLFW,告诉GLFW我们要使用的OpenGL版本是3.3  主版本号(Major)和次版本号(Minor)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // GLFW我们使用的是核心模式(Core-profile)   我们只能使用OpenGL功能的一个子集
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// MAC OSX needs
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "QiangGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Fialed to create GLFW window" << std::endl;
        glfwTerminate(); // 终止
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenGL渲染窗口的尺寸大小，即视口(Viewport)
    glViewport(0, 0, 800, 600);

    // build and compile our shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShaderGreen, 1, &fragmentShaderSourceGreen, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShaderGreen);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "compile vertex fail" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "compile fragment fail" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "compile fragment green fail" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int shaderProgramGreen = glCreateProgram();
    glAttachShader(shaderProgramGreen, vertexShader);
    glAttachShader(shaderProgramGreen, fragmentShaderGreen);
    glLinkProgram(shaderProgramGreen);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "link shader fail" << infoLog << std::endl;
    }

    glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
        std::cout << "link shader green fail" << infoLog << std::endl;
    }

    // 编译完成后删除Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderGreen);

    // VAO 中包含很多 VBO ,一个VBO 中有「position,color,uv 等信息构成」
    // 使用glDrawArry 绘制2个三角形 练习题1
    float vertices1[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f, // left
        -0.0f, -0.5f, 0.0f, // right
        -0.45f, 0.5f, 0.0f  // top

    };

    float vertices2[] = {
        // second triangle
        0.0f, -0.5f, 0.0f, // left
        0.9f, -0.5f, 0.0f, // right
        0.45f, 0.5f, 0.0f  // top
    };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), &vertices1, GL_STATIC_DRAW); // GL_STATIC_DRAW  GL_DYNAMIC_DRAW   GL_STREAM_DRAW
    // 顶点起步、顶点个数、数据类型，是否希望被标准化（0，1）,步长（3个顶点）， 位置数据在缓冲中起始位置的偏移量(Offset)
    // 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2, GL_STATIC_DRAW); // GL_STATIC_DRAW  GL_DYNAMIC_DRAW   GL_STREAM_DRAW
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // // 可以安全地解除绑定
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // // 解除对 VAO 的绑定
    // glBindVertexArray(0);

    // 函数在我们每次循环的开始前检查一次GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 执行渲染 。。。
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramGreen);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 检查并调用事件，交换缓冲
        //  函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);
        // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramGreen);

    // 渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

void frame_buffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
