const unsigned int  RADIX_SIZE_BYTES = 8;

const unsigned int  RADIX_CARDINALITY = 256;

__kernel void CountingSort(__global float4 * unsorted, 
						   __global   uint * buckets,
									  uint threadsQuan,
									  uint elementsPerThread,
					                  uint passNumber )
{
	uint gid = get_global_id ( 0 );

	// Threadself range of array
	uint valueRangeOffset = gid * elementsPerThread;

	// Clean buckets
	for (uint i = 0; i < 256; i++)
	{
		buckets[gid * RADIX_CARDINALITY + i] = 0;
	}

	// calc radix position
	uint indexOffset  =  2 - (passNumber * RADIX_SIZE_BYTES) / 32 ;

	uint leftOffset = RADIX_SIZE_BYTES * ( 3 - passNumber % 4 );

	uint rightOffset = RADIX_SIZE_BYTES * 3;

	union { uint U[4]; float4 F; } Value;

	for ( uint i = 0; i < elementsPerThread; i ++ )
	{
		Value.F = unsorted [ valueRangeOffset + i ];

		// calc radix value

		uint bucketId = Value.U[indexOffset];

		bucketId = bucketId << leftOffset;

		bucketId = bucketId >> rightOffset;

		// inc bucket with value = radix value
		// buckets are private for each thread
		//buckets[256 * gid + bucketId] = 1;

		//buckets[threadsQuan * bucketId + gid] += 1;
		buckets[threadsQuan * bucketId + gid] += 1;
	}
}

// BucketsSize = Threads * RADIX_CARDINALITY * sizeof( uint )
// TempSize = BucketsSize * 2
// Offsets  = BucketsSize

__kernel void Scan(__global uint * buckets,
				   __local  uint * temp,
				   __global uint * offsets,
				            uint bucketsQuan,
							uint groupSize)
{
	uint tid = get_local_id ( 0 );

	uint lastIterationPrefix = 0;

	for ( int i = 0; i < bucketsQuan / groupSize; i++ )
	{
		int pout = 0, pin = 1;

		uint tOffset = i * groupSize + tid;

		// sinthronize
		barrier ( CLK_LOCAL_MEM_FENCE );

		// Load piece of array to local memory
		temp[pout * groupSize + tid] = ( tOffset > 0 ) ? buckets[ tOffset - 1 ] : 0;

		temp[pin * groupSize + tid] = 0;

		// Do prefix-sum for [i*groupSize ; i*groupSize + groupSize] range
		for (uint offset = 1; offset < groupSize; offset *= 2)
		{
			pout = 1 - pout;
			pin  = 1 - pin;

			if ( tid >= offset )
			{
				temp[ pout * groupSize + tid ] = temp[ pin * groupSize + tid ] + temp[ pin * groupSize + tid - offset ];
			}
			else
			{
				temp[ pout * groupSize + tid ] = temp[ pin * groupSize + tid ];
			}
			barrier ( CLK_LOCAL_MEM_FENCE );
		}

		offsets[tOffset] = temp[ pout * groupSize + tid ] + lastIterationPrefix;

		barrier ( CLK_LOCAL_MEM_FENCE );

		if ( tid == groupSize - 1) {
			lastIterationPrefix += temp[ pout * groupSize + tid ];
		}
	}
}

__kernel void RankAndPermute(__global float4 * unsorted,
							 __global float4 * sorted,
							 __global uint * offsets,
							 		  uint threadsQuan,
									  uint elementsPerThread,
									  uint passNumber)
{
	uint gid = get_global_id ( 0 );

	// Threadself range of array
	uint valueRangeOffset = gid * elementsPerThread;

	// calc radix position
	uint indexOffset  =  2 - (passNumber * RADIX_SIZE_BYTES) / 32 ;

	uint leftOffset = RADIX_SIZE_BYTES * ( 3 - passNumber % 4 );

	uint rightOffset = RADIX_SIZE_BYTES * 3;

	union { uint U[4]; float4 F; } Value;

	for ( uint i = 0; i < elementsPerThread; i ++ )
	{
		Value.F = unsorted [ valueRangeOffset + i ];

		// calc radix value

		uint bucketId = Value.U[indexOffset];

		bucketId = bucketId << leftOffset;

		bucketId = bucketId >> rightOffset;

		// inc bucket with value = radix value
		// buckets are private for each thread

		sorted[offsets[threadsQuan * bucketId + gid]] = Value.F;

		offsets[threadsQuan * bucketId + gid] += 1;
	}
}