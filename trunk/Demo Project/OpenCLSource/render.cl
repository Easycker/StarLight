#define DELTA 0.3f


#define USE_TEXTURE

typedef struct _SRay
{
	float4 Origin;
	
	float4 Direction;
}SRay, *PSRay;

#define plain_normal (float4)( -1.0F, 0.0F, 0.0F, 0.0F )
#define plain_x	     (float4)( 0.0F, 1.0F, 0.0F, 0.0F )
#define plain_y	     (float4)( 0.0F, 0.0F, 1.0F, 0.0F )
#define plain_min_x  -50.0F
#define plain_max_x   50.0F
#define plain_min_y  -50.0F
#define plain_max_y   50.0F

bool IntersectPlain( PSRay pRay, float4* normal , float* time , float4* color )
{
	float4 origin = pRay->Origin;
	origin.w = 1;
	float tmp = -dot( origin , plain_normal );
	
	float4 direction = pRay->Direction;
	direction.w = 0;
	tmp /= dot( direction , plain_normal );
	
	if( tmp < 0 )
		return false;

	*time = tmp;
	
	float4 point = pRay->Origin + *time * pRay->Direction;
	
	float x_val = dot( point , plain_x );
	float y_val = dot( point , plain_y );	
	
	float color_mix = ( ( (int)( round(x_val/4) + round(y_val/4) ) % 2 ) == 0 );
		
	*color = mix( (float4) ( 0.0F, 0.0F, 0.0F, 0.0F ) , (float4) ( 1.0F, 1.0F, 1.0F, 0.0F ) , color_mix );	
	
	*normal = plain_normal;
	if( dot( *normal , pRay->Direction ) > 0 )
		*normal = -*normal;
	
	return ( x_val >= plain_min_x ) && 
	       ( x_val <= plain_max_x ) && 
	       ( y_val >= plain_min_y ) && 
	       ( y_val <= plain_max_y );
 	
}

#define sphere_center (float4)(-10.0F , 0.0F ,0.0F , 0.0F )
#define sphere_radius 10.0F

bool IntersectSphere ( PSRay pRay, float4* normal , float* time , float4* color )
{

	float eps = 1e-2;

	float4 base = pRay->Origin - sphere_center;

	float A = dot ( pRay->Direction, pRay->Direction );

	float B = dot ( pRay->Direction, base );

	float C = dot ( base , base ) - sphere_radius * sphere_radius;

	float D = B * B - A * C;
	
	if ( D > eps )
	{
		D = sqrt ( D );

		float start = ( -B - D ) / A;
		float final = ( -B + D ) / A;
		
		if( start > eps )
			*time = start;
		else *time = final;
			
		*color = (float4)( 1.0F , 0.0F , 0.0F , 0.0F );
		
		float4 point = pRay->Origin + *time * pRay->Direction;
		
		*normal = normalize( point - sphere_center );
		
		if( start < eps && final > eps )
			*normal = -*normal;

		return *time > eps;
	}

	return false;
}

void GenerateRay (  PSRay pRay,
				    const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale)
{
	float2 coords = (float2)((float)get_global_id(0)/256.0f - 1.0f, (float)get_global_id(1)/256.0f - 1.0f) * Scale;
	
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

#define light_position (float4)( -100.0F, 0.0F, 0.0F, 0.0F )

float4 Phong ( float4 point, float4 normal, float4 color, __constant float4 Position )
{
	float4 Unit = (float4) ( 1.0F, 1.0F, 1.0F, 1.0F );

	float4 view = normalize ( Position - point );
	float4 light = normalize ( light_position - point );
   
	float diffuse = max( dot ( light, normal ) , 0.0F );

	float4 reflectVec = reflect ( -light, normal );

	float specular = native_powr ( max( dot ( reflectVec, view ) , 0.0F ), P );

	return K_A * Unit + diffuse * ( K_D * color + K_S * specular * Unit );
}

#define refract_depth 5

#define inner_n 1.2F

float4 Refract( float4 incoming_ray , float4 normal , bool inner )
{
		
	float n = inner_n;
	
	if( !inner )
		n = 1/n;
		
	float cosi = dot( normal , incoming_ray );
	float sin2 = n * n * ( 1.0 - cosi*cosi );
	if( sin2 > 1 )
		return reflect( incoming_ray,normal);

	return n * incoming_ray - ( n*cosi + sqrt( 1.0 - sin2 ))*normal;
}

bool RaytraceStep( PSRay pRay , const float4 Position , float4* color , float* color_mul , PSRay new_ray )
{
	float  sp_time;
	float4 sp_normal;
	float4 sp_color;
	
	if( !IntersectSphere( pRay , &sp_normal , &sp_time , &sp_color ) )
		sp_time = INFINITY;
	
	float  pl_time;
	float4 pl_normal;
	float4 pl_color;
	
	if( !IntersectPlain( pRay , &pl_normal , &pl_time , &pl_color ) )
		pl_time = INFINITY;
		
	if( sp_time < pl_time )
	{
	
		*color	   = (float4)( 0.0F, 0.0F, 0.0F, 0.0F ); 
		*color_mul = 1.0F;
		
		float4 point = pRay->Origin + sp_time * pRay->Direction;
		
		new_ray->Origin = point;
		new_ray->Direction = Refract( normalize( pRay->Direction ) , sp_normal , dot( sp_normal , point - sphere_center ) < 0.0F );
		new_ray->Origin.w = 0;
		new_ray->Direction.w = 0;
		
		return true;
	}
	
	if( pl_time < sp_time )
	{
	
		float4 point = pRay->Origin + pl_time * pRay->Direction;
		
		*color = Phong( point , pl_normal , pl_color , Position );
		
		return false;
		
	}
	
	*color	   = (float4)( 0.0F, 0.0F, 0.0F, 0.0F ); 
	*color_mul = 1.0F;
	
	return false;
	
}

float4 Raytrace ( PSRay pRay, const float4 Position)
{
	
	float4 color;
	float4 ret_color = (float4)( 0.0F, 0.0F, 0.0F, 0.0F );
	float  color_mul = 1.0F;
	
	for( int i = 0 ; i < refract_depth ; i++ )
	{
		SRay new_ray;
		float new_mul;
		
		bool not_last = RaytraceStep( pRay , Position , &color , &new_mul , &new_ray );
		
		ret_color += color * color_mul;
		
		if( !not_last )
			break;
		
		color_mul *= new_mul;
		pRay = &new_ray;
	}
	
	return ret_color;
}

//OpenCL kernel
__kernel void main (
#ifdef USE_TEXTURE
					__global __write_only image2d_t texture,
#else
					__global float4* texture,
#endif
			        const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale)
{
	__private SRay ray;
	GenerateRay (&ray,
      			 Position,
				 Side,
				 Up,
				 View,
				 Scale);

#ifdef USE_TEXTURE
	write_imagef (texture, (int2)(get_global_id(0), get_global_id(1)), Raytrace ( &ray, Position ) );
#else
	texture[get_global_size(0) * get_global_id(1) + get_global_id(0)] = Raytrace ( &ray, Position ) );
#endif
}