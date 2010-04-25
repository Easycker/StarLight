#include <iostream>

#include <fstream>

#include <cl/CL.h>

#include <math.h>

using namespace std;

//-----------------------------------------------------------------------------------------------------------

cl_context context;											/* CL context */

cl_device_id device;										/* CL device */

cl_program program;											/* CL program */

cl_kernel CountSortKernel;									/* CL kernel */

cl_kernel ScanKernel;										/* CL kernel */

cl_kernel PermuteKernel;									/* CL kernel */

cl_command_queue commandQueue;								/* CL command queue */

//-----------------------------------------------------------------------------------------------------------

size_t maxWorkGroupSize;									/* Max allowed work-items in a group */

cl_uint maxDimensions;										/* Max group dimensions allowed */

size_t * maxWorkItemSizes;									/* Max work-items sizes in each dimensions */

cl_ulong totalLocalMemory;									/* Max local memory allowed */

cl_ulong usedLocalMemory;									/* Used local memory */

size_t kernelWorkGroupSize;									/* Group size returned by kernel */

//-----------------------------------------------------------------------------------------------------------

const unsigned int  ELEMENTS_PER_THREAD = 16;

const unsigned int  ARRAY_SIZE = 16 * 2;

const unsigned int  THREADS = ARRAY_SIZE / ELEMENTS_PER_THREAD;

const unsigned int  RADIX_SIZE_BYTES = 8;

const unsigned int  RADIX_CARDINALITY = 256;

const unsigned int  BUCKETS_QUAN = THREADS * RADIX_CARDINALITY;

size_t groupSize = 1;

//Host ptr

cl_float * unsortedArray = NULL;

cl_float * sortedArray = NULL;

#ifdef _DEBUG

	cl_uint * bucketsArray = NULL;

	cl_uint * offsetsArray = NULL;

#endif

// Device memory

cl_mem unsArr;

cl_mem buckets;

//cl_mem temp;

cl_mem offsets;

cl_mem srtdArr;

//-----------------------------------------------------------------------------------------------------------

char * LoadFile ( const char * path, int *outLength = NULL );

//-----------------------------------------------------------------------------------------------------------

cl_int SetupOpenCL ( cl_device_type deviceType = CL_DEVICE_TYPE_GPU );

void ReleaseOpenCL ( );

void oclPrintPlatformDescription( cl_platform_id platform );

void oclPrintPlatformInfo (cl_platform_id platform, cl_platform_info paramName, char* paramNameStr );

void oclStatusCheck( cl_int status, const char* message );

void oclPrintDeviceDescription ( cl_device_id device );

//-----------------------------------------------------------------------------------------------------------


//Setup
void SetupCountingSort ( );
void SetupBucketScan ( );
void SetupRankAndPermute ( );
//SetupNTS (NTS means "Note The Sign")
void SetupCountingSortNTS ( );
void SetupBucketScanNTS ( );
void SetupRankAndPermuteNTS ( );

//Start
void StartCountingSort( cl_uint pass );
void StartBucketScan ( );
void StartRankAndPermute ( cl_uint pass );

//StartNTS (NTS means "Note The Sign") 
void StartCountingSortNTS( cl_uint pass);
void StartBucketScanNTS ( );
void StartRankAndPermuteNTS ( );

