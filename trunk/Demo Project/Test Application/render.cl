#define DELTA 0.3f

#define USE_BOX_

typedef struct _SRay
{
	float4 Origin;
	
	float4 Direction;
}SRay, *PSRay;

bool IntersectBox ( PSRay pRay, float4 minimum, float4 maximum, float* start, float* final )
{
	float4 OMAX = ( minimum - pRay->Origin ) / pRay->Direction;
	
	float4 OMIN = ( maximum - pRay->Origin ) / pRay->Direction;
	
	float4 MAX = max ( OMAX, OMIN );
	
	float4 MIN = min ( OMAX, OMIN );
	
	*final = min ( MAX.x, min ( MAX.y, MAX.z ) );
	
	*start = max ( max ( MIN.x, 0.0F), max ( MIN.y, MIN.z ) );	
	
	return *final >*start;
}
bool IntersectSphere ( PSRay pRay, float radius, float* start, float* final )
{
	float A = dot ( pRay->Direction, pRay->Direction );

	float B = dot ( pRay->Direction, pRay->Origin );

	float C = dot ( pRay->Origin, pRay->Origin ) - radius * radius;

	float D = B * B - A * C;
	
	if ( D > 0.0f )
	{
		D = sqrt ( D );

		*start = fmax ( 0.0F, ( -B - D ) / A );

		*final = ( -B + D ) / A;

		return *final > 0.0F;
	}

	return false;
}
float CalcFunction ( float4 point )
{
	float x = point.x, y = point.y, z = point.z, T = 1.6180339887f;

	return 2.0f - native_cos ( x + T * y ) - native_cos ( x - T * y ) - native_cos ( y + T * z ) -
		         native_cos ( y - T * z ) - native_cos ( z - T * x ) - native_cos ( z + T * x );

//	return native_sin ( x ) + native_sin ( y ) + native_sin ( z );
}
#define STEP  0.01f

float4 CalcNormal ( float4 point )
{
	float4 AxisX = (float4) ( 1.0F, 0.0F, 0.0F, 0.0F );

	float4 AxisY = (float4) ( 0.0F, 1.0F, 0.0F, 0.0F );

	float4 AxisZ = (float4) ( 0.0F, 0.0F, 1.0F, 0.0F );

	float A = CalcFunction ( point - AxisX * STEP );
	float B = CalcFunction ( point + AxisX * STEP );

	float C = CalcFunction ( point - AxisY * STEP );
	float D = CalcFunction ( point + AxisY * STEP );

	float E = CalcFunction ( point - AxisZ * STEP );
	float F = CalcFunction ( point + AxisZ * STEP );

	return normalize ( (float4) ( B - A, D - C, F - E, 0 ) );
}

#define INTERVALS 100

bool IntersectSurface ( PSRay pRay, float start, float final, float* val)
{
	float step = ( final - start ) / (float) ( INTERVALS );

	//-----------------------------------------------------------------------------------

	float time = start;

	float4 point = pRay->Origin + time * pRay->Direction;

	//-----------------------------------------------------------------------------------

	float right, left = CalcFunction ( point );

	//-----------------------------------------------------------------------------------

	for ( int i = 0; i < INTERVALS; i++ )
	{
		time += step;

		point += step * pRay->Direction;

		right = CalcFunction ( point );
		
		if ( left * right < 0.0F )
		{
			*val = time + right * step / ( left - right );

			return true;
		}
		
		left = right;
	}

	return false;
}
void GenerateRay (  PSRay pRay,
				    const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale,
					int2 TexCoord )
{
	float2 coords = (float2)((float)TexCoord.x/256.0f - 1.0f, (float)TexCoord.y/256.0f - 1.0f) * Scale;
	
	float4 direction = View - Side * coords.x + Up * coords.y;
   
	pRay->Origin = Position;
	pRay->Origin.w = 0;
	pRay->Direction.w = 0;
	pRay->Direction = normalize ( direction );
}

#define K_A 0.2F

#define K_D 0.8F

#define K_S 0.8F

#define P 64.0F

float4 reflect(float4 vec, float4 normal)
{
	return vec - normal * 2 * dot(vec, normal);
}
float4 Phong ( float4 point, float4 normal, float4 color, __constant float4 Position)
{
	float4 Unit = (float4) ( 1.0F, 1.0F, 1.0F, 1.0F );

	float4 light = normalize ( Position - point );
   
	float diffuse = fabs ( dot ( light, normal ) );

	float4 reflectVec = reflect ( -light, normal );

	float specular = native_powr ( fabs ( dot ( reflectVec, light ) ), P );

	return K_A * Unit + diffuse * ( K_D * color + K_S * specular * Unit );
}

float4 Raytrace ( PSRay pRay, const float4 Position)
{
	float4 BoxMinimum = (float4) ( -5.0F, -5.0F, -5.0F, 0.0F );

	float4 BoxMaximum = (float4) ( 5.0F, 5.0F, 5.0F, 0.0F );

	float Radius = 5.5f;

	float4 result = (float4) ( 0.0F, 0.0F, 0.0F, 0.0F );
    
	float start, final, time;

#ifdef USE_BOX

	if ( IntersectBox ( pRay, BoxMinimum, BoxMaximum, &start, &final ) )

#else

	if ( IntersectSphere ( pRay, Radius, &start, &final ) )

#endif
	{
		if ( IntersectSurface ( pRay, start, final, &time ))
		{
			float4 point = pRay->Origin + pRay->Direction * time;
					
			float4 normal = CalcNormal ( point );

			float4 color = native_divide( point - BoxMinimum , BoxMaximum - BoxMinimum );

			result = Phong ( point, normal, color, Position);
		}
	}
	return result;
}

//OpenCL kernel
__kernel void main (__global float4 * texture,	
			        const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale)
{
	__private int2 TexCoord = (int2)(get_global_id(0), get_global_id(1));

	__private SRay ray;
	GenerateRay (&ray,
      			 Position,
				 Side,
				 Up,
				 View,
				 Scale,
				 TexCoord );

	texture[512*TexCoord.y + TexCoord.x] = Raytrace ( &ray, Position);
}