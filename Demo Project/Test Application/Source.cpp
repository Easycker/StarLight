#include <Mouse.h>

#include "opencl.h"

#include <Keyboard.h>

#include <FrameBuffer.h>

#include <iostream>

#include <fstream>

#include <conio.h>

using namespace std;

using namespace Math;

using namespace Render;

#define USE_TEXTURE

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       GLOBAL VARIABLES                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////

// Camera Control

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.2F );

Camera * camera = NULL;
Texture2D* openGLTexture = NULL;

int width = 512, height = 512, mode = GLFW_WINDOW;


// Sphere position (only for test)

Vector3D position = Vector3D :: Zero;
size_t groupSizeX = 1;										/* Work-Group size ( for CPU = 1 ) */
size_t groupSizeY = 64;
//-----------------------------------------------------------------------------------------------------------

cl_mem clMemTexture;	

cl_context context;											/* CL context */

cl_device_id device;										/* CL device */

cl_program program;											/* CL program */

cl_kernel kernel;											/* CL kernel */

cl_command_queue commandQueue;								/* CL command queue */

//-----------------------------------------------------------------------------------------------------------

size_t maxWorkGroupSize;									/* Max allowed work-items in a group */

cl_uint maxDimensions;										/* Max group dimensions allowed */

size_t * maxWorkItemSizes;									/* Max work-items sizes in each dimensions */

cl_ulong totalLocalMemory;									/* Max local memory allowed */

cl_ulong usedLocalMemory;									/* Used local memory */

size_t kernelWorkGroupSize;									/* Group size returned by kernel */

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
	glBegin ( GL_QUADS );

		glTexCoord2f(0.0f, 0.0f); glVertex2f ( -1.0F, -1.0F );
		glTexCoord2f(0.0f, 1.0f); glVertex2f ( -1.0F,  1.0F );
		glTexCoord2f(1.0f, 1.0f); glVertex2f (  1.0F,  1.0F );   
		glTexCoord2f(1.0f, 0.0f); glVertex2f (  1.0F, -1.0F );

	glEnd ( );
}
char * LoadFile ( const char * path, long *outLength = NULL )
{
	ifstream file ( path , ios_base::binary );

	//-----------------------------------------------------------------

	if ( !file )
	{
		cout << "ERROR: Could not open file" << endl; exit ( -1 );
	}

	//-----------------------------------------------------------------

	file.seekg ( 0, ios :: end );

	unsigned long length = file.tellg ( );

	if (outLength)
	{
		*outLength = static_cast<long>(length);
	}

	file.seekg ( 0, ios :: beg );

	//-----------------------------------------------------------------

	if ( 0 == length )
	{
		cout << "WARNING: File is empty" << endl;
	}

	//-----------------------------------------------------------------

	char * source = new char [length + 1];

	file.read( source , length );

//	unsigned long i = 0;

//	while ( file )
//	{
//		source [i++] = file.get ( );
//	}

	source[ length ] = 0;

	return source;
}

