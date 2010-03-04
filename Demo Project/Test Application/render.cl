#define DELTA 0.3f

float Radius = 5.5f;

#define USE_BOX_

typedef struct _SCamera
{
	float4 Position;
	
	float4 Side;
	
	float4 Up;
	
	float4 View;
	
	float2 Scale;
}SCamera;

typedef struct _SRay
{
	float4 Origin;
	
	float4 Direction;
}SRay;

bool IntersectBox ( SRay ray, float4 minimum, float4 maximum, float* start, float* final )
{
	float4 OMAX = ( minimum - ray.Origin ) / ray.Direction;
	
	float4 OMIN = ( maximum - ray.Origin ) / ray.Direction;
	
	float4 MAX = fmax ( OMAX, OMIN );
	
	float4 MIN = fmin ( OMAX, OMIN );
	
	*final = fmin ( MAX.x, fmin ( MAX.y, MAX.z ) );
	
	*start = fmax ( fmax ( MIN.x, 0.0F), fmax ( MIN.y, MIN.z ) );	
	
	return *final >*start;
}
bool IntersectSphere ( SRay ray, float radius, float* start, float* final )
{
	float A = dot ( ray.Direction, ray.Direction );

	float B = dot ( ray.Direction, ray.Origin );

	float C = dot ( ray.Origin, ray.Origin ) - radius * radius;

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

	return 2.0f - cos ( x + T * y ) - cos ( x - T * y ) - cos ( y + T * z ) -
		         cos ( y - T * z ) - cos ( z - T * x ) - cos ( z + T * x );

	//return sin ( x ) + sin ( y ) + sin ( z );
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

bool IntersectSurface ( SRay ray, float start, float final, float* val)
{
	float step = ( final - start ) / (float) ( INTERVALS );

	//-----------------------------------------------------------------------------------

	float time = start;

	float4 point = ray.Origin + time * ray.Direction;

	//-----------------------------------------------------------------------------------

	float right, left = CalcFunction ( point );

	//-----------------------------------------------------------------------------------

	for ( float i = 0.0f; i < (float) ( INTERVALS ); i++ )
	{
		time += step;

		point += step * ray.Direction;

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
SRay GenerateRay ( SCamera camera, int2 TexCoord )
{
	float2 coords = (float2)((float)TexCoord.x/256.0f - 1.0f, (float)TexCoord.y/256.0f - 1.0f) * camera.Scale;
	
	float4 direction = camera.View - camera.Side * coords.x + camera.Up * coords.y;
   
	SRay ray;
	ray.Origin = camera.Position;
	ray.Origin.w = 0;
	ray.Direction = normalize ( direction );
	ray.Direction.w = 0;
	return ray;
}

#define K_A 0.2F

#define K_D 0.8F

#define K_S 0.8F

#define P 64.0F

float4 reflect(float4 vec, float4 normal)
{
	return vec - normal * 2 * dot(vec, normal);
}
float4 Phong ( float4 point, float4 normal, float4 color, SCamera Camera )
{
	float4 Unit = (float4) ( 1.0F, 1.0F, 1.0F, 1.0F );

	float4 light = normalize ( Camera.Position - point );
   
	float diffuse = fabs ( dot ( light, normal ) );

	float4 reflectVec = reflect ( -light, normal );

	float specular = pow ( fabs ( dot ( reflectVec, light ) ), P );

	return K_A * Unit + diffuse * ( K_D * color + K_S * specular * Unit );
}

float4 Raytrace ( SRay ray, SCamera Camera )
{
	float4 BoxMinimum = (float4) ( -5.0F, -5.0F, -5.0F, 0.0F );

	float4 BoxMaximum = (float4) ( 5.0F, 5.0F, 5.0F, 0.0F );

	float4 result = (float4) ( 0.0F, 0.0F, 0.0F, 1.0F );
    
	float start, final, time;

#ifdef USE_BOX

	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, &start, &final ) )

#else

	if ( IntersectSphere ( ray, Radius, &start, &final ) )

#endif
	{
		
		if ( IntersectSurface ( ray, start, final, &time ) )
		{
			
			float4 point = ray.Origin + ray.Direction * time;
					
			float4 normal = CalcNormal ( point );

			float4 color = ( point - BoxMinimum ) / ( BoxMaximum - BoxMinimum );

			result = Phong ( point, normal, color, Camera );
		}
	}
	return result;
}

//OpenCL kernel
__kernel void main (__global float4 * texture,	
			        __constant float4 Position,
				    __constant float4 Side,
				    __constant float4 Up,
				    __constant float4 View,
				    __constant float2 Scale)
{
	SCamera Camera;

	Camera.Position		= Position;
	Camera.Side			= Side;
	Camera.Up			= Up;
	Camera.View			= View;
	Camera.Scale		= Scale;

	__private int2 TexCoord = (int2)(get_global_id(0), get_global_id(1));

	__private SRay ray = GenerateRay ( Camera, TexCoord );

	texture[512*TexCoord.y + TexCoord.x] = Raytrace ( ray, Camera );
}
