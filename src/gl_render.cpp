#include <gl_render.h>

#include <gl_include.h>

#include <iostream>

#include <shader_compile.h>

#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vertex.h>
#include <fragment.h>

GLuint main_program;

GLuint quadVAO;

uvec2 screen_size;

GLint projection_id;
GLint model_id;
GLint resolution_id;
GLint zoom_id;
GLint offset_id;
GLint max_iter_id;
GLint time_id;

void render_rect(unsigned int tex_id, fvec2 in_position, fvec2 in_size, float rotation){
    glm::vec2 position = glm::vec2(in_position.x, in_position.y);
    glm::vec2 size = glm::vec2(in_size.x, in_size.y);

    glUseProgram(main_program);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    glUniformMatrix4fv(model_id, 1, false, glm::value_ptr(model));

    // glm::mat4 texpart = glm::mat4(1.0f);
    // glUniformMatrix4fv(texpart_id, 1, false, glm::value_ptr(texpart));

    glBindVertexArray(quadVAO);
    // glBindTexture(GL_TEXTURE_2D, tex_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void render_rect_init(){
    GLuint VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void shader_init(){
    // printf("%d\n", glGetError());
    main_program = shaders_source((char *)(vertex), (char *)(fragment));
    // printf("main_program: %d\n", main_program);
    // printf("%d\n", glGetError());

    // if (!glIsProgram(main_program)) {
    //     printf("Error: main_program is not a valid shader program!\n");
    // } else printf("main_program is a valid shader program\n");

    GLint linkStatus;
    glGetProgramiv(main_program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(main_program, 512, NULL, infoLog);
        printf("Shader Program Linking Error: %s\n", infoLog);
    }

    glUseProgram(main_program);
    // printf("%d\n", glGetError());

    projection_id = glGetUniformLocation(main_program, "projection");
    model_id = glGetUniformLocation(main_program, "model");
    resolution_id = glGetUniformLocation(main_program, "u_resolution");
    zoom_id = glGetUniformLocation(main_program, "u_zoom");
    offset_id = glGetUniformLocation(main_program, "u_offset");
    max_iter_id = glGetUniformLocation(main_program, "u_max_iter");
    time_id = glGetUniformLocation(main_program, "u_time");

    // printf("%d\n", glGetError());

    // std::cout << "uniform ids: "  << projection_id << " " << model_id << " " << resolution_id << " " << zoom_id << " " << offset_id << " " << max_iter_id << std::endl;
}

void gl_init(){
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Initializing OpenGL..." << std::endl;

    glClearColor(0, 0, 0, 1);

    shader_init();

    render_rect_init();

    printf("screen_size: %f, %f\n", float(screen_size.x), float(screen_size.y));
    glUniform2f(resolution_id, screen_size.x, screen_size.y);

    

    gl_zoom(1);
    gl_offset(0, 0);
}

void gl_zoom(float v){
    glUniform1f(zoom_id, v);
}

void gl_offset(float x, float y){
    glUniform2f(offset_id, x, y);
}

void gl_iter(int v){
    glUniform1i(max_iter_id, v);
}

void gl_time(float v){
    glUniform1f(time_id, v);
}

void gl_clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::translate(projection, glm::vec3(-1.0f, 1.0f, 0.0f));
    projection = glm::scale(projection, glm::vec3(2.0f / screen_size.x, -2.0f / screen_size.y, 1.0f));
    glUniformMatrix4fv(projection_id, 1, false, glm::value_ptr(projection));
}

void gl_resize(){
    glUniform2f(resolution_id, screen_size.x, screen_size.y);
    glViewport(0, 0, screen_size.x, screen_size.y);
}