//-----------------------------------------------------------------------------------------------------------
int main ( int argc, char** argv )
{
	//unsortedArray = ( cl_float* ) _aligned_malloc( ARRAY_SIZE * sizeof ( cl_float4 ), 16 );

	//sortedArray =   ( cl_float* ) _aligned_malloc( ARRAY_SIZE * sizeof ( cl_float4 ), 16 );

	// Allocate memory

	unsortedArray = ( cl_float* ) malloc( ARRAY_SIZE * sizeof ( cl_float4 ) );

	sortedArray =   ( cl_float* ) malloc( ARRAY_SIZE * sizeof ( cl_float4 ) );
#ifdef _DEBUG
	bucketsArray = (cl_uint* ) malloc ( THREADS * RADIX_CARDINALITY * sizeof(cl_uint) );

	offsetsArray = (cl_uint* ) malloc ( BUCKETS_QUAN * sizeof(cl_uint) );

	memset ( offsetsArray, 0, BUCKETS_QUAN * sizeof(cl_uint) );
#endif

	//Initialize memory

	for ( int i = 0; i < ARRAY_SIZE * 4; ++i )
	{
		unsortedArray[i] = ARRAY_SIZE * 4 - (float)i;
	}

	cout << "-------------------UNSORTED ARRAY-------------------" << endl;

	for ( int i = 0; i < ARRAY_SIZE; ++i )
	{
		cout << unsortedArray[4*i + 0] << "\t";
		cout << unsortedArray[4*i + 1] << "\t";
		cout << unsortedArray[4*i + 2] << "\t";
		cout << unsortedArray[4*i + 3] << "\t";
	}

	memset( sortedArray, 0, ARRAY_SIZE * sizeof ( cl_float4 ) );

	//Setup

	SetupOpenCL ( );

	SetupCountingSort ( );
	SetupBucketScan ( );
	SetupRankAndPermute ( );

	// Start

	for ( int i = 0; i < 4*3; i++ )
	{
		StartCountingSort ( i );
		StartBucketScan ( );
		StartRankAndPermute ( i );

		cl_mem temp = unsArr;
		unsArr = srtdArr;
		srtdArr = temp;
	}

#ifdef _DEBUG
	cout << endl;
	cout << "-------------------COUNTING SORT-------------------" << endl;
	cout << "KEYS:";
	
	for ( int i = 0; i < THREADS; i ++ )
	{
		cout << "_________________THREAD #" << i << "____________________" <<endl;

		for ( int j = 0; j < ELEMENTS_PER_THREAD; j++ )
		{
			cout << j << ":\t";
			cout << unsortedArray[i * ELEMENTS_PER_THREAD * 4 + j*4 + 0] << "\t";
			cout << unsortedArray[i * ELEMENTS_PER_THREAD * 4 + j*4 + 1] << "\t";
			cout << unsortedArray[i * ELEMENTS_PER_THREAD * 4 + j*4 + 2] << "\t";
			cout << unsortedArray[i * ELEMENTS_PER_THREAD * 4 + j*4 + 3] << "\t";
			cout << endl;
		}
		cout << endl;
	}

	cout << "---------------------BUCKET DATA ---------------------" << endl;

	unsigned int count = 0;

	for ( int i = 0; i < RADIX_CARDINALITY; i ++ )
	{
		for ( int j = 0; j < THREADS; j++ )
		{
			if ( bucketsArray[i*THREADS + j] > 0 )
			{
				cout << "B#" <<i << "T" << j << "\t";
				cout << bucketsArray[i*THREADS + j] << "\t";
				count += bucketsArray[i * THREADS + j];
			}
		}
	}
	cout << endl;
	cout << "All: " << count;
	cout << endl;

	cout << "-------------------SCAN-------------------" << endl;

	for ( int i = 0; i < BUCKETS_QUAN; i ++ )
	{
		cout << offsetsArray[i] << "\t";
	}
	cout << "-------------------SORTED ARRAY-------------------" << endl;

	for ( int i = 0; i < ARRAY_SIZE; ++i )
	{
		cout << sortedArray[4*i + 0] << "\t";
		cout << sortedArray[4*i + 1] << "\t";
		cout << sortedArray[4*i + 2] << "\t";
		cout << sortedArray[4*i + 3] << "\t";
	}

#endif

	ReleaseOpenCL ( );

	free ( unsortedArray );
	free ( sortedArray );

#ifdef _DEBUG
	free ( bucketsArray );
	free ( offsetsArray );
#endif

	return 0;
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

	oclStatusCheck( status, "clGetPlatformIDs failed" );

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
			oclStatusCheck( status, "clGetDeviceIDs" );

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

	oclStatusCheck ( 
		status, 
		"clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE failed" );

	status = clGetDeviceInfo (
		device                               /* device */,
		CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS   /* param_name */,
		sizeof ( cl_uint )                   /* param_value_size */,
		( void * ) &maxDimensions            /* param_value */,
		NULL                                 /* param_value_size_ret */ );

	oclStatusCheck ( 
		status, 
		"clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS failed" );

	maxWorkItemSizes = ( size_t * ) malloc ( maxDimensions * sizeof ( size_t ) );

	status = clGetDeviceInfo (
		device                              /* device */,
		CL_DEVICE_MAX_WORK_ITEM_SIZES       /* param_name */,
		sizeof ( size_t ) * maxDimensions   /* param_value_size */,
		( void * ) maxWorkItemSizes         /* param_value */,
		NULL                                /* param_value_size_ret */ );

	oclStatusCheck ( 
		status, 
		"ERROR! clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_SIZES failed" );

	status = clGetDeviceInfo (
		device                         /* device */,
		CL_DEVICE_LOCAL_MEM_SIZE       /* param_name */,
		sizeof ( cl_ulong )            /* param_value_size */,
		( void * ) &totalLocalMemory   /* param_value */,
		NULL                           /* param_value_size_ret */ );

	oclStatusCheck( 
		status,
		"clGetDeviceInfo CL_DEVICE_LOCAL_MEM_SIZE failed" );

	/*Print information about selected device. */
	//--------------------------------------------------------------------------
	cout << "Max work group size: " << maxWorkGroupSize << endl;
	cout << "Max work item dimensions: " << maxDimensions << endl;

	for ( size_t i = 0; i < maxDimensions; i++ ) 
		cout << i <<" dim: " << maxWorkItemSizes[i] << " items" << endl;

	cout << "Local memory size: " << totalLocalMemory << endl;

	/* Create Context */
	//--------------------------------------------------------------------------
	//cl_context_properties properties[] =
	//{
	//	CL_CONTEXT_PLATFORM, 
	//};

	context = clCreateContext(
		NULL      /* properties  */,
		1         /* num_devices */,
		&device   /* devices     */,
		NULL      /* pfn_notify  */,
		NULL      /* user_data   */,
		&status   /* error code  */);

	oclStatusCheck ( status, "clCreateContext" );

	cout << "Context created" << endl;

	/* Create command queue. */
	//--------------------------------------------------------------------------
	commandQueue = clCreateCommandQueue ( 
		context   /* context     */,
		device    /* device      */,
		0         /* properties  */,
		&status   /* errcode_ret */ );

	oclStatusCheck ( status, "clCreateCommandQueue" );

	cout << "Command queue created" << endl;

	/* Memory objects*/
	//--------------------------------------------------------------------------

	unsArr = clCreateBuffer (
		context                                   /* context */,
		CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR   /* flags */,
		ARRAY_SIZE * sizeof ( cl_float4 )         /* size */,
		unsortedArray                             /* host_ptr */,
		&status                                   /* errcode_ret */ );

	oclStatusCheck( 
		status,
		"clCreateBuffer for unsortedArray" 
		);

	srtdArr = clCreateBuffer (
		context                                   /* context */,
		CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR   /* flags */,
		ARRAY_SIZE * sizeof ( cl_float4 )         /* size */,
		sortedArray                               /* host_ptr */,
		&status                                   /* errcode_ret */ );

	oclStatusCheck( 
		status,
		"clCreateBuffer for sortedArray" 
		);

	buckets = clCreateBuffer (
		context                           /* context */,
		CL_MEM_READ_WRITE                 /* flags */,
		BUCKETS_QUAN * sizeof (cl_uint)   /* size */,
		NULL                              /* host_ptr */,
		&status                           /* errcode_ret */ );

	oclStatusCheck( 
		status,
		"clCreateBuffer for buckets" 
		);

	offsets = clCreateBuffer (
		context                           /* context */,
		CL_MEM_READ_WRITE                 /* flags */,
		BUCKETS_QUAN * sizeof (cl_uint)   /* size */,
		NULL                              /* host_ptr */,
		&status                           /* errcode_ret */ );

	oclStatusCheck( 
		status,
		"clCreateBuffer for offsets" 
		);

	//temp = clCreateBuffer (
	//	context                              /* context */,
	//	CL_MEM_READ_WRITE                    /* flags */,
	//	BUCKETS_QUAN * 2* sizeof (cl_uint)   /* size */,
	//	NULL                                 /* host_ptr */,
	//	&status                              /* errcode_ret */ );

	oclStatusCheck( 
		status,
		"clCreateBuffer for temp_offsets" 
		);

	cout << "Memory allocated and initialized" << endl;

	/* Load and Create Program */
	//--------------------------------------------------------------------------

	const char * source = LoadFile ( "Sort.cl" );

	size_t sourceSize [] = { strlen ( source ) };

	program = clCreateProgramWithSource (
		context      /* context */,
		1            /* count */,
		&source      /* strings */,
		sourceSize   /* lengths */,
		&status      /* errcode_ret */ );

	oclStatusCheck(
		status,
		"clCreateProgramWithSource" 
		);

	cout << "Program created" <<endl;

	/* Build Program */
	//--------------------------------------------------------------------------

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

		cout << "ERROR! clBuildProgram failed" << endl; exit ( -1 );
	}

	cout << "Program built" << endl;

	/* Release memory */
	//--------------------------------------------------------------------------
	for ( cl_uint i = 0; i < numPlatforms; i++ )
	{
		delete[] devices[i];
	}

	delete[] numDevices;

	delete[] platforms;

	return status;
}

