#include <math.h>

#include "RadixSort.h"

#include "oclUtil.h"

cl_context context;					/* CL context */

cl_device_id device;				/* CL device */

cl_command_queue commandQueue;		/* CL command queue */

size_t maxWorkGroupSize;			/* Max allowed work-items in a group */

cl_uint maxDimensions;				/* Max group dimensions allowed */

size_t * maxWorkItemSizes;			/* Max work-items sizes in each dimensions */

cl_ulong totalLocalMemory;			/* Max local memory allowed */

cl_ulong usedLocalMemory;			/* Used local memory */

cl_int SetupOpenCL ( cl_device_type deviceType = CL_DEVICE_TYPE_GPU );

void generateUnsortedArray( cl_float * unsortedArray, cl_uint arraySize)
{
	for ( cl_uint i = 0; i < arraySize * 4; i++ )
	{
		//unsortedArray[i*4.0F + 0] = sin(i)*i;
		//unsortedArray[i*4.0F + 1] = cos(i)*i;
		//unsortedArray[i*4.0F + 2] = tan(i)*i;
		//unsortedArray[i*4.0F + 3] = i;
		unsortedArray[ i ] = (float)arraySize * 4.0F - (float)i;
	}
}

int main ( )
{
	cl_int status;

	SetupOpenCL( );

	cl_uint arraySize = 256*2;

	cl_float * unsortedArray = new cl_float [ 4 * arraySize ];

	generateUnsortedArray ( unsortedArray, arraySize );

	//for ( cl_uint i = 0; i < arraySize; i++ )
	//{
	//	cout << unsortedArray[ i*4 + 0 ] << "\t";
	//	cout << unsortedArray[ i*4 + 1 ] << "\t";
	//	cout << unsortedArray[ i*4 + 2 ] << "\t";
	//	cout << unsortedArray[ i*4 + 3 ] << "\t";
	//	cout << endl;
	//}

	cout << "---------------------------------";

	cl_mem clUnsortedArray = clCreateBuffer (
		context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		arraySize * sizeof( cl_float4 ),
		unsortedArray,
		&status);
	oclCheckError( status );

	cl_mem clSortedArray = clCreateBuffer (
		context,
		CL_MEM_READ_WRITE,
		arraySize * sizeof( cl_float4 ),
		NULL,
		&status
		);
	oclCheckError( status );

	oclRadixSort mySorter( device, context, commandQueue, maxWorkItemSizes[0] );

	mySorter.StartSort( clUnsortedArray, clSortedArray, arraySize );

	cl_event events [1];

	status = clEnqueueReadBuffer (
		commandQueue                             /* command_queue */,
		clUnsortedArray                          /* buffer */,
		CL_TRUE                                  /* blocking_read */,
		0                                        /* offset */,
		arraySize*sizeof(cl_float4)              /* cb */,
		unsortedArray                            /* ptr */,
		0                                        /* num_events_in_wait_list */,
		NULL                                     /* event_wait_list */,
		&events [0]                              /* event */ 
	);

	oclCheckError( status );

	status = clWaitForEvents (
		1             /* num_events */,
		events        /* event_list */ 
		);

	oclCheckError( status );

	clReleaseEvent ( events [0] );

	//for ( cl_uint i = 0; i < arraySize; i++ )
	//{
	//	cout << unsortedArray[ i*4 + 0 ] << "\t";
	//	cout << unsortedArray[ i*4 + 1 ] << "\t";
	//	cout << unsortedArray[ i*4 + 2 ] << "\t";
	//	cout << unsortedArray[ i*4 + 3 ] << "\t";
	//	cout << endl;
	//}

	delete[] unsortedArray;
}


