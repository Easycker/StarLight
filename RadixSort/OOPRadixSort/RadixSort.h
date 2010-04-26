#pragma once

#include <iostream>
#include <fstream>
#include <cl\cl.h>
#include "oclUtil.h"

using namespace std;

#define SORT_SOURCE_PATH "Sort.cl"
#define RADIX_CARDINALITY 256

char * LoadFile ( const char * path, int *outLength );

class oclRadixSort
{
public:

	oclRadixSort(
		cl_device_id device,
		cl_context context,
		cl_command_queue commandQueue,
		cl_uint maxGroupSize
		);

	//void SetupSort ( );

	void StartSort (
		cl_mem unsortedArray,
		cl_mem sortedArray,
		cl_int arraySize
		);

	void ReadBuffer (
		cl_mem srcBuffer, 
		void * dstBuffer, 
		size_t size
		);

	~oclRadixSort( );

private:
	void CountingSort( cl_uint pass );
	void BucketScan ( );
	void Permute ( cl_uint pass );

	// OpenCL common
	cl_program m_Program;
	cl_context m_Context;
	cl_command_queue m_commandQueue;
	cl_int m_Status;
	cl_uint m_maxWorkItemSize;
	cl_uint m_WorkGroupSize;

	// Algorithm variables
	cl_uint m_ArraySize;
	cl_uint m_CountingSortThreads;
	cl_uint m_ElementsPerThread;
	cl_uint m_BucketsQuan;
	
	// Memory objects
	cl_mem m_Buckets;
	cl_mem m_Offsets;
	cl_mem m_SortedArray;
	cl_mem m_UnsortedArray;
	cl_mem m_TempArray;

	// kernels:
	cl_kernel m_CountingSortKernel;
	cl_kernel m_BucketScanKernel;
	cl_kernel m_PermuteKernel;
};

oclRadixSort::oclRadixSort( 
						   cl_device_id device,
						   cl_context context,
						   cl_command_queue commandQueue,
						   cl_uint maxWorkItemSize
						   )
						   : m_Context( context ),
						     m_commandQueue( commandQueue ),
							 m_maxWorkItemSize( maxWorkItemSize ),
							 m_WorkGroupSize(256),
							 m_CountingSortThreads( m_maxWorkItemSize ),
							 m_BucketsQuan(m_CountingSortThreads * RADIX_CARDINALITY)
{
	const char * source = LoadFile ( SORT_SOURCE_PATH, NULL );

	size_t sourceSize [] = { strlen ( source ) };

	m_Program = clCreateProgramWithSource (
		m_Context    /* context */,
		1            /* count */,
		&source      /* strings */,
		sourceSize   /* lengths */,
		&m_Status    /* errcode_ret */ 
		);
	oclCheckError( m_Status );

	m_Status = clBuildProgram (
		m_Program   /* program */,
		1         /* num_devices */,
		&device   /* device_list */,
		NULL      /* options */,
		NULL      /* pfn_notify */,
		NULL      /* user_data */ 
		);

	if ( m_Status != CL_SUCCESS )
	{
		oclPrintBuildLog( device, m_Program );
	}

	oclCheckError( m_Status );

	m_Buckets = clCreateBuffer( 
		m_Context                                                    /* context */,
		CL_MEM_READ_WRITE                                            /* flags */,
		RADIX_CARDINALITY * m_CountingSortThreads * sizeof(cl_uint)  /* size */,
		NULL                                                         /* host_ptr */,
		&m_Status                                                    /* errcode_ret */
		);

	oclCheckError( m_Status );

	m_Offsets = clCreateBuffer( 
		m_Context                                                    /* context */,
		CL_MEM_READ_WRITE                                            /* flags */,
		RADIX_CARDINALITY * m_CountingSortThreads * sizeof(cl_uint)  /* size */,
		NULL                                                         /* host_ptr */,
		&m_Status                                                    /* errcode_ret */
		);

	oclCheckError( m_Status );

	m_CountingSortKernel = clCreateKernel (
		m_Program         /* program */,
		"CountingSort"    /* kernel_name */,
		&m_Status         /* errcode_ret */ 
		);

	oclCheckError( m_Status );

	m_BucketScanKernel = clCreateKernel (
		m_Program         /* program */,
		"Scan"            /* kernel_name */,
		&m_Status         /* errcode_ret */ 
		);

	oclCheckError( m_Status );

	m_PermuteKernel = clCreateKernel (
		m_Program         /* program */,
		"RankAndPermute"  /* kernel_name */,
		&m_Status         /* errcode_ret */ 
		);

	oclCheckError( m_Status );
}

