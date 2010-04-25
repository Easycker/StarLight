#include <iostream>
#include <cl/cl.h>
#include "oclUtil.h"
using namespace std;

void oclPrintPlatformDescription( cl_platform_id platform )
{
	if ( platform == NULL )
	{
		return;
	}

	oclPrintPlatformInfo( platform, CL_PLATFORM_NAME, "Platform name: " );
	oclPrintPlatformInfo( platform, CL_PLATFORM_VENDOR, "Platform vendor: " );
	oclPrintPlatformInfo( platform, CL_PLATFORM_VERSION, "Platform version: " );
	oclPrintPlatformInfo( platform, CL_PLATFORM_EXTENSIONS, "Platform extensions: " );
}

void oclPrintDeviceDescription ( cl_device_id device )
{
	if ( device == NULL )
	{
		return;
	}
	const unsigned int infoSize = 500;

	char info[infoSize];

	cl_int status;

	status = clGetDeviceInfo(
		device           /* device */,
		CL_DEVICE_NAME   /* param_name */,
		infoSize         /* param_value_size */,
		info             /* param value */,
		NULL             /* param_value_size_ret */ ); 

	oclCheckError( status );

	cout << info << endl;
}

void oclPrintPlatformInfo (cl_platform_id platform, cl_platform_info paramName, char* paramNameStr )
{
	cl_int status;

	const unsigned int infoSize = 500;

	char info [infoSize];

	size_t infoSizeRet;

	status = clGetPlatformInfo( 
		platform       /* platform */, 
		paramName      /* param_name */, 
		infoSize       /* param_value_size */, 
		(void*) info   /* param_value */, 
		&infoSizeRet   /* param_value_size_ret */ );

	oclCheckError( status );

	if ( paramName == CL_PLATFORM_EXTENSIONS )
	{
		int lastNewLine = 0;

		for ( cl_uint i = 0; i < infoSizeRet; i++ )
		{
			if ( info[i] == ' ' && lastNewLine != i - 1 ) 
			{
				info[i] = '\n';
			}
			if ( info[i] == '\n' )
			{
				lastNewLine = i;
			}
		}
	}

	if ( infoSizeRet == infoSize )
	{
		info[infoSize] = 0;
	}

	cout << paramNameStr << info << endl;
}

void oclPrintBuildLog(cl_device_id device, cl_program program)
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

	//cout << endl << endl << endl << source;

	free ( buildLog );

	cout << "ERROR! clBuildProgram failed" << endl; exit ( -1 );
}

void oclCheckError(cl_int errorCode)
{
	static int count = 0;

	if ( errorCode != CL_SUCCESS )
	{
		cout << endl;
		cout << "Error " << errorCode << " at " << count;
		cout << endl;
		exit ( -1 );
	}

	count ++;
}