cl_int SetupOpenCL ( cl_int deviceType )
{
	/*
	* Have a look at the available platforms and pick either
	* the AMD/NVIDIA one if available or a reasonable default.
	*/

	cl_int status = CL_SUCCESS;

	cl_uint numPlatforms = 0;

	char extensions[200];

	status = clGetPlatformIDs ( 0               /* num_entries */,
		NULL            /* platforms */,    
		&numPlatforms   /* num_platforms */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetPlatformIDs failed" << endl; exit ( -1 );
	}

	cl_platform_id platform = NULL;

	if ( 0 < numPlatforms ) 
	{
		cl_platform_id * platforms = new cl_platform_id [numPlatforms];

		status = clGetPlatformIDs ( numPlatforms   /* num_entries */,
			platforms      /* platforms */,
			NULL           /* num_platforms */ );

		if ( status != CL_SUCCESS )
		{
			cout << "ERROR! clGetPlatformIDs failed" << endl; exit ( -1 );
		}

		for ( unsigned i = 0; i < numPlatforms; ++i ) 
		{
			char info [100];

			status = clGetPlatformInfo ( platforms [i]        /* platform */,
				CL_PLATFORM_VENDOR   /* param_name */,
				sizeof ( info )      /* param_value_size */,
				info                 /* param_value */,
				NULL                 /* param_value_size_ret */ );

			if ( status != CL_SUCCESS )
			{
				cout << "ERROR! clGetPlatformInfo failed" << endl; exit ( -1 );
			}
			cout << endl << "Platform vendor: " << info <<endl;

			platform = platforms [i];
			status = clGetPlatformInfo(platform,
				CL_PLATFORM_EXTENSIONS, 
				sizeof(extensions),
				extensions,
				NULL);
			if ( status != CL_SUCCESS )
			{
				cout << "ERROR! clGetPlatformInfo failed" << endl; exit ( -1 );
			}
			cout << endl << "Platform " << i << " extensions: " << extensions <<endl;
		}

		delete [] platforms;
	}

	cl_context_properties cprops[] = {

		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,

		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),

		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),

		0

	};


	// Create a context for the given type of the device.

	context = clCreateContextFromType ( cprops       /* properties */,
		deviceType   /* device_type */,
		NULL         /* pfn_notify */,
		NULL         /* user_data */,
		&status      /* errcode_ret */);
	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clCreateContextFromType failed" << endl; exit ( -1 );
	}
	/*
	* Get all devices in the given context.
	*/

	size_t infoSize = 0;

	status = clGetContextInfo ( context              /* context */,
		CL_CONTEXT_DEVICES   /* param_name */,
		0                    /* param_value_size */,
		NULL                 /* param_value */,
		&infoSize            /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetContextInfo failed" << endl; exit ( -1 );
	}

	if ( infoSize > 0 )
	{
		cl_device_id* ids = ( cl_device_id * ) alloca ( infoSize );

		status = clGetContextInfo ( context              /* context */,
			CL_CONTEXT_DEVICES   /* param_name */,
			infoSize             /* param_value_size */,
			ids                  /* param_value */,
			NULL                 /* param_value_size_ret */ );

		if ( status != CL_SUCCESS )
		{
			cout << "ERROR! clGetContextInfo failed" << endl; exit ( -1 );
		}
		device = ids [0];
		cout << "Device 0 id: " << device << endl;
		status = clGetDeviceInfo(device, 
			CL_DEVICE_EXTENSIONS,
			sizeof(extensions),
			extensions,
			NULL);
		if(status != CL_SUCCESS)
		{
			cout << "ERROR! Can not get info on supported extensions!" << endl;
		}
		else
		{
			cout << "Supported extensions: " << extensions << endl;
		}
	}

	if ( device == NULL )
	{
		cout << "ERROR! Can not find device!" << endl; exit ( -1 );
	}

	/* Create command queue */

	commandQueue = clCreateCommandQueue ( context   /* context */,
		device    /* device */,
		0         /* properties */,
		&status   /* errcode_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clCreateCommandQueue failed" << endl; exit ( -1 );
	}

	/*
	* Get basic info about selected device.
	*/

	status = clGetDeviceInfo ( 
		device                          /* device */,
		CL_DEVICE_MAX_WORK_GROUP_SIZE   /* param_name */,
		sizeof ( size_t )               /* param_value_size */,
		( void * ) &maxWorkGroupSize    /* param_value */,
		NULL                            /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE failed" << endl; exit ( -1 );
	}

	status = clGetDeviceInfo (
		device                               /* device */,
		CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS   /* param_name */,
		sizeof ( cl_uint )                   /* param_value_size */,
		( void * ) &maxDimensions            /* param_value */,
		NULL                                 /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS failed" << endl; exit ( -1 );
	}

	maxWorkItemSizes = ( size_t * ) malloc ( maxDimensions * sizeof ( size_t ) );

	status = clGetDeviceInfo (
		device                              /* device */,
		CL_DEVICE_MAX_WORK_ITEM_SIZES       /* param_name */,
		sizeof ( size_t ) * maxDimensions   /* param_value_size */,
		( void * ) maxWorkItemSizes         /* param_value */,
		NULL                                /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_SIZES failed" << endl; exit ( -1 );
	}

	status = clGetDeviceInfo (
		device                         /* device */,
		CL_DEVICE_LOCAL_MEM_SIZE       /* param_name */,
		sizeof ( cl_ulong )            /* param_value_size */,
		( void * ) &totalLocalMemory   /* param_value */,
		NULL                           /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetDeviceInfo CL_DEVICE_LOCAL_MEM_SIZE failed" << endl; exit ( -1 );
	}

	/*
	* Create and initialize memory objects.
	*/

#ifdef USE_TEXTURE

	clMemTexture = clCreateFromGLTexture2D(context,
		CL_MEM_WRITE_ONLY,
		GL_TEXTURE_2D,
		0,
		openGLTexture->GetHandle(),
		&status);

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clCreateBuffer failed" << endl; exit ( -1 );
	}

	status = clEnqueueAcquireGLObjects(commandQueue, 1, &clMemTexture, 0, NULL, NULL);
	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clEnqueueAcquireGLObjects failed" << endl; exit ( -1 );
	}