oclRadixSort::~oclRadixSort( )
{
	clReleaseMemObject ( m_Buckets );

	clReleaseKernel ( m_CountingSortKernel );
	clReleaseKernel ( m_BucketScanKernel );
	clReleaseKernel ( m_PermuteKernel );

	clReleaseProgram ( m_Program );
}

void oclRadixSort::StartSort( 
						cl_mem unsortedArray, 
						cl_mem sortedArray, 
						cl_int arraySize 
						 )
{
	m_UnsortedArray = unsortedArray;

	m_SortedArray = sortedArray;

	m_ArraySize = arraySize;

	m_ElementsPerThread = arraySize / m_maxWorkItemSize;

	// check!
	cl_uint * hostBuckets = new cl_uint[ m_BucketsQuan ]; 

	cl_uint * hostOffsets = new cl_uint[ m_BucketsQuan ]; 

	ofstream file("log.txt");

	for ( int i = 0; i < 1; i++ )
	{
		CountingSort( i );

		ReadBuffer( m_Buckets, hostBuckets, 512 * 256 * sizeof(cl_uint) );
		cl_uint count = 0;
		//for( cl_uint j = 0; j < 256; j++ )
		//{
		//	file << "Bucket: " << j << endl;
		//	for ( cl_uint k = 0; k < m_CountingSortThreads; k++ )
		//	{
		//		file << hostBuckets[j*m_CountingSortThreads + k] << "\t";
		//		count += hostBuckets[j*m_CountingSortThreads + k];
		//	}
		//	file << endl;
		//}
		count = 0;
		//for ( cl_uint j = 0; j < 512; j++ )
		//{
		//	for ( cl_uint k = 0; k < 512; k++ )
		//	{
		//		if (hostBuckets[j] == hostBuckets[k] && j != k)
		//		{
		//			cout << endl << hostBuckets[j] << endl;
		//			count ++;
		//		}
		//	}
		//}

		if (count == 0) cout << "no collisions" << endl;

		for ( cl_uint j = 0; j < 512*256; j++ )
		{
			if (hostBuckets[j] != 0)
			{
				count ++;
			}
			if ( hostBuckets[j] >= 512 * 256 || hostBuckets[j] == -1)
			{
				cout << "ATT!!!" << hostBuckets[j] << endl;
			}
		}
		cout << "AT ALL: " << count << endl;

		BucketScan ( );

		file <<endl << "__________________________________" << endl;
		ReadBuffer( m_Offsets, hostOffsets, m_BucketsQuan * sizeof(cl_uint) );
		for ( cl_uint j = 0; j < m_BucketsQuan; j++ )
		{
			file << hostOffsets[j] << "\t";
		}
		file.close();

		//Permute ( i );

		//cl_mem temp = m_UnsortedArray;
		//m_UnsortedArray = m_SortedArray;
		//m_SortedArray = temp;
	}
}