void ReleaseOpenCL()
{
	cl_int status;

	// Memory object

	clReleaseMemObject( buckets );

	//clReleaseMemObject( temp );

	clReleaseMemObject ( offsets );

	clReleaseMemObject( srtdArr );

	clReleaseMemObject( unsArr );

	// Command queue 

	status = clReleaseCommandQueue ( commandQueue );

	status = NULL;

	// Context

	status = clReleaseContext ( context );

	context = NULL;

	// Device 

	device = NULL;
}

void SetupCountingSort()
{
	cl_int status;

	/* Create kernel from program */
	//--------------------------------------------------------------------------
	CountSortKernel = clCreateKernel (
		program         /* program */,
		"CountingSort"  /* kernel_name */,
		&status         /* errcode_ret */ );

	oclStatusCheck (
		status,
		"clCreateKernel scan" 
		);

	/* Get kernel info */
	//--------------------------------------------------------------------------
	status = clGetKernelWorkGroupInfo (
		CountSortKernel                     /* kernel */,
		device                     /* device */,
		CL_KERNEL_LOCAL_MEM_SIZE   /* param_name */,
		sizeof ( cl_ulong )        /* param_value_size */,
		&usedLocalMemory           /* param_value */,
		NULL                       /* param_value_size_ret */ );

	oclStatusCheck(
		status,
		"clGetKernelWorkGroupInfo usedLocalMemory" 
		);

	if ( usedLocalMemory > totalLocalMemory )
	{
		cout << "Unsupported: Insufficient local memory on device." << endl; exit ( -1 );
	}

	cout << "Used local memory: " << usedLocalMemory << "/" << totalLocalMemory << endl; 

	status = clGetKernelWorkGroupInfo (
		CountSortKernel                      /* kernel */,
		device                      /* device */,
		CL_KERNEL_WORK_GROUP_SIZE   /* param_name */,
		sizeof ( size_t )           /* param_value_size */,
		&kernelWorkGroupSize        /* param_value */,
		0                           /* param_value_size_ret */ );

	oclStatusCheck(
		status,
		"clGetKernelWorkGroupInfo kernelWorkGroupSize" 
		);
}


