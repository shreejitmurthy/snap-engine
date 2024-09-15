//
// Created by Shreejit Murthy on 7/9/2024.
//

#include "shader.h"
#include <glad/glad.h>
#include <SDL3/SDL_iostream.h>
#include <log/src/log.h>

void checkCompileErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            log_error("SNP::Shader compilation error of type: %s, %s", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            log_error("SNP::Shader program linking error of type: %s, %s", type, infoLog);
        }
    }
}

char* readFile(const char* filePath) {
    if (filePath != NULL) {
        // Open the file
        SDL_IOStream* io = SDL_IOFromFile(filePath, "rb");
        if (io == NULL) {
            log_error("SNP::Shader file unsuccessfully read: %s", filePath);
            return NULL;
        }

        // Get the length of the file
        Sint64 length = SDL_GetIOSize(io);
        if (length < 0) {
            log_error("SNP::SDL_IOStream file size error");
            SDL_CloseIO(io);
            return NULL;
        }

        // Allocate memory to read the file content
        char* content = (char*)malloc(length + 1);
        if (content == NULL) {
            log_error("SNP::Shader memory allocation failed");
            SDL_CloseIO(io);
            return NULL;
        }

        // Read the file content
        Sint64 bytesRead = SDL_ReadIO(io, content, length);
        if (bytesRead != length) {
            log_error("SNP::SDL_IOStream file read error");
            free(content);
            SDL_CloseIO(io);
            return NULL;
        }

        // Null-terminate the content
        content[length] = '\0';

        SDL_CloseIO(io);
        return content;
    } else {
        return NULL;
    }
}

snp_shader snp_shader_init(snp_shader_args args) {
    snp_shader shader;
    char* vertexCode = readFile(args.vertex_path);
    char* fragmentCode = readFile(args.fragment_path);

    if (!vertexCode || !fragmentCode) {
        return shader;
    }

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar* const*)&vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar* const*)&fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    checkCompileErrors(shader.ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertexCode);
    free(fragmentCode);

    return shader;
}

void snp_shader_use(snp_shader shader) {
    glUseProgram(shader.ID);
}

void snp_shader_set_mat4(snp_shader shader, const char* name, mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, (float*)mat);
}

void snp_shader_set_vec3(snp_shader shader, const char* name, vec3 vec) {
    glUniform3f(glGetUniformLocation(shader.ID, name), vec[0], vec[1], vec[2]);
}

void snp_shader_set_vec4(snp_shader shader, const char* name, vec4 vec) {
    glUniform4f(glGetUniformLocation(shader.ID, name), vec[0], vec[1], vec[2], vec[3]);
}
