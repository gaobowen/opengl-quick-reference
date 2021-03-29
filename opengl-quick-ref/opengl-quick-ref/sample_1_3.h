#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"


//着色器详细用法
inline void sample_1_3(float* change)
{
	/*
	向量重组
	vec2 someVec;
	vec4 differentVec = someVec.xyxx;
	vec3 anotherVec = differentVec.zyw;
	vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

	vec2 vect = vec2(0.5, 0.7);
	vec4 result = vec4(vect, 0.0, 0.0);
	vec4 otherResult = vec4(result.xyz, 1.0);
	*/

	/*
	GLSL定义了in和out关键字专门来实现这个目的。
	每个着色器使用这两个关键字设定输入和输出，只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。
	但在顶点和片段着色器中会有点不同。

	顶点着色器，接收的是一种特殊形式的输入，否则就会效率低下。
	为了定义顶点数据该如何管理，我们使用location这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。
	顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。

	片段着色器，它需要一个vec4颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。
	如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。

	如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。
	当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。

	uniform，是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同。
	首先，uniform是全局的(Global)。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问。
	第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新。
	*/

	const char *vertexShaderSource =
		"#version 430 core                                                                                            \n"
		"layout(location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 					  \n"
		"layout(location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1					  \n"
		"																														  \n"
		"out vec3 ourColor; // 向片段着色器输出一个颜色											  \n"
		"uniform float xOffset;																					  \n"
		"void main()																										  \n"
		"{																														  \n"
		"	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);			    			  \n"
		"	ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色 \n"
		"}																														  \0";

	const char * fragmentShaderSource =
		"#version 430 core								       \n"
		"out vec4 FragColor;							       \n"
		"in vec3 ourColor;                                      \n"
		"void main()												   \n"
		"{																	\n"
		"	FragColor = vec4(ourColor, 1.0f);	        \n"
		"} 														            \0";

	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//设置位置属性  layout(location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置颜色属性  layout(location = 1)，起始的颜色属性在3个float位置属性之后，这里第六个参数填(void*)(3 * sizeof(float))
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	*change += 0.001;
	
	glUseProgram(shaderProgram);
	//必须glUseProgram后才生效
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "xOffset");
	glUniform1f(vertexColorLocation, *change);

	//loop
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
