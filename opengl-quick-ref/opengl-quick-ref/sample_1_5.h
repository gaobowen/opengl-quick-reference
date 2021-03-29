#pragma once


#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

inline void sample_1_5()
{
	/*
	一个变量被声明维shared，那么它将被保存到特定的位置，从而对同一个本地工作组内所有计算着色器可见。

	*/

	const GLchar* Program = " \
    #version 430\n\
    layout (local_size_x = 16, local_size_y = 1) in;\n\
    layout (r32f, binding = 0) uniform image1D in_array; \n\
    layout (r32f, binding = 1) uniform image1D out_array; \n\
    \
    void main() \n\
    { \
        int pos = int(gl_GlobalInvocationID.x);\n\
        vec4 value = imageLoad(in_array, pos);\n\
        value.x += 1.0f;\n\
        imageStore(out_array, pos, value);\n\
    } \
";


}