void SetupBucketScan ( )
{
	cl_int status;

	/* Create kernel from program */
	//--------------------------------------------------------------------------
	ScanKernel = clCreateKernel (
		program         /* program */,
		"Scan"          /* kernel_name */,
		&status         /* errcode_ret */ 
		);

	oclStatusCheck (
		status,
		"clCreateKernel scan" 
		);
}

void SetupRankAndPermute ( )
{
	cl_int status;

	/* Create kernel from program */
	//--------------------------------------------------------------------------
	PermuteKernel = clCreateKernel (
		program           /* program */,
		"RankAndPermute"  /* kernel_name */,
		&status           /* errcode_ret */ 
		);

	oclStatusCheck (
		status,
		"clCreateKernel scan" 
		);
}

void StartCountingSort( cl_uint passNumber)
{
	cl_int status;

	cl_event events [1];

	/* Enqueue a kernel run call. */
	//--------------------------------------------------------------------------
	size_t globalThreads [] = { THREADS };

	size_t localThreads [] = { groupSize };

	status = clSetKernelArg(
		CountSortKernel     /* kernel */,
		0                   /* arg_index */,
		sizeof ( cl_mem )   /* arg_size */,
		(void*)(&unsArr)    /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg unsArr" 
		);

	status = clSetKernelArg(
		CountSortKernel     /* kernel */,
		1                   /* arg_index */,
		sizeof(cl_mem)      /* arg_size */,
		(void*)(&buckets)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg buckets" 
		);

	status = clSetKernelArg(
		CountSortKernel       /* kernel */,
		2                     /* arg_index */,
		sizeof(cl_uint)       /* arg_size */,
		(void*)(&passNumber)  /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg radixNum" 
		);

	cout << "Kernel arguments was set up" << endl;

	status = clEnqueueNDRangeKernel (
		commandQueue      /* command_queue */,
		CountSortKernel   /* kernel */,
		1                 /* work_dim */,
		NULL              /* global_work_offset */,
		globalThreads     /* global_work_size */,
		localThreads      /* local_work_size */,
		0                 /* num_events_in_wait_list */,
		NULL              /* event_wait_list */,
		NULL              /* event */ 
		);

	oclStatusCheck (
		status,
		"clEnqueueNDRangeKernel run kernels" 
		);

	//status = clFinish ( commandQueue );

	oclStatusCheck (
		status,
		"clFinish" 
		);

	cout << "Counting sort finish it's work" << endl;

#ifdef _DEBUG
	/* Enqueue read buffer. */
	//--------------------------------------------------------------------------
	status = clEnqueueReadBuffer (
		commandQueue                             /* command_queue */,
		buckets								     /* buffer */,
		CL_TRUE                                  /* blocking_read */,
		0                                        /* offset */,
		THREADS * RADIX_CARDINALITY * sizeof(cl_uint)   /* cb */,
		bucketsArray                             /* ptr */,
		0                                        /* num_events_in_wait_list */,
		NULL                                     /* event_wait_list */,
		&events [0]                              /* event */ 
	);

	oclStatusCheck (
		status,
		"clEnqueueReadBuffer srtdArr->sortedArray" 
		);

	status = clWaitForEvents (
		1             /* num_events */,
		events        /* event_list */ 
		);

	oclStatusCheck (
		status,
		"clWaitForEvents" 
		);

	clReleaseEvent ( events [0] );

	cout << "Buckets uploaded to the host" << endl;
#endif
}


