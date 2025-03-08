#include <gl_render.h>

#include <gl_include.h>

#include <iostream>

#include <shader_compile.h>

#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint main_program;

GLuint quadVAO;

uvec2 screen_size;

GLint projection_id;
GLint model_id;
GLint resolution_id;
GLint zoom_id;
GLint offset_id;

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
    // unsigned char vertex[] = 
    // "#version 300 es\n"
    // "layout (location = 0) in vec4 vertex;\n"
    // "\n"
    // "out vec2 TexCoords;\n"
    // "\n"
    // "uniform mat4 model;\n"
    // "uniform mat4 projection;\n"
    // "uniform mat4 texpart;\n"
    // "\n"
    // "void main()\n"
    // "{\n"
    // "    TexCoords = (texpart * vec4(vertex.zw, 0.0, 1.0)).xy;\n"
    // "    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
    // "}\n"
    // "";

    // unsigned char fragment[] = 
    // "#version 300 es\n"
    // "precision mediump float;\n"
    // "in vec2 TexCoords;\n"
    // "out vec4 fragcolor;\n"
    // "\n"
    // "uniform vec2 u_resolution; // Screen resolution (width, height)\n"
    // "uniform float u_time;      // Time variable for animations (optional)\n"
    // "\n"
    // "vec3 getColor(int iter, int maxIter) {\n"
    // "    // Normalize iteration count to a value between 0 and 1\n"
    // "    float t = float(iter) / float(maxIter);\n"
    // "    // Return a simple gradient (you can customize this)\n"
    // "    return vec3(t, t * t, t * t * t);\n"
    // "}\n"
    // "\n"
    // "void main() {\n"
    // "    // Map the fragment coordinate to the complex plane\n"
    // "    vec2 uv = (TexCoords - 0.5 * u_resolution) / u_resolution.y;\n"
    // "    \n"
    // "    // For Mandelbrot: c is the mapped coordinate and z starts at (0,0)\n"
    // "    vec2 c = uv;\n"
    // "    vec2 z = vec2(0.0);\n"
    // "    \n"
    // "    int maxIter = 100;\n"
    // "    int i;\n"
    // "\n"
    // "    // Iteratively apply z = z^2 + c\n"
    // "    for(i = 0; i < maxIter; i++){\n"
    // "         // If the magnitude squared exceeds 4, assume divergence\n"
    // "         if(dot(z, z) > 4.0) break;\n"
    // "         // Calculate z^2: (a+bi)^2 = (a^2 - b^2) + 2abi\n"
    // "         z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;\n"
    // "    }\n"
    // "    \n"
    // "    // Use the iteration count to choose a color\n"
    // "    vec3 color = getColor(i, maxIter);\n"
    // "    fragcolor = vec4(color, 1.0);\n"
    // "}\n";


    // unsigned char fragment[] = 
    // "#version 300 es\n"
    // "precision mediump float;\n"
    // "in vec2 TexCoords;\n"
    // "out vec4 fragcolor;\n"
    // "\n"
    // "uniform vec2 u_resolution; // Screen resolution (width, height)\n"
    // "uniform float u_time;      // Time variable for animations (optional)\n"
    // "\n"
    // "void main() {\n"
    // "    vec3 color;\n"
    // // "    color.xy = TexCoords.xy / u_resolution.xy;\n"
    // "    color.r = TexCoords.x / u_resolution.x;\n"
    // "    color.g = TexCoords.y / u_resolution.y;\n"
    // "    fragcolor = vec4(color.rg, TexCoords.x + TexCoords.y, 1.0);\n"
    // "}\n";

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

    unsigned char fragment[] = 
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 fragcolor;\n"
    "\n"
    "uniform vec2 u_resolution;\n"
    "uniform float u_time;\n"
    "uniform float u_zoom;\n"
    "uniform vec2 u_offset;\n"
    "\n"
    "vec3 getColor(int iter, int maxIter) {\n"
    "    float t = float(iter) / float(maxIter);\n"
    "    return vec3(t, t * t, t * t * t);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution) / u_resolution.y;\n"
    "    \n"
    "    vec2 c = uv / u_zoom + u_offset;\n"
    "    vec2 z = vec2(0.0);\n"
    "    \n"
    "    int maxIter = 100;\n"
    "    int i;\n"
    "\n"
    "    for(i = 0; i < maxIter; i++){\n"
    "         if(dot(z, z) > 4.0) break;\n"
    "         z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;\n"
    "    }\n"
    "    \n"
    "    vec3 color = getColor(i, maxIter);\n"
    "    fragcolor = vec4(color, 1.0);\n"
    "}\n";

    printf("%d\n", glGetError());
    main_program = shaders_source((char *)(vertex), (char *)(fragment));
    printf("main_program: %d\n", main_program);
    printf("%d\n", glGetError());

    if (!glIsProgram(main_program)) {
        printf("Error: main_program is not a valid shader program!\n");
    } printf("main_program is a valid shader program\n");

    GLint linkStatus;
    glGetProgramiv(main_program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(main_program, 512, NULL, infoLog);
        printf("Shader Program Linking Error: %s\n", infoLog);
    }

    glUseProgram(main_program);
    printf("%d\n", glGetError());

    projection_id = glGetUniformLocation(main_program, "projection");
    model_id = glGetUniformLocation(main_program, "model");
    resolution_id = glGetUniformLocation(main_program, "u_resolution");
    zoom_id = glGetUniformLocation(main_program, "u_zoom");
    offset_id = glGetUniformLocation(main_program, "u_offset");

    printf("%d\n", glGetError());

    std::cout << "uniform ids: "  << projection_id << " " << model_id << " " << resolution_id << " " << zoom_id << " " << offset_id << std::endl;
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