void oclRadixSort::CountingSort( cl_uint passNumber )
{
	size_t globalThreads [] = { m_CountingSortThreads };

	size_t localThreads [] = { m_WorkGroupSize };

	/* Unsorted array */
	m_Status = clSetKernelArg(
		m_CountingSortKernel        /* kernel */,
		0                           /* arg_index */,
		sizeof ( cl_mem )           /* arg_size */,
		(void*)(&m_UnsortedArray)   /* arg_value */
		);
	oclCheckError( m_Status );

	/* Buckets histogram */
	m_Status = clSetKernelArg(
		m_CountingSortKernel   /* kernel */,
		1                      /* arg_index */,
		sizeof ( cl_mem )      /* arg_size */,
		(void*)(&m_Buckets)    /* arg_value */
		);
	oclCheckError( m_Status );

	/* threads Quantity  */
	m_Status = clSetKernelArg(
		m_CountingSortKernel              /* kernel */,
		2                                 /* arg_index */,
		sizeof ( cl_uint )                /* arg_size */,
		(void*)(&m_CountingSortThreads)   /* arg_value */
		);

	oclCheckError( m_Status );

	/* elements per thread  */
	m_Status = clSetKernelArg(
		m_CountingSortKernel            /* kernel */,
		3                               /* arg_index */,
		sizeof ( cl_uint )              /* arg_size */,
		(void*)(&m_ElementsPerThread)   /* arg_value */
		);

	oclCheckError( m_Status );

	/* pass number */
	m_Status = clSetKernelArg(
		m_CountingSortKernel   /* kernel */,
		4                      /* arg_index */,
		sizeof ( cl_uint )     /* arg_size */,
		(void*)(&passNumber)   /* arg_value */
		);
	oclCheckError( m_Status );

	m_Status = clEnqueueNDRangeKernel (
		m_commandQueue         /* command queue */,
		m_CountingSortKernel   /* kernel */,
		1                      /* work_dim */,
		NULL                   /* global_work_offset */,
		globalThreads          /* global_work_size */,
		localThreads           /* local_work_size */,
		0                      /* num_events_in_wait_list */,
		NULL                   /* event_wait_list */,
		NULL                   /* event */
		);
	oclCheckError ( m_Status, "clEnqueueNDRangeKernel CountingSort" );

	m_Status = clFinish ( m_commandQueue );
	oclCheckError( m_Status, "clFinish CountingSort" );
}


void oclRadixSort::BucketScan ( )
{
	size_t globalThreads [] = { m_WorkGroupSize };

	size_t localThreads [] = { m_WorkGroupSize };

	/* buckets */
	m_Status = clSetKernelArg(
		m_BucketScanKernel    /* kernel */,
		0                     /* arg_index */,
		sizeof ( cl_mem )     /* arg_size */,
		(void*)(&m_Buckets)   /* arg_value */
		);
	oclCheckError( m_Status );

	/* temp */
	m_Status = clSetKernelArg(
		m_BucketScanKernel                    /* kernel */,
		1                                     /* arg_index */,
		m_WorkGroupSize * 2 * sizeof(cl_uint)   /* arg_size */,
		NULL                                  /* arg_value */
		);
	oclCheckError( m_Status );

	/* offsets */
	m_Status = clSetKernelArg(
		m_BucketScanKernel    /* kernel */,
		2                     /* arg_index */,
		sizeof ( cl_mem )     /* arg_size */,
		(void*)(&m_Offsets)   /* arg_value */
		);
	oclCheckError( m_Status );

	/* Buckets quantity */
	m_Status = clSetKernelArg(
		m_BucketScanKernel          /* kernel */,
		3                        /* arg_index */,
		sizeof ( cl_uint )       /* arg_size */,
		(void*)(&m_BucketsQuan)   /* arg_value */
		);
	oclCheckError( m_Status );

	/* groupSize */
	m_Status = clSetKernelArg(
		m_BucketScanKernel          /* kernel */,
		4                           /* arg_index */,
		sizeof ( cl_uint )          /* arg_size */,
		(void*)(&m_WorkGroupSize)   /* arg_value */
		);
	oclCheckError( m_Status );

	/* Start kernels */
	m_Status = clEnqueueNDRangeKernel (
		m_commandQueue       /* command_queue */,
		m_BucketScanKernel   /* kernel */,
		1                    /* work_dim */,
		NULL                 /* global_work_offset */,
		globalThreads        /* global_work_size */,
		localThreads         /* local_work_size */,
		0                    /* num_events_in_wait_list */,
		NULL                 /* event_wait_list */,
		NULL                 /* event */ 
		);
	oclCheckError ( m_Status, "clEnqueueNDRangeKernel BucketScan" );

	m_Status = clFinish ( m_commandQueue );
	oclCheckError( m_Status, "clFinish BucketScan" );
}