void StartBucketScan ( )
{
	cl_int status;

	cl_event events [1];

	/* Arguments */
	//--------------------------------------------------------------------------

	/* buckets */
	status = clSetKernelArg(
		ScanKernel     /* kernel */,
		0                   /* arg_index */,
		sizeof ( cl_mem )   /* arg_size */,
		(void*)(&buckets)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg kernel: scan arg: buckets" 
		);

	/* temp */
	//status = clSetKernelArg(
	//	ScanKernel     /* kernel */,
	//	1                   /* arg_index */,
	//	sizeof ( cl_mem )   /* arg_size */,
	//	(void*)(&temp)   /* arg_value */
	//	);

	status = clSetKernelArg(
		ScanKernel                           /* kernel */,
		1                                    /* arg_index */,
		BUCKETS_QUAN * 2 * sizeof(cl_uint)   /* arg_size */,
		NULL                                 /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg kernel: scan arg: temp" 
		);

	/* offsets */
	status = clSetKernelArg(
		ScanKernel     /* kernel */,
		2                   /* arg_index */,
		sizeof ( cl_mem )   /* arg_size */,
		(void*)(&offsets)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg kernel: scan arg: offsets" 
		);

	/* N */
	status = clSetKernelArg(
		ScanKernel          /* kernel */,
		3                        /* arg_index */,
		sizeof ( cl_uint )       /* arg_size */,
		(void*)(&BUCKETS_QUAN)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg kernel: scan arg: N" 
		);

	/* Enqueue a kernel run call. */
	//--------------------------------------------------------------------------
	size_t globalThreads [] = { BUCKETS_QUAN };

	size_t localThreads [] = { BUCKETS_QUAN };

	status = clEnqueueNDRangeKernel (
		commandQueue      /* command_queue */,
		ScanKernel        /* kernel */,
		1                 /* work_dim */,
		NULL              /* global_work_offset */,
		globalThreads     /* global_work_size */,
		localThreads      /* local_work_size */,
		0                 /* num_events_in_wait_list */,
		NULL              /* event_wait_list */,
		NULL              /* event */ 
		);

	oclStatusCheck (
		status,
		"clEnqueueNDRangeKernel run kernels" 
		);

	//status = clFinish ( commandQueue );

	oclStatusCheck (
		status,
		"clFinish" 
		);

	cout << "BucketScan finish it's work" << endl;

#ifdef _DEBUG
	/* Enqueue read buffer. */
	//--------------------------------------------------------------------------
	status = clEnqueueReadBuffer (
		commandQueue                     /* command_queue */,
		offsets                          /* buffer */,
		CL_TRUE                          /* blocking_read */,
		0                                /* offset */,
		BUCKETS_QUAN * sizeof(cl_uint)   /* cb */,
		offsetsArray                     /* ptr */,
		0                                /* num_events_in_wait_list */,
		NULL                             /* event_wait_list */,
		&events [0]                      /* event */ 
	);

	oclStatusCheck (
		status,
		"clEnqueueReadBuffer srtdArr->sortedArray" 
		);

	status = clWaitForEvents (
		1             /* num_events */,
		events        /* event_list */ 
		);

	oclStatusCheck (
		status,
		"clWaitForEvents" 
		);

	clReleaseEvent ( events [0] );

	cout << "offsets uploaded to the host" << endl;
#endif
}

