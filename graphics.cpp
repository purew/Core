
#include <iostream>
#include "graphics.h"


#include <fstream>

#include <string>
#include <cmath>


texture allText[30];

int width, height;
int redbits, greenbits, bluebits, alphabits;
int depthbits, stencilbits;
int mode;







//--------------------------------------------
//		initWindow - uses GLFW to initiate
//		a window
//--------------------------------------------
/** Will try to initialize an OpenGL context.
 *  More details
 */
bool rendererAllmighty::initWindow(int _width, int _height, int bpp, int _mode, char* title)
{
	width	= _width;
	height	= _height;
	if (bpp%3)
		return 0;
	redbits = greenbits = bluebits = bpp/3;
	alphabits = 8;
	mode = _mode;

	// Initialize GLFW
	glfwInit();

  // V-sync
    glfwSwapInterval(1);

    int maxModes = 20;
	GLFWvidmode list[maxModes];
    int foundModes = glfwGetVideoModes( list, maxModes );
    GLFWvidmode bestMode = list[foundModes-1];

    std::cout << "Highest video mode :" << bestMode.Width << "x"<<bestMode.Height <<"\n";
    std::cout << "Bpp: " << bestMode.RedBits+bestMode.GreenBits+bestMode.BlueBits<<"\n";

	// Open window
	int ok;
	if (mode==GLFW_FULLSCREEN)
	{
        ok = glfwOpenWindow(
            bestMode.Width, bestMode.Height,          // Width and height of window
            bestMode.RedBits, bestMode.GreenBits, bestMode.BlueBits,           // Number of red, green, and blue bits for color buffer
            alphabits,                 // Number of bits for alpha buffer
            0,                // Number of bits for depth buffer (Z-buffer)
            0,                 // Number of bits for stencil buffer
            mode        // We want a desktop window (could be GLFW_FULLSCREEN)
            );
	}
	else
	{
	    ok = glfwOpenWindow(
            width,height,          // Width and height of window
            8,8,8,           // Number of red, green, and blue bits for color buffer
            alphabits,                 // Number of bits for alpha buffer
            24,                // Number of bits for depth buffer (Z-buffer)
            0,                 // Number of bits for stencil buffer
            mode        // We want a desktop window (could be GLFW_FULLSCREEN)
            );
    }

	// If we could not open a window, exit now
	if( !ok )
	{
		glfwTerminate();
		std::cout << "glfwOpenWindow returned false\n";
		return 0;
	}

	// Set window title
	glfwSetWindowTitle( title );

	// No v-sync
	glfwSwapInterval( 1 );

	// Enable sticky keys
	glfwEnable( GLFW_STICKY_KEYS );

	// Successful
	return 1;
}

//--------------------------------------------
//		setOrtho - sets up ortogonal viewing
//		2D, with blending etc
//--------------------------------------------
void rendererAllmighty::setOrtho()
{
	glEnable( GL_TEXTURE_2D );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

    int width,height;
    glfwGetWindowSize( &width, &height );

	glOrtho( 0, width, height, 0, -10, 10 );

	glMatrixMode( GL_MODELVIEW );

	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
}

void rendererAllmighty::drawRectangle( int x, int y, int w, int h, float r, float g, float b, float alpha )
{
	glDisable( GL_TEXTURE_2D );

	glLoadIdentity();
	glColor4f( r, g, b, alpha );

	glBegin( GL_QUADS );
		glVertex3f( x, y, 0.1 );
		glVertex3f( x+w, y, 0.1 );
		glVertex3f( x+w, y+h, 0.1 );
		glVertex3f( x, y+h, 0.1 );
	glEnd();

	glEnable( GL_TEXTURE_2D );
}


void rendererAllmighty::swapBuffers()
{


	// Swap front and back buffers (we use a double buffered display)
//	glFinish();

	glfwSwapBuffers();





	int width, height;
	glfwGetWindowSize( &width, &height );
	if ( height < 1 )
		height = 1;





	// Clear color and depht buffers
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	glLoadIdentity();
    glTranslatef( 0.375, 0.375, 0.0 );

	glfwSleep(0.01);




}


//--------------------------------------------
//		loadTexture - loads file as a texture of
//		type type.
//--------------------------------------------

