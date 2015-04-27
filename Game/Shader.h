//Preprocessor directives
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#include <GL/glew.h>

class Shader
{
public:
	//Define Shader Program ID
	GLuint Program; 

	//Builds the shader from the sourcepath
	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
	{
		string vertexCode;
		string fragmentCode;

		/*----------Read from the file----------*/
		try {
		ifstream vShaderFile(vertexSourcePath);
		ifstream fShaderFile(fragmentSourcePath);
		stringstream vShaderStream, fShaderStream; 

		//Read the buffer contents into the stream
		vShaderStream << vShaderFile.rdbuf(); 
		fShaderStream << fShaderFile.rdbuf();

		//Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		//Read the stream into the GLChar array
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str(); 
		}
		catch (exception &e)
		{
			cout << "Error, Shader not successfuly read!" << endl; 
		}

		const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();

		/*-----------Compile Shaders & Error check----------*/
		GLuint vertex, fragment; 
		GLint success; 
		
		//Vertex
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex); 
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
			{
				cout << "Error, vertex shader not compiled!" << endl;
			}

		//Fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment); 
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
			{
				cout << "Error, fragment shader not compiled!" << endl;
			}

		//Link Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			cout << "Error, shader program not linked!" << endl; 
		}

		//Clean up memory
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//Function to use the program
	void use()
	{
		glUseProgram(this->Program);
	}
};

#endif