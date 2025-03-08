//generated with fths (find in cmake)
unsigned char vertex[] = 
"#version 300 es\n"
"layout (location = 0) in vec4 vertex;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
"}\n"
"";