grError rendererAllmighty::loadTexture( char* file, gType type, texture* text )
{

    std::stringstream out;
    out << "Loading " << file << "\n";
//    coLogg( out);

	// Test if the file exists
	std::ifstream fin( file, std::ios_base::in );
	if ( !fin.is_open() )
	{
	    std::stringstream out;
	    out << file << " does not exist\n";
		std::cout << out;
//		coLogg( out);
		return NO_SUCH_FILE;
	}
	fin.close();

	// Do we have an object?
	if (text == 0)
		text = new texture;

	if (type == TGA)
	{
		// Start generating the texture
		glGenTextures( 1, &(text->data) );
		glBindTexture( GL_TEXTURE_2D, text->data );

		// Is it a TGA-file? Open with glfw
		GLFWimage img;
		int result = glfwReadImage( file, &img, GLFW_NO_RESCALE_BIT );
		if ( result == GL_FALSE )
		{
			std::cout << "glfwReadImage failed\n";
			return GLFW_ERROR;
		}
		if ( &img == 0 )
		{
			std::cout << "glfwReadImage error\n";
			return GLFW_ERROR;
		}
		text->width = img.Width;
		text->height = img.Height;

		glBindTexture( GL_TEXTURE_2D, text->data );
		int err = gluBuild2DMipmaps(GL_TEXTURE_2D,
									img.BytesPerPixel,
									img.Width,
									img.Height,
									GL_RGB,
									GL_UNSIGNED_BYTE,
									img.Data);
		if ( err ) {
		     std::stringstream out;
            out << file << "Couldn't load " << file << " due to: " << gluErrorString( err ) << "\n";
            std::cout << out;
//            coLogg( out);
			return GLUT_ERROR;	//gluBuild failed, return false
		}
		glfwFreeImage( &img );


		result = glfwLoadTexture2D( file, 0);
		if (result == GL_FALSE)
			return GLFW_ERROR;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		return NO_ERROR;
	}

	if (type==PNG)
	{
		pngInfo info;
		GLuint id = pngBind(file, PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

		if (id != 0)
		{
			text->data = id;
			text->width = info.Width;
			text->height = info.Height;
			text->alpha = info.Alpha;

			return NO_ERROR;
		}
		else return GLPNG_ERROR;
	}

	return NO_SUPPORTED_FORMAT;
}

void rendererAllmighty::drawSprite( texture* sprite, int x, int y, float rotation, float blended )
{
	if (glfwGetKey(GLFW_KEY_ENTER))
		std::cout << "2. drawSprite: rot: " << rotation << "\n";
	drawSprite( sprite, x, y, sprite->width, sprite->height, rotation, blended );
}

void rendererAllmighty::drawSprite( texture* sprite, int x, int y)
{
	drawSprite( sprite, x, y, sprite->width, sprite->height, 0.0, 1.0 );
}


void rendererAllmighty::drawSprite( texture* sprite, int x, int y, int w, int h, float rotation, float blended )
{

    float colour=1;
    glColor4f( colour, colour, colour, blended );
	glBindTexture(GL_TEXTURE_2D, sprite->data);

    float wsin = w/2*sin(rotation);
    float wcos = w/2*cos(rotation);
    float hsin = h/2*sin(rotation);
    float hcos = h/2*cos(rotation);


	glBegin( GL_QUADS );
        glTexCoord2f(0.0, 0.0);	glVertex2f( x-wcos+hsin, y-wsin-hcos );	// lower left
        glTexCoord2f(1.0, 0.0);	glVertex2f( x+wsin+hcos, y-wcos+hsin );	// lower right
        glTexCoord2f(1.0, 1.0);	glVertex2f( x+wcos-hsin, y+wsin+hcos ); 	// upper right
		glTexCoord2f(0.0, 1.0);	glVertex2f( x-wsin-hcos, y+wcos-hsin ); 	// upper left
	glEnd();

}


void rendererAllmighty::drawSprite( texture* sprite, int x, int y, int w, int h)
{
    float colour=1;
    glColor4f( colour, colour, colour, 1.0 );
	glBindTexture(GL_TEXTURE_2D, sprite->data);




	glBegin( GL_QUADS );
        glTexCoord2f(0.0, 0.0);	glVertex2f( x, y );	// lower left
        glTexCoord2f(1.0, 0.0);	glVertex2f( x+w, y );	// lower right
        glTexCoord2f(1.0, 1.0);	glVertex2f( x+w, y+h ); 	// upper right
		glTexCoord2f(0.0, 1.0);	glVertex2f( x, y+h ); 	// upper left
	glEnd();

}








