#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

//着色器绘制三角形
inline void sample_1_1()
{

	//顶点着色器
	//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)
	//由于每个顶点都有一个3D坐标，我们就创建一个vec3输入变量aPos
	//layout (location = 0)设定了输入变量的位置值(Location)
	//为了设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量，它在幕后是vec4类型的。
	//在main函数的最后，我们将gl_Position设置的值会成为该顶点着色器的输出。
	//也可以单独定义成文件再载入
	const char *vertexShaderSource =
		"#version 430 core                                                 \n"
		"layout (location = 0) in vec3 aPos;                       \n"
		"void main()                                                            \n"
		"{                                                                             \n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  \n"
		"}                                                                             \0";

	//加载编译着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//是否编译成功
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器
	/*片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，我们应该自己将其计算出来。
	声明输出变量可以使用out关键字，这里我们命名为FragColor。
	下面，我们将一个Alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。*/
	const char * fragmentShaderSource =
		"#version 430 core								       \n"
		"out vec4 FragColor;							       \n"
		"void main()												   \n"
		"{																	\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);	\n"
		"} 														            \0";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//着色器程序
	//着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	//两个着色器现在都编译了，剩下的事情是把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中。
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//激活这个程序对象
	glUseProgram(shaderProgram);
	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//顶点数组对象
	/*
	顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
	这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。
	这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//顶点缓冲对象(Vertex Buffer Objects, VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	/*现在，我们已经把输入顶点数据发送给了GPU，并指示了GPU如何在顶点和片段着色器中处理它。
	就快要完成了，但还没结束，OpenGL还不知道它该如何解释内存中的顶点数据，以及它该如何将顶点数据链接到顶点着色器的属性上。
	我们需要告诉OpenGL怎么做。
	*/
	//链接顶点属性
	/*
	 第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用 layout(location = 0), 把顶点属性的位置值设置为0。
			因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	 第四个参数我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
			要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
			一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
	 第六个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。（参考sample_1_3.h）

	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//解绑VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//解绑VBO
	glBindVertexArray(0);


	//loop
	//循环绘制只需要下面3步即可
	//激活这个程序对象
	glUseProgram(shaderProgram);
	//复用VAO
	glBindVertexArray(VAO);
	//绘制Array
	//
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

