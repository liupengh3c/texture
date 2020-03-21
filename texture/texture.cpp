// texture.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL.h>
// GLFW
#include <GLFW/glfw3.h>
int main()
{
    cout << "Hello World!\n";
    int width, height;
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "texture study", nullptr, nullptr);

    if (window == nullptr)
    {
        cout << "failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    cout << "width=" << width << "height=" << height << endl;
    glViewport(0, 0, width, height);

    // Shader,顶点着色器
    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "vertexColor = vec4(color,1.0f);\n"
        "}\0";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    }

    // 片段着色器1
    const GLchar* fragmentShader1Source = "#version 330 core\n"
        "out vec4 color;\n"
        "in vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "color = vertexColor;\n"
        "}\n\0";

    GLuint fragment1Shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment1Shader, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragment1Shader);

    // 创建shader对象
    GLuint shader1Program = glCreateProgram();
    glAttachShader(shader1Program, vertexShader);
    glAttachShader(shader1Program, fragment1Shader);
    glLinkProgram(shader1Program);

    // 片段着色器2
    const GLchar* fragmentShader2Source = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "color = ourColor;\n"
        "}\n\0";

    GLuint fragment2Shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment2Shader, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragment2Shader);

    // 创建shader对象
    GLuint shader2Program = glCreateProgram();
    glAttachShader(shader2Program, vertexShader);
    glAttachShader(shader2Program, fragment2Shader);
    glLinkProgram(shader2Program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragment1Shader);
    glDeleteShader(fragment2Shader);

    // 渲染一个矩形
    GLfloat vertices[] = {
        // 位置                // 颜色              // 纹理坐标
         -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Bottom Left
         -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // top middle
          0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Top Left 
          0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
          1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    };
    //GLfloat vertices[] = {         
    //     -1.0f,  0.0f,  0.0f,   // Bottom Left
    //     -0.5f,  1.0f,  0.0f,   // top middle
    //      0.0f,  0.0f,  0.0f,   // Top Left 
    //      0.5f, -1.0f,  0.0f,  
    //      1.0f,  0.0f,  0.0f
    //};
    GLuint firstIndices[] = {  // Note that we start from 0!
        0, 1, 2, // First Triangle
    };

    GLuint secondIndices[] = {  // Note that we start from 0!
        2, 3, 4, // First Triangle
    };

    GLuint VBO[2], VAO[2], EBO[2];
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);
    glGenVertexArrays(2, VAO);

    // 第一个三角形
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstIndices), firstIndices, GL_STATIC_DRAW);

    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0); // Unbind VAO

    // 第二个三角形
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(secondIndices), secondIndices, GL_STATIC_DRAW);

    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO


    while (!glfwWindowShouldClose(window))
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 第一个三角形
        glUseProgram(shader1Program);
        glBindVertexArray(VAO[0]);
       
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // 第二个三角形 
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shader2Program, "ourColor");
        glUseProgram(shader2Program);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(2, EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
