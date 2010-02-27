#include <iostream>
#include <GL/glfw.h>
#include "graphics.h"
#include "GLFT_Font.hpp"
#include <fstream>

int main()
{

	rendererAllmighty renderer;
    renderer.initWindow(800,600,24,GLFW_WINDOW, "CORE");
    renderer.setOrtho();

	std::ofstream outfile("testfil");
	if (outfile.is_open() == 0 )
		std::cout << "it's not open either\n";
	outfile << "bleeh";
	outfile.close();


	std::ifstream file( "tank.png", std::ios::binary );
    if ( file.is_open() == false)
		std::cout << "it's not open\n";

	GLFT_Font normalFont("binreg.ttf",24);

    std::cout << "Entering main-loop\n";
    while (!glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED ))
    {
    	normalFont.drawText(10,10, "Testar att printa");

		renderer.swapBuffers();

    }

    glfwTerminate();

    return 0;
}
