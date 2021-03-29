#pragma once

#include<Windows.h>
#include "GL/glew.h"

//设置单个顶点绘制三角形
inline void sample_1_0()
{

	/*OpenGL仅当3D坐标在3个轴（x、y和z）上都为 [-1.0, 1.0] 的范围内时才处理它。
	所有在所谓的标准化设备坐标(Normalized Device Coordinates)范围内的坐标才会最终呈现在屏幕上（在这个范围以外的坐标都不会显示）。*/
	glPushMatrix();
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 1.0f); //x=0.0 为屏幕水平方向1/2处, y=1.0 为屏幕顶部， 即屏幕顶部1/2处。
	glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.87f, -0.5f);//右下
	glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(-0.87f, -0.5f);//左下
	glEnd();
	glPopMatrix();

}