cl_int SetupOpenCL ( cl_device_type deviceType )
{
	/*
	* Have a look at the available platforms and pick either
	* the AMD/NVIDIA one if available or a reasonable default.
	*/

	cl_int status = CL_SUCCESS;

	cl_uint numPlatforms = 0;

	/* Get Platforms */
	//--------------------------------------------------------------------------
	status = clGetPlatformIDs ( 
		0               /* num_entries   */,
		NULL            /* platforms     */,    
		&numPlatforms   /* num_platforms */ );

	oclCheckError( status );

	if ( numPlatforms == 0 )
	{
		cout << "ERROR! There are no platforms found!" << endl; exit ( -1 );
	}

	cl_platform_id * platforms = new cl_platform_id [numPlatforms];

	status = clGetPlatformIDs( 
		numPlatforms   /* num_entries   */,
		platforms      /* platforms     */,
		NULL           /* num_platforms */ );

	/* Get IDs for each platform and  */
	//--------------------------------------------------------------------------
	cl_device_id LastDevice = 0;

	cl_device_id ** devices = new cl_device_id* [numPlatforms];

	cl_uint * numDevices = new cl_uint [numPlatforms];

	for ( cl_uint i = 0; i < numPlatforms; i++ )
	{
		status = clGetDeviceIDs (
			platforms[i]     /* platform */,
			deviceType       /* device_type */,
			NULL             /* num_entries*/,
			NULL             /* devices*/,
			&numDevices[i]   /* num_devices*/ );


		if ( status == CL_DEVICE_NOT_FOUND )
		{
			numDevices[i] = 0;

			devices[i] = NULL;
		}
		else
		{
			oclCheckError( status );

			devices[i] = new cl_device_id[ numDevices[i] ];

			status = clGetDeviceIDs (
				platforms[i]    /* platform */,
				deviceType      /* device_type */,
				numDevices[i]   /* num_entries*/,
				devices[i]      /* devices*/,
				NULL            /* num_devices*/ );

			LastDevice = devices[i][0];	// Save first device from last platform
		}
	}

	/* Print information about each platform */
	//--------------------------------------------------------------------------
	for ( cl_uint i = 0; i < numPlatforms; i++ )
	{
		cout << "Platform #" << i << endl;

		oclPrintPlatformDescription( platforms[i] );

		cout << "Devices: " << endl;

		// Print Devices
		for ( cl_uint j = 0; j < numDevices[i]; j++ )
		{
			oclPrintDeviceDescription ( devices[i][j] );
		}

		cout << endl;
	}

	/* Choose device */
	//--------------------------------------------------------------------------
	if ( LastDevice == NULL )
	{
		cout << "No devices found!" << endl; exit( -1 );
	}
	else
	{
		cout << "Device will be used: ";
		oclPrintDeviceDescription( LastDevice );

		device = LastDevice;
	}

	/* Get basic info about selected device. */
	//--------------------------------------------------------------------------
	status = clGetDeviceInfo ( 
		device                          /* device */,
		CL_DEVICE_MAX_WORK_GROUP_SIZE   /* param_name */,
		sizeof ( size_t )               /* param_value_size */,
		( void * ) &maxWorkGroupSize    /* param_value */,
		NULL                            /* param_value_size_ret */ );

	oclCheckError( status );

	status = clGetDeviceInfo (
		device                               /* device */,
		CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS   /* param_name */,
		sizeof ( cl_uint )                   /* param_value_size */,
		( void * ) &maxDimensions            /* param_value */,
		NULL                                 /* param_value_size_ret */ );

	oclCheckError( status );

	maxWorkItemSizes = ( size_t * ) malloc ( maxDimensions * sizeof ( size_t ) );

	status = clGetDeviceInfo (
		device                              /* device */,
		CL_DEVICE_MAX_WORK_ITEM_SIZES       /* param_name */,
		sizeof ( size_t ) * maxDimensions   /* param_value_size */,
		( void * ) maxWorkItemSizes         /* param_value */,
		NULL                                /* param_value_size_ret */ );

	oclCheckError( status );

	status = clGetDeviceInfo (
		device                         /* device */,
		CL_DEVICE_LOCAL_MEM_SIZE       /* param_name */,
		sizeof ( cl_ulong )            /* param_value_size */,
		( void * ) &totalLocalMemory   /* param_value */,
		NULL                           /* param_value_size_ret */ );

	oclCheckError( status );

	/*Print information about selected device. */
	//--------------------------------------------------------------------------
	cout << "Max work group size: " << maxWorkGroupSize << endl;
	cout << "Max work item dimensions: " << maxDimensions << endl;

	for ( size_t i = 0; i < maxDimensions; i++ ) 
		cout << i <<" dim: " << maxWorkItemSizes[i] << " items" << endl;

	cout << "Local memory size: " << totalLocalMemory << endl;

	context = clCreateContext(
		NULL      /* properties  */,
		1         /* num_devices */,
		&device   /* devices     */,
		NULL      /* pfn_notify  */,
		NULL      /* user_data   */,
		&status   /* error code  */);

	oclCheckError( status );

	cout << "Context created" << endl;

	/* Create command queue. */
	//--------------------------------------------------------------------------
	commandQueue = clCreateCommandQueue ( 
		context   /* context     */,
		device    /* device      */,
		0         /* properties  */,
		&status   /* errcode_ret */ );

	oclCheckError( status );

	cout << "Command queue created" << endl;

	//--------------------------------------------------------------------------
	for ( cl_uint i = 0; i < numPlatforms; i++ )
	{
		delete[] devices[i];
	}

	delete[] numDevices;

	delete[] platforms;

	return status;
}