void StartRankAndPermute ( cl_uint passNumber )
{
	cl_int status;

	cl_event events [1];

	/* Enqueue a kernel run call. */
	//--------------------------------------------------------------------------
	size_t globalThreads [] = { THREADS };

	size_t localThreads [] = { groupSize };

	status = clSetKernelArg(
		PermuteKernel     /* kernel */,
		0                   /* arg_index */,
		sizeof ( cl_mem )   /* arg_size */,
		(void*)(&unsArr)    /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg unsArr" 
		);

	status = clSetKernelArg(
		PermuteKernel     /* kernel */,
		1                   /* arg_index */,
		sizeof(cl_mem)      /* arg_size */,
		(void*)(&srtdArr)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg srtdArr" 
		);

	status = clSetKernelArg(
		PermuteKernel     /* kernel */,
		2                   /* arg_index */,
		sizeof(cl_mem)      /* arg_size */,
		(void*)(&offsets)   /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg offsets" 
		);

	status = clSetKernelArg(
		PermuteKernel       /* kernel */,
		3                     /* arg_index */,
		sizeof(cl_uint)       /* arg_size */,
		(void*)(&passNumber)  /* arg_value */
		);

	oclStatusCheck (
		status,
		"clSetKernelArg passNumber" 
		);

	cout << "Kernel arguments was set up" << endl;

	status = clEnqueueNDRangeKernel (
		commandQueue      /* command_queue */,
		PermuteKernel     /* kernel */,
		1                 /* work_dim */,
		NULL              /* global_work_offset */,
		globalThreads     /* global_work_size */,
		localThreads      /* local_work_size */,
		0                 /* num_events_in_wait_list */,
		NULL              /* event_wait_list */,
		NULL              /* event */ 
		);

	oclStatusCheck (
		status,
		"clEnqueueNDRangeKernel run kernels" 
		);

	//status = clFinish ( commandQueue );

	oclStatusCheck (
		status,
		"clFinish" 
		);

	cout << "StartRankAndPermute finish it's work" << endl;

#ifdef _DEBUG
	/* Enqueue read buffer. */
	//--------------------------------------------------------------------------
	status = clEnqueueReadBuffer (
		commandQueue                             /* command_queue */,
		srtdArr                                  /* buffer */,
		CL_TRUE                                  /* blocking_read */,
		0                                        /* offset */,
		ARRAY_SIZE*sizeof(cl_float4)             /* cb */,
		sortedArray                              /* ptr */,
		0                                        /* num_events_in_wait_list */,
		NULL                                     /* event_wait_list */,
		&events [0]                              /* event */ 
	);

	oclStatusCheck (
		status,
		"clEnqueueReadBuffer srtdArr->sortedArray" 
		);

	status = clWaitForEvents (
		1             /* num_events */,
		events        /* event_list */ 
		);

	oclStatusCheck (
		status,
		"clWaitForEvents" 
		);

	clReleaseEvent ( events [0] );

	cout << "Buckets uploaded to the host" << endl;
#endif
}

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

	oclStatusCheck ( status, "clGetDeviceInfo" );

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

	oclStatusCheck( 
		status,               /* status */
		"clGetPlatformInfo"   /* message */ );

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

void oclStatusCheck( cl_int status, const char* message )
{
	if ( status != CL_SUCCESS )
	{
		cout << "ERROR! " << status << " " << message << endl;

		ReleaseOpenCL();

		exit ( -1 );
	}
}

char * LoadFile ( const char * path, int *outLength )
{
	ifstream file ( path );

	//-----------------------------------------------------------------

	if ( !file )
	{
		cout << "ERROR: Could not open file " << path << endl; exit ( -1 );
	}

	//-----------------------------------------------------------------

	file.seekg ( 0, ios :: end );

	unsigned long length = file.tellg ( );

	if (outLength)
	{
		*outLength = static_cast<int>(length);
	}

	file.seekg ( 0, ios :: beg );

	//-----------------------------------------------------------------

	if ( 0 == length )
	{
		cout << "WARNING: File is empty" << endl;
	}

	//-----------------------------------------------------------------

	char * source = new char [length + 1];

	unsigned long i = 0;

	while ( file )
	{
		source [i++] = file.get ( );
	}

	source [i - 1] = 0;

	return source;
}