void oclRadixSort::Permute ( cl_uint passNumber )
{
	size_t globalThreads [] = { m_CountingSortThreads };

	size_t localThreads [] = { m_WorkGroupSize };

	/* Unsorted Array*/
	m_Status = clSetKernelArg(
		m_PermuteKernel             /* kernel */,
		0                           /* arg_index */,
		sizeof ( cl_mem )           /* arg_size */,
		(void*)(&m_UnsortedArray)   /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Sorted Array */
	m_Status = clSetKernelArg(
		m_PermuteKernel           /* kernel */,
		1                         /* arg_index */,
		sizeof(cl_mem)            /* arg_size */,
		(void*)(&m_SortedArray)   /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Offsets array*/
	m_Status = clSetKernelArg(
		m_PermuteKernel       /* kernel */,
		2                     /* arg_index */,
		sizeof(cl_mem)        /* arg_size */,
		(void*)(&m_Offsets)   /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Threads Quantity*/
	m_Status = clSetKernelArg(
		m_PermuteKernel                   /* kernel */,
		3                                 /* arg_index */,
		sizeof(cl_uint)                   /* arg_size */,
		(void*)(&m_CountingSortThreads)   /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Elements Per Thread*/
	m_Status = clSetKernelArg(
		m_PermuteKernel                 /* kernel */,
		4                               /* arg_index */,
		sizeof(cl_uint)                 /* arg_size */,
		(void*)(&m_ElementsPerThread)   /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Pass number*/
	m_Status = clSetKernelArg(
		m_PermuteKernel       /* kernel */,
		5                     /* arg_index */,
		sizeof(cl_uint)       /* arg_size */,
		(void*)(&passNumber)  /* arg_value */
		);
	oclCheckError ( m_Status );

	/* Start kernels*/
	m_Status = clEnqueueNDRangeKernel (
		m_commandQueue    /* command_queue */,
		m_PermuteKernel   /* kernel */,
		1                 /* work_dim */,
		NULL              /* global_work_offset */,
		globalThreads     /* global_work_size */,
		localThreads      /* local_work_size */,
		0                 /* num_events_in_wait_list */,
		NULL              /* event_wait_list */,
		NULL              /* event */ 
		);
	oclCheckError ( m_Status, "clEnqueueNDRangeKernel Permute" );

	m_Status = clFinish ( m_commandQueue );
	oclCheckError ( m_Status, "clFinish Permute" );
}

void oclRadixSort::ReadBuffer (
				 cl_mem srcBuffer, 
				 void * dstBuffer, 
				 size_t size
				 )
{
	cl_event events [1];

	m_Status = clEnqueueReadBuffer (
		m_commandQueue                           /* command_queue */,
		srcBuffer                                /* buffer */,
		CL_TRUE                                  /* blocking_read */,
		0                                        /* offset */,
		size                                     /* cb */,
		dstBuffer                                /* ptr */,
		0                                        /* num_events_in_wait_list */,
		NULL                                     /* event_wait_list */,
		&events [0]                              /* event */ 
	);
	oclCheckError( m_Status, "clEnqueueReadBuffer" );

	m_Status = clWaitForEvents (
		1             /* num_events */,
		events        /* event_list */ 
		);
	oclCheckError ( m_Status, "clWaitForEvents at ReadBuffer" );
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