#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"
#include "std_image.h"


inline void sample_1_4()
{
	/* s => x-axis, t => y-axis, r => z-axis */
	/*
		纹理坐标在x和y轴上，范围为0到1之间（注意我们使用的是2D纹理图像）。
		纹理坐标起始于(0, 0)，也就是纹理图片的左下角，终始于(1, 1)，即纹理图片的右上角。
		使用纹理坐标获取纹理颜色叫做采样(Sampling)。

	*/

	//一个步长包含3种类型
	const char *vertexShaderSource =
		"#version 430 core                                                 \n"
		"layout (location = 0) in vec3 aPos;						\n"
		"layout (location = 1) in vec3 aColor;						\n"
		"layout (location = 2) in vec2 aTexCoord;				\n"
		"																				\n"
		"out vec3 ourColor;													\n"
		"out vec2 TexCoord;												\n"
		"																				\n"
		"void main()																\n"
		"{																				\n"
		"	gl_Position = vec4(aPos, 1.0);								\n"
		"	ourColor = aColor;												\n"
		"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);	\n"
		"}                                                                              \0";

	/*
		你可能会奇怪为什么sampler2D变量是个uniform，我们却不用glUniform给它赋值。
	使用glUniform1i，我们可以给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理。
	一个纹理的位置值通常称为一个纹理单元(Texture Unit)。一个纹理的默认纹理单元是0，它是默认的激活纹理单元，所以教程前面部分我们没有分配一个位置值。

	我们还可以把得到的纹理颜色与顶点颜色混合，来获得更有趣的效果。
	我们只需把纹理颜色与顶点颜色在片段着色器中相乘来混合二者的颜色
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

	mix 最后的数值  表示第二个参数的权重

	*/
	//输出纹理叠加
	const char * fragmentShaderSource =
		" #version 430 core                                                                                                               \n"
		" out vec4 FragColor;																											  \n"
		" 																																			  \n"
		" in vec3 ourColor;																												  \n"
		" in vec2 TexCoord;																												  \n"
		" 																																			  \n"
		" 																																			  \n"
		" // texture samplers																											  \n"
		" uniform sampler2D texture1;																							  \n"
		" uniform sampler2D texture2;																							  \n"
		" uniform sampler2D texture3;                                                                                             \n"
		" 																																			  \n"
		" void main()																														  \n"
		" {																																			  \n"
		" 	// linearly interpolate between both textures																	  \n"
		"  // vec4 clr_tex1=texture2D(texture1, TextCoord);                                                             \n"
		" 	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);         \n"
		"   FragColor = texture(texture1, TexCoord) * texture(texture3, TexCoord).a;                     \n"
		" }																																			  \0";


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

	//texture coords 坐标0, 0 为左下角, 1, 1为右上角。左下角0, 0 设置成 0, 1 x轴翻转180度。
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	//两个三角形组成矩形
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	unsigned int VBO;
	unsigned int* pVAO = (unsigned int*)malloc(sizeof(unsigned int));
	unsigned int* pEBO = (unsigned int*)malloc(sizeof(unsigned int));
	glGenVertexArrays(1, pVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, pEBO);

	glBindVertexArray(*pVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//根据顶点数据结构设置偏移和步长
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	//创建纹理
	unsigned int texture1;
	glGenTextures(1, &texture1);
	//绑定纹理到当前工作上下文
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 设置平铺模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置缩放模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int imgw, imgh, nrChannels;
	unsigned char *data = stbi_load("D:\\github\\opengl-quick-reference\\opengl-quick-ref\\resources\\textures\\container.jpg", &imgw, &imgh, &nrChannels, 0);
	/*
	//为当前纹理 加载贴图
	第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
	第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
	第六个参数应该总是被设为0（历史遗留的问题）。
	第七第八个参数定义了源图的格式和数据类型。使用RGB值加载这个图像( png为RBGA )，并把它们储存为char(byte)数组，我们将会传入对应值。
	最后一个参数是真正的图像数据的指针。
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgw, imgh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//纹理2 同理
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("D:\\github\\opengl-quick-reference\\opengl-quick-ref\\resources\\textures\\awesomeface.png", &imgw, &imgh, &nrChannels, 0);
	//png有alpha通道这里要注意
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	unsigned char alpha[] = {
		0xff, 0xff, 0xff, 0xff,
		0xff, 0x00, 0x00, 0x88,
		0xff, 0x00, 0x00, 0x88,
		0xff, 0xff, 0xff, 0xff,
	};

	unsigned int texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 4, 4, 0, GL_ALPHA, GL_UNSIGNED_BYTE, alpha);
	glGenerateMipmap(GL_TEXTURE_2D);



	glUseProgram(shaderProgram);
	//激活纹理0  至少有16个单元可供选择
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//uniform sampler2D texture1; 设置片段着色器中 采样纹理texture1 为0号单元纹理（GL_TEXTURE0）
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture3"), 2);

	//绘制
	glBindVertexArray(*pVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO); //glDrawElements前必须glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//清理
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteVertexArrays(1, pVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, pEBO);

	/*
	简单的全局变换

	#version 430 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    
    out vec2 TexCoord;
    
    uniform mat4 transform;
    
    void main()
    {
    	gl_Position = transform * vec4(aPos, 1.0f);
    	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    }

		glm::mat4 trans = glm::mat4(1.0f)
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	*/

	/*
	glPixelStorei(GL_UNPACK_ALIGNMENT，4) 保证每一行数据4字节对齐
	第一到第四个参数组成一个矩形
	第五个参数 GL_RGB,  GL_RGBA。GL_RED则只读取像素的红色数据.
	第六个参数 GL_UNSIGNED_BYTE GL_FLOAT
	第七个参数表示一个数据指针
	glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	
	*/
}


