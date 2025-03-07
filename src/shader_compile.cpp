#include <gl_include.h>

#include <iostream>

#include <string.h>

#include <shader_compile.h>

GLuint compile_shader(GLenum type/*, GLsizei nsources*/, const char **sources)
{
	GLuint  shader;
	GLint   success, len;
	//GLsizei i, srclens[nsources];
	GLsizei i;
	const GLsizei nsources = 1;
	GLsizei srclens[nsources];

	for (i = 0; i < nsources; ++i)
		srclens[i] = GLsizei(strlen(sources[i]));
	shader = glCreateShader(type);
	glShaderSource(shader, nsources, sources, srclens);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		printf("Error compiling shader");
		if (len > 1) {
			char *log;
			log = (char *)malloc(len);
			glGetShaderInfoLog(shader, len, NULL, log);
			fprintf(stderr, "%s\n\n", log);
			printf(": %s", log);
			free(log);
		}
		printf("\n");
		//SDL_Log("Error compiling shader.\n");
	}
	printf("shader: %u\n",shader);
	//SDL_Log("shader: %u",shader);

	return shader;
}

GLuint shaders_source(const char *vsSource, const char *fsSource){
	// GLenum err = glewInit();
	// if (err != GLEW_OK) printf("error");
	GLuint vertexShader;
	GLuint fragmentShader;
	vertexShader   = compile_shader(GL_VERTEX_SHADER,   &vsSource);
	fragmentShader = compile_shader(GL_FRAGMENT_SHADER, &fsSource);

    GLuint shading_program_id = glCreateProgram();
	if (shading_program_id == 0) printf("shading program error\n");

	glAttachShader(shading_program_id, vertexShader);
	glAttachShader(shading_program_id, fragmentShader);

	glLinkProgram(shading_program_id);

	return shading_program_id;
}