#else

	clMemTexture = clCreateBuffer (
		context                                   /* context */,
		CL_MEM_WRITE_ONLY    /* flags */,
		width * height * sizeof ( cl_float4 )       /* size */,
		NULL                               /* host_ptr */,
		&status                                   /* errcode_ret */ );
#endif
	/*
	* Create a CL program using the kernel source.
	*/

	const char * source = LoadFile ( "render.cl" );

	size_t sourceSize [] = { strlen ( source ) };

	program = clCreateProgramWithSource (
		context      /* context */,
		1            /* count */,
		&source      /* strings */,
		sourceSize   /* lengths */,
		&status      /* errcode_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clCreateProgramWithSource failed" << endl; exit ( -1 );
	}

	/* Create a cl program executable for all the devices specified */

	status = clBuildProgram (
		program   /* program */,
		1         /* num_devices */,
		&device   /* device_list */,
		NULL      /* options */,
		NULL      /* pfn_notify */,
		NULL      /* user_data */ );

	if ( status != CL_SUCCESS )
	{
		size_t buildLogSize = 0;

		cl_int logStatus = clGetProgramBuildInfo (
			program                /* program */, 
			device                 /* device */, 
			CL_PROGRAM_BUILD_LOG   /* param_name */, 
			0                      /* param_value_size */, 
			NULL                   /* param_value */, 
			&buildLogSize          /* param_value_size_ret */ );

		if ( logStatus != CL_SUCCESS )
		{
			cout << "ERROR! clGetProgramBuildInfo failed" << endl; exit ( -1 );
		}

		char * buildLog = ( char * ) malloc ( buildLogSize );

		memset ( buildLog, 0, buildLogSize );

		logStatus = clGetProgramBuildInfo (
			program                /* program */, 
			device                 /* device */, 
			CL_PROGRAM_BUILD_LOG   /* param_name */, 
			buildLogSize           /* param_value_size */, 
			buildLog               /* param_value */, 
			NULL                   /* param_value_size_ret */ );

		if ( logStatus != CL_SUCCESS )
		{
			cout << "ERROR! clGetProgramBuildInfo failed" << endl; free ( buildLog ); exit ( -1 );
		}

		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                        PROGRAM BUILD INFO                        +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		cout << buildLog << endl;

		cout << endl << endl << endl << source;

		free ( buildLog );

		_getch();

		cout << "ERROR! clBuildProgram failed" << endl; exit ( -1 );
	}

	/* Get a kernel object handle for a kernel with the given name */

	kernel = clCreateKernel ( program       /* program */,
		"main"   /* kernel_name */,
		&status       /* errcode_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clCreateKernel failed" << endl; exit ( -1 );
	}

	return status;
}
cl_int ReleaseOpenCL ( void )
{
	cl_int result;
	result = clReleaseKernel(kernel);
	if(result != CL_SUCCESS)
	{
		return result;
	}
	result = clReleaseProgram(program);
	if(result != CL_SUCCESS)
	{
		return result;
	}

#ifdef USE_TEXTURE

	result = clEnqueueReleaseGLObjects(commandQueue,
		1,
		&clMemTexture,
		0,
		NULL,
		NULL);
	if(result != CL_SUCCESS)
	{
		return result;
	}

#endif

	result = clReleaseMemObject(clMemTexture);
	if(result != CL_SUCCESS)
	{
		return result;
	}
	result = clReleaseCommandQueue(commandQueue);
	if(result != CL_SUCCESS)
	{
		return result;
	}
	result = clReleaseContext(context);
	if(result != CL_SUCCESS)
	{
		return result;
	}
	return result;

}
cl_int SetupKernels ( void )
{
	cl_int status = CL_SUCCESS;

	/*
	* Set appropriate arguments to the kernel.
	*/

	status = clSetKernelArg (
		kernel                   /* kernel */,
		0                        /* arg_index */,
		sizeof ( cl_mem )        /* arg_size */,
		( void * ) &clMemTexture /* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}

	status = clGetKernelWorkGroupInfo (
		kernel                     /* kernel */,
		device                     /* device */,
		CL_KERNEL_LOCAL_MEM_SIZE   /* param_name */,
		sizeof ( cl_ulong )        /* param_value_size */,
		&usedLocalMemory           /* param_value */,
		NULL                       /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetKernelWorkGroupInfo CL_KERNEL_LOCAL_MEM_SIZE failed" << endl; exit ( -1 );
	}

	if ( usedLocalMemory > totalLocalMemory )
	{
		cout << "Unsupported: Insufficient local memory on device." << endl; exit ( -1 );
	}

	cout << "Used local memory: " << usedLocalMemory << "/" << totalLocalMemory << endl; 

	/* Check group size against group size returned by kernel */

	status = clGetKernelWorkGroupInfo (
		kernel                      /* kernel */,
		device                      /* device */,
		CL_KERNEL_WORK_GROUP_SIZE   /* param_name */,
		sizeof ( size_t )           /* param_value_size */,
		&kernelWorkGroupSize        /* param_value */,
		0                           /* param_value_size_ret */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clGetKernelWorkGroupInfo CL_KERNEL_COMPILE_WORK_GROUP_SIZE failed" << endl; exit ( -1 );
	}

	if ( groupSizeX * groupSizeY > kernelWorkGroupSize )
	{
		cout << "Out of Resources!" << endl;		
		cout << "Group Size specified : " << groupSizeX << endl;
		cout << "Max Group Size supported on the kernel : " << kernelWorkGroupSize << endl;
		cout << "Falling back to " << kernelWorkGroupSize << endl;

		groupSizeX = kernelWorkGroupSize;
		groupSizeY = 1;
	}

	return CL_SUCCESS;
}

cl_int StartKernels ( void )
{
	cl_int status = CL_SUCCESS;

	cl_event events [1];

	size_t globalThreads [] = { width, height };

	size_t localThreads [] = { groupSizeX, groupSizeY };

	if ( localThreads [0] > maxWorkItemSizes [0] || localThreads [0] > maxWorkGroupSize )
	{
		cout << "Unsupported: Device does not support requested number of work items" << endl; exit ( -1 );
	}

	cl_float4 temp;
	Vector3D tempVector = camera->GetPosition();
	temp.s[0] = tempVector.X;
	temp.s[1] = tempVector.Y;
	temp.s[2] = tempVector.Z;
	temp.s[3] = 0;
	status = clSetKernelArg (
		kernel                   /* kernel */,
		1                        /* arg_index */,
		sizeof ( cl_float4 )        /* arg_size */,
		( void * ) temp.s			/* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}

	tempVector = camera->GetSide();
	temp.s[0] = tempVector.X;
	temp.s[1] = tempVector.Y;
	temp.s[2] = tempVector.Z;
	temp.s[3] = 0;
	status = clSetKernelArg (
		kernel                   /* kernel */,
		2                        /* arg_index */,
		sizeof ( cl_float4 )        /* arg_size */,
		( void * ) temp.s /* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}

	tempVector = camera->GetUp();
	temp.s[0] = tempVector.X;
	temp.s[1] = tempVector.Y;
	temp.s[2] = tempVector.Z;
	temp.s[3] = 0;
	status = clSetKernelArg (
		kernel                   /* kernel */,
		3                        /* arg_index */,
		sizeof ( cl_float4 )        /* arg_size */,
		( void * ) temp.s /* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}

	tempVector = camera->GetView();
	temp.s[0] = tempVector.X;
	temp.s[1] = tempVector.Y;
	temp.s[2] = tempVector.Z;
	temp.s[3] = 0;
	status = clSetKernelArg (
		kernel                   /* kernel */,
		4                        /* arg_index */,
		sizeof ( cl_float4 )        /* arg_size */,
		( void * ) temp.s /* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}	

	Vector2D tempVector2D;
	tempVector2D = camera->GetScreenScale();
	cl_float2 temp2;
	temp2.s[0] = tempVector2D.X;
	temp2.s[1] = tempVector2D.Y;
	status = clSetKernelArg (
		kernel                   /* kernel */,
		5                        /* arg_index */,
		sizeof ( cl_float2 )     /* arg_size */,
		( void * )temp2.s			/* arg_value */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clSetKernelArg #0 failed" << endl; exit ( -1 );
	}
	status = clEnqueueNDRangeKernel (
		commandQueue    /* command_queue */,
		kernel          /* kernel */,
		2               /* work_dim */,
		NULL            /* global_work_offset */,
		globalThreads   /* global_work_size */,
		localThreads    /* local_work_size */,
		0               /* num_events_in_wait_list */,
		NULL            /* event_wait_list */,
		NULL            /* event */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clEnqueueNDRangeKernel failed" << endl; exit ( -1 );
	}

	status = clFinish ( commandQueue );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clFinish failed" << endl; exit ( -1 );
	}
	//* Enqueue read buffer */

#ifndef USE_TEXTURE

	status = clEnqueueReadBuffer (
		commandQueue                          /* command_queue */,
		clMemTexture					 /* buffer */,
		CL_TRUE                               /* blocking_read */,
		0                                     /* offset */,
		width* height * sizeof ( cl_float4 )   /* cb */,
		*(openGLTexture->Data)                          /* ptr */,
		0                                     /* num_events_in_wait_list */,
		NULL                                  /* event_wait_list */,
		&events [0]                           /* event */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clEnqueueReadBuffer failed" << endl; exit ( -1 );
	}

	/* Wait for the read buffer to finish execution */

	status = clWaitForEvents ( 1             /* num_events */,
		events								 /* event_list */ );

	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! clWaitForEvents failed" << endl; exit ( -1 );
	}

	clReleaseEvent ( events [0] );

