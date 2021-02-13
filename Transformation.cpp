// Triangle.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>

//inclusion de librerias
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Tamano de la ventana OpenGL
const GLint WIDTH = 600, HEIGHT = 600;

//Declaracion del Vertex Shader dentro del clase principal
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 transform;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() \n "
"{\n"
"	gl_Position = model*vec4(position, 1.0);\n"
"}\0";

// Declaracion del FragmentShader dentro de la clase principal 
// Define color del triangulo
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main() \n "
"{\n"
"	color = vec4(1.0f, 0.5f, 0.7f, 1.0f);\n" //Colores normalizados 0-1
"}\0";

int main()
{
	glfwInit();//Inicializador de OpenGL

	//Parametros de configuracion de OpenGL de la version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version maxima de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Version minima de OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Habilita el redimencionado de la ventana

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Moto", nullptr, nullptr); //Creacion de la ventana GLFW para OpenGL

	int screenWidth, screenHeight; //Variables enteras
 
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight); //Guarda el ancho y el alto de la ventana GLFW en las variables anteriormente creadas

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	//Initialize the GLEW to setup the OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initilize the GLEW" << std::endl;

		glfwTerminate();

		return EXIT_FAILURE;
	}
	// Define el viewport and sus dimensiones
	glViewport(0, 0, screenWidth, screenHeight);

	// Compilacion del vectorShader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Se crea una variable sucess y un Char infoLog y Guarda en infoLog el estatus de la compilacion del vectorShader y FragmentShader
	GLint success;
	GLchar infoLog[512]; //Variable of infoLog

	// Lineas de codigo para mostrar errores en la compilacion del vertexShader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Compilacion del FragmentShader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Graba en infoLog el estado de la compilacion del fragmentShader 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//Se crea un ShaderProgram, contiene adjunto los shaders de Vertex y Fragment para la tuberia de gráfico
	GLuint shaderProgram = glCreateProgram(); //Creacion de shaderProgram
	glAttachShader(shaderProgram, vertexShader); //Adjunta el VertexShader
	glAttachShader(shaderProgram, fragmentShader); //Adjunta el FragmentShader
	glLinkProgram(shaderProgram);
	
	// Graba en infoLog el estado de la compilacion del SahderProgram
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	//Borra los VertexShader y FragmentShader de la memoria para liberar espacio, porque ya no se usan durante la compilacion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Vertices del triangulo de manera normalizados
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, //bottom left
		 0.5f, -0.5f, 0.0f, //bottom right
		 0.0f,  0.5f, 0.0f //up center
	};

	GLuint VBO, VAO; //Declaracion de variables entero signadas VBO (VertexBufferObjet) y VAO (VertexArrayObject)
	glGenVertexArrays(1, &VAO); //Genero el 1 objeto de Arreglo de Vertices y se le asigna a VAO 
	glGenBuffers(1, &VBO); //Genero el 1 objeto de Buffer de Vertices y se le asigna a VBO

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat ), (GLvoid *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	GLfloat aspectRatio = screenWidth / screenHeight;
	glm::mat4 projection;
	projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (Keypressed, mouse moved etc.) and call corresponding response 
		// function
		glfwPollEvents();

		// Render
		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram); //Invoca el shaderProgram creado llamado "shaderProgram"

		//glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		//GLint projectionUniform = glGetUniformLocation(shaderProgram, "projection");
		//glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glm::mat4 view;
		//transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		model = glm::rotate(model,  0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(screenWidth/2, screenHeight/2, 0.0f));//Used with ortho projection

		GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
		//GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
		//GLint projLocation = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));


		//Dibujar
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); //Metodo para dibujar
		glBindVertexArray(0);

		// draw openGL stuff

		// Swap the screen buffer
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO); //Borra el Objeto de Arreglo de Vertices
	glDeleteBuffers(1, &VBO); //Borra el Buffer de objeto que en este caso contiene al triangulo

	glfwTerminate(); //Finaliza la ventana GLFW

	return EXIT_SUCCESS; //Finaliza la rutina satisfactoriamente.
}

