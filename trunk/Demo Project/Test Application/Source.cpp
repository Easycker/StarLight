#include <Mouse.h>

#include <Keyboard.h>

#include <FrameBuffer.h>

using namespace Math;

using namespace Render;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       GLOBAL VARIABLES                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////

// Camera Control

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.02F );

Camera * camera = NULL;

// Sphere position (only for test)

Vector3D position = Vector3D :: Zero;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                           FUNCTIONS                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}

void MouseButton ( int button, int state )
{
	mouse.StateChange ( button, state );
}

void KeyButton ( int key, int state )
{
	keyboard.StateChange ( key, state );
}

void Draw ( int width, int height, float time )
{
	{
		mouse.Apply ( camera );

		keyboard.Apply ( camera );

		camera->SetViewport ( width, height );

		camera->Setup ( );
	}
	
	glLightfv ( GL_LIGHT0, GL_POSITION, Vector4D ( 0.0F, 25.0F, 0.0F, 1.0F ) );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glDisable ( GL_LIGHTING );	

	{
		glColor3f ( 0.8F, 0.8F, 0.8F );

		glBegin ( GL_LINE_LOOP );		

		glVertex3f ( -10.0F, 0.0F, -10.0F );
		glVertex3f (  10.0F, 0.0F, -10.0F );
		glVertex3f (  10.0F, 0.0F,  10.0F );
		glVertex3f ( -10.0F, 0.0F,  10.0F );		

		glEnd ( );

		glBegin ( GL_LINE_LOOP );	

		glVertex3f ( -10.0F, 10.0F, -10.0F );
		glVertex3f (  10.0F, 10.0F, -10.0F );
		glVertex3f (  10.0F, 10.0F,  10.0F );
		glVertex3f ( -10.0F, 10.0F,  10.0F );		

		glEnd ( );

		glBegin ( GL_LINES );			

		glVertex3f ( -10.0F,  0.0F, -10.0F );
		glVertex3f ( -10.0F, 10.0F, -10.0F );
		glVertex3f (  10.0F,  0.0F, -10.0F );
		glVertex3f (  10.0F, 10.0F, -10.0F );
		glVertex3f (  10.0F,  0.0F,  10.0F );
		glVertex3f (  10.0F, 10.0F,  10.0F );
		glVertex3f ( -10.0F,  0.0F,  10.0F );
		glVertex3f ( -10.0F, 10.0F,  10.0F );	

		glEnd ( );
	}

	glEnable ( GL_LIGHTING );	

	{
		GLUquadric * quad = gluNewQuadric ( );

		gluQuadricTexture ( quad, true );  

		glTranslatef ( position.X,
			           position.Y,
					   position.Z );

		gluSphere ( quad, 2.0F, 50, 50 );

		gluDeleteQuadric ( quad );
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ENTRY POINT                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////

int main ( void )
{
	glfwInit ( );

	//---------------------------------------------------------------------------------------------

	cout << "Do you want to run program in full screen mode? [Y/N]" << endl;

	int choice = getchar ( );

	int width = 750, height = 750, mode = GLFW_WINDOW;

	//---------------------------------------------------------------------------------------------

	if ( choice == 'Y' || choice == 'y' )
	{
		GLFWvidmode vidmode;

		glfwGetDesktopMode ( &vidmode );

		width = vidmode.Width;

		height = vidmode.Height;

		mode = GLFW_FULLSCREEN;
	}

	//---------------------------------------------------------------------------------------------

	if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 16, 0, mode ) )
	{
		glfwTerminate ( ); exit ( 0 );
	}

	//---------------------------------------------------------------------------------------------

	glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );

	glfwSetMouseButtonCallback ( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------

	glEnable ( GL_DEPTH_TEST );

	glEnable ( GL_LIGHT0 );

	//---------------------------------------------------------------------------------------------
	
	camera = new Camera ( Vector3D ( -30.0F, 10.0F, -30.0F )       /* default position */,
		                  Vector3D ( 0.0F, -ONEPI / 4.0F, 0.0F )   /* default orientation */ );

	camera->SetFrustum ( );

	//---------------------------------------------------------------------------------------------

	bool running = GL_TRUE;

	int frames = 0;

	char caption [100];

	double start = glfwGetTime ( );

	//---------------------------------------------------------------------------------------------

	while ( running )
	{
		double time = glfwGetTime ( );

		if ( ( time - start ) > 1.0 || frames == 0 )
		{
			double fps = frames / ( time - start );

			sprintf_s ( caption, "Simple Particle System - %.1f FPS", fps );

			glfwSetWindowTitle ( caption );

			start = time;

			frames = 0;
		}

		frames++;

		//-----------------------------------------------------------------------------------------

		glfwGetWindowSize ( &width, &height );

		height = max ( height,  1 );

		//-----------------------------------------------------------------------------------------

		position.X = 5.0F * sinf ( 6.0F * time );
		position.Z = 5.0F * cosf ( 6.0F * time );

		Draw ( width, height, time );

		//-----------------------------------------------------------------------------------------

		glfwSwapBuffers ( );		

		running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

	glfwTerminate ( ); exit ( 0 );
}