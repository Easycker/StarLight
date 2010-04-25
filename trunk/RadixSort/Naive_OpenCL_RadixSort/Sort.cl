const unsigned int  RADIX_SIZE_BYTES = 8;

const unsigned int  RADIX_CARDINALITY = 256;

const unsigned int  ELEMENTS_PER_THREAD = 16;

const unsigned int  ARRAY_SIZE = 32;

const unsigned int  THREADS = 32 / 16;

__kernel void CountingSort(__global float4 * unsorted, 
						   __global   uint * buckets,
					                  uint passNumber )
{
	uint gid = get_global_id ( 0 );

	// Threadself range of array
	uint valueRangeOffset = gid * ELEMENTS_PER_THREAD;

	// Clean buckets
	for (int i = 0; i < RADIX_CARDINALITY; i++)
	{
		buckets[gid * RADIX_CARDINALITY + i] = 0;
	}

	// calc radix position
	uint indexOffset  =  2 - (passNumber * RADIX_SIZE_BYTES) / 32 ;

	uint leftOffset = RADIX_SIZE_BYTES * ( 3 - passNumber % 4 );

	uint rightOffset = RADIX_SIZE_BYTES * 3;

	union { uint U[4]; float4 F; } Value;

	for ( uint i = 0; i < ELEMENTS_PER_THREAD; i ++ )
	{
		Value.F = unsorted [ valueRangeOffset + i ];

		// calc radix value

		uint bucketId = Value.U[indexOffset];

		bucketId = bucketId << leftOffset;

		bucketId = bucketId >> rightOffset;

		// inc bucket with value = radix value
		// buckets are private for each thread

		buckets[THREADS * bucketId + gid] += 1;
	}
}

// BucketsSize = Threads * RADIX_CARDINALITY * sizeof( uint )
// TempSize = BucketsSize * 2
// Offsets  = BucketsSize

__kernel void Scan(__global uint * buckets,
				   __local  uint * temp,
				   __global uint * offsets,
				            uint n)
{
	uint tid = get_local_id ( 0 );

	int pout = 0, pin = 1;

	temp[pout * n + tid] = ( tid > 0 ) ? buckets[ tid - 1 ] : 0;
	temp[pin * n + tid] = 0;

	barrier ( CLK_LOCAL_MEM_FENCE );

	for (uint offset = 1; offset < n; offset *= 2)
	{
		pout = 1 - pout;
		pin  = 1 - pin; // in scan.pdf there is pout in this place

		if ( tid >= offset )
		{
			temp[ pout*n + tid ] = temp[ pin*n + tid ] + temp[ pin*n + tid - offset ];
		}
		else
		{
			temp[ pout*n + tid ] = temp[ pin*n + tid ];
		}
		
		barrier ( CLK_LOCAL_MEM_FENCE );
	}

	offsets[tid] = temp[pout*n + tid];
}

__kernel void RankAndPermute(__global float4 * unsorted,
							 __global float4 * sorted,
							 __global uint * offsets,
									  uint passNumber)
{
	uint gid = get_global_id ( 0 );

	// Threadself range of array
	uint valueRangeOffset = gid * ELEMENTS_PER_THREAD;

	// calc radix position
	uint indexOffset  =  2 - (passNumber * RADIX_SIZE_BYTES) / 32 ;

	uint leftOffset = RADIX_SIZE_BYTES * ( 3 - passNumber % 4 );

	uint rightOffset = RADIX_SIZE_BYTES * 3;

	union { uint U[4]; float4 F; } Value;

	for ( uint i = 0; i < ELEMENTS_PER_THREAD; i ++ )
	{
		Value.F = unsorted [ valueRangeOffset + i ];

		// calc radix value

		uint bucketId = Value.U[indexOffset];

		bucketId = bucketId << leftOffset;

		bucketId = bucketId >> rightOffset;

		// inc bucket with value = radix value
		// buckets are private for each thread

		sorted[offsets[THREADS * bucketId + gid]] = Value.F;

		offsets[THREADS * bucketId + gid] += 1;
	}
}