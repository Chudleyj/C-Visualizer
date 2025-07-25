#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	unsigned int ID; 


}Shader; 

char* getShaderCode(const char* path);
Shader shader_create(const char* vertexPath, const char* fragmentPath); 
void shader_deleteProg(Shader shader);
void shader_use(Shader shader); 
void shader_setBool(Shader shader, const char* name, bool value); 
void shader_setInt(Shader shader, const char* name, int value);
void shader_setFloat(Shader shader, const char* name, float value);

void shader_checkCompileErrors(unsigned int shaderID, const char* type);

#endif