#endif

	return CL_SUCCESS;
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

	glEnable (GL_TEXTURE_2D);

	//---------------------------------------------------------------------------------------------

	TextureData2D textureData(width, height, 4);
	Texture2D texture(&textureData);
	openGLTexture = &texture;
	openGLTexture->Setup();
	//Drawing of a green cross
	for(int i = 0; i < width*height; i++)
	{
		if(i % width == i / height || width - i % width == i / height)
		{
			(*(openGLTexture->Data))[4*i]   = 0.0f;
			(*(openGLTexture->Data))[4*i+1] = 1.0f;
			(*(openGLTexture->Data))[4*i+2] = 0.0f;
		}

		(*(openGLTexture->Data))[4*i+3] = 1.0f;
	}
	openGLTexture->Update();

	camera = new Camera ( Vector3D ( -10.0F, 0.0F, -10.0F )       /* default position */,
		                  Vector3D ( 0.0F, -ONEPI / 4.0F, 0.0F )   /* default orientation */ );

	camera->SetFrustum ( );
	camera->SetViewport( width , height );

	SetupOpenCL(CL_DEVICE_TYPE_GPU);
	SetupKernels();

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

		mouse.Apply ( camera );
		keyboard.Apply ( camera );
		StartKernels();

#ifndef USE_TEXTURE

		openGLTexture->Update();

#endif
		//cout<<camera->GetPosition().X<<" "<<camera->GetPosition().Y<<" "<<camera->GetPosition().Z<<" "<<camera->GetView().X<<" "<<camera->GetView().Y<<" "<<camera->GetView().Z<<" "<<endl;
		Draw ( width, height, time );

		//-----------------------------------------------------------------------------------------

		glfwSwapBuffers ( );		

		running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}
	ReleaseOpenCL();
	glfwTerminate ( ); exit ( 0 );
}