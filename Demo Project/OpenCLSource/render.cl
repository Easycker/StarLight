#define DELTA 0.3f

#define PHOTON_MAP_SIZE 512*512

/////////////////////////////////////////////////////////////

#define USE_TEXTURE

/////////////////////////////////////////////////////////////

typedef struct _SRay
{
	float4 Origin;
	
	float4 Direction;

}SRay, *PSRay;

typedef struct _PhotonMapType
{
	float4 point;
	uint4 voxel;
}PhotonMapType;

/////////////////////////////////////////////////////////////

#define PLAIN_NORMAL (float4)(-1.0F, 0.0F, 0.0F, 0.0F)
#define PLAIN_X	     (float4)(0.0F, 1.0F, 0.0F, 0.0F)
#define PLAIN_Y	     (float4)(0.0F, 0.0F, 1.0F, 0.0F)
#define PLAIN_MIN_X  -20.0F
#define PLAIN_MAX_X   20.0F
#define PLAIN_MIN_Y  -20.0F
#define PLAIN_MAX_Y   20.0F

/////////////////////////////////////////////////////////////

#define SPHERE_CENTER (float4)(-10.0F, 0.0F,0.0F, 0.0F)
#define SPHERE_RADIUS 8.0F

/////////////////////////////////////////////////////////////

#define LIGHT_POSITION (float4)(-25.0F, 0.0F, 0.0F, 0.0F)

/////////////////////////////////////////////////////////////

#define REFRACT_DEPTH 5

#define INNER_N 1.2F

/////////////////////////////////////////////////////////////

bool IntersectPlain(PSRay pRay, float4* normal, float* time, float4* color)
{
	float4 origin = pRay->Origin;
	origin.w = 1;
	
	float4 direction = pRay->Direction;
	direction.w = 0;

	float tmp = -native_divide(dot(origin, PLAIN_NORMAL), dot(direction, PLAIN_NORMAL));
	
	if(tmp < 0)
	{
		return false;
	}

	*time = tmp;
	
	float4 point = pRay->Origin + *time * pRay->Direction;
	
	float x_val = dot(point, PLAIN_X);
	float y_val = dot(point, PLAIN_Y);	
	
	float color_mix =(((int)(round(x_val / 4) + round(y_val / 4)) % 2) == 0);
		
	*color = mix((float4)(0.0F, 0.0F, 0.0F, 0.0F),(float4)(1.0F, 1.0F, 1.0F, 0.0F), color_mix);	
	
	*normal = PLAIN_NORMAL;
	if(dot(*normal, pRay->Direction) > 0)
	{
		*normal = -(*normal);
	}
	
	return(x_val >= PLAIN_MIN_X) && 
	      (x_val <= PLAIN_MAX_X) && 
	      (y_val >= PLAIN_MIN_Y) && 
	      (y_val <= PLAIN_MAX_Y);
 	
}

bool IntersectSphere(PSRay pRay, float4* normal, float* time, float4* color)
{

	float eps = 1e-2;

	float4 base = pRay->Origin - SPHERE_CENTER;

	float A = dot(pRay->Direction, pRay->Direction);

	float B = dot(pRay->Direction, base);

	float C = dot(base, base) - SPHERE_RADIUS * SPHERE_RADIUS;

	float D = B * B - A * C;
	
	if(D > eps)
	{
		D = sqrt(D);

		float start =(-B - D) / A;
		float final =(-B + D) / A;
		
		if(start > eps)
		{
			*time = start;
		}
		else *time = final;
			
		*color =(float4)(1.0F, 0.0F, 0.0F, 0.0F);
		
		float4 point = pRay->Origin + *time * pRay->Direction;
		
		*normal = normalize(point - SPHERE_CENTER);
		
		if((start < eps) && (final > eps))
		{
			*normal = -(*normal);
		}

		return *time > eps;
	}

	return false;
}

void GenerateRay(PSRay pRay,
				    const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale)
{

	float x_coord = 2.0F * (float)get_global_id( 0 ) / (float)get_global_size( 0 ) - 1.0F;
	float y_coord = 2.0F * (float)get_global_id( 1 ) / (float)get_global_size( 1 ) - 1.0F;

	float2 coords =(float2)( x_coord , y_coord ) * Scale;
	
	float4 direction = View - Side * coords.x + Up * coords.y;
   
	pRay->Origin = Position;
	pRay->Origin.w = 0;
	pRay->Direction.w = 0;
	pRay->Direction = normalize(direction);
}

float4 reflect(float4 vec, float4 normal)
{
	return vec - normal * 2 * dot(vec, normal);
}

#define P 64.0F

float4 Phong(float4 point,
			  float4 normal,
			  float4 color,
			  __constant float4 Position , float ka , float kd , float ks )
{
	float4 Unit  = (float4)(1.0F, 1.0F, 1.0F, 1.0F);

	float4 view  = normalize(Position - point);
	float4 light = normalize(LIGHT_POSITION - point);

	float diffuse = max(dot(light, normal), 0.0F);

	float4 reflectVec = reflect(-light, normal);

	float specular = native_powr(max(dot(reflectVec, view), 0.0F), P);

	return ka * Unit + diffuse * (kd * color + ks * specular * Unit);
}

float4 Refract(float4 incoming_ray,
			   float4 normal,
			   bool inner)
{
	float n = INNER_N;
	
	if(!inner)
	{
		n = 1 / n;
	}
		
	float cosi = dot(normal, incoming_ray);
	float sin2 = n * n * (1.0 - cosi * cosi);
	if(sin2 > 1)
	{
		return reflect(incoming_ray, normal);
	}

	return n * incoming_ray - (n * cosi + sqrt(1.0 - sin2)) * normal;
}

#define PLAIN_K_A 0.2F
#define PLAIN_K_D 0.8F
#define PLAIN_K_S 0.8F

#define SPHERE_K_A 0.2F
#define SPHERE_K_D 0.2F
#define SPHERE_K_S 0.8F

bool RaytraceStep(PSRay pRay,
				  const float4 Position,
				  float4* color,
				  float* color_mul,
				  PSRay new_ray)
{
	float  sp_time;
	float4 sp_normal;
	float4 sp_color;
	
	if(!IntersectSphere(pRay, &sp_normal, &sp_time, &sp_color))
	{
		sp_time = INFINITY;
	}
	
	float  pl_time;
	float4 pl_normal;
	float4 pl_color;
	
	if(!IntersectPlain(pRay, &pl_normal, &pl_time, &pl_color))
	{
		pl_time = INFINITY;
	}
		
	if(sp_time < pl_time)
	{
	
		float4 point = pRay->Origin + sp_time * pRay->Direction;
	
		*color	   = Phong(point, sp_normal, sp_color, Position , SPHERE_K_A , SPHERE_K_D , SPHERE_K_S );
		*color_mul = 0.8F;
		
		new_ray->Origin = point;
		new_ray->Direction = Refract(normalize(pRay->Direction), sp_normal, dot(sp_normal, point - SPHERE_CENTER) < 0.0F);
		new_ray->Origin.w = 0;
		new_ray->Direction.w = 0;
		
		return true;
	}
	
	if(pl_time < sp_time)
	{
	
		float4 point = pRay->Origin + pl_time * pRay->Direction;
		
		new_ray->Origin = point;
		new_ray->Origin.w = 0;
		
		*color = Phong(point, pl_normal, pl_color, Position , PLAIN_K_A , PLAIN_K_D , PLAIN_K_S );
		
		return false;	
	}
	
	*color	   = (float4)(0.0F, 0.0F, 0.0F, 0.0F); 
	*color_mul = 1.0F;
	new_ray->Origin = (float4)( INFINITY , INFINITY , INFINITY , INFINITY );
	
	return false;
	
}

#define MIN_POINT (float4)( -25.0F , -25.0F , -25.0F , 0.0F )
#define MAX_POINT (float4)( 25.0F , 25.0F , 25.0F , 0.0F )

#define MAX_VOXEL 200

#define PHOTON_RADIUS 0.25F

uint4 GetVoxel( float4 point )
{
	point -= MIN_POINT;
	point /= PHOTON_RADIUS * 2;
	uint4 ret = (uint4)( 0 , 0 , 0 , 0 );
	ret.x = round( point.x );
	ret.y = round( point.y );
	ret.z = round( point.z );
	return ret;
}
uint GetVoxelIndex( uint x , uint y , uint z )
{
	return ( ( x * MAX_VOXEL + y ) * MAX_VOXEL + z ) * 2;
}

float SummPhotons( float4 point , __global __read_only image2d_t photonImage , uint firstIndex , uint lastIndex )
{

	float ret = 0.0F;
	
	const sampler_t photonSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
	
	for( int j = firstIndex ; j < lastIndex ; j++ )
	{
		float4 photonPoint = read_imagef( photonImage , photonSampler , (int2)( j % 512 , j / 512 ) ); 
		float4 tmp = photonPoint - point;
		tmp.w = 0;
		float range = dot( tmp , tmp );
		ret += max( 0.0F , PHOTON_RADIUS - range ); 
	}
	return ret;
}

float4 Raytrace(PSRay pRay, const float4 Position , __global PhotonMapType* photonMap , __global uint2* voxelData , __global __read_only image2d_t photonImage  )
{
	float4 color;
	float4 ret_color = (float4)(0.0F, 0.0F, 0.0F, 0.0F);
	float  color_mul = 1.0F;
	
	for(int i = 0; i < REFRACT_DEPTH; i++)
	{
		SRay new_ray;
		float new_mul;
		
		bool not_last = RaytraceStep(pRay, Position, &color, &new_mul, &new_ray);
		
		ret_color += color * color_mul;
		
		if(!not_last)
		{
			float photon_mul = 0.0F;
	
			float4 point = new_ray.Origin;
			uint4 voxel = GetVoxel( point );
			
			for( int x = voxel.x - 1 ; x <= voxel.x + 1 ; x++ )
			for( int y = voxel.y - 1 ; y <= voxel.y + 1 ; y++ )
			for( int z = voxel.z - 1 ; z <= voxel.z + 1 ; z++ )
			{
				uint index = GetVoxelIndex( x , y , z );
				uint2 voxel = voxelData[ index / 2 ];
				photon_mul += SummPhotons( point , photonImage , voxel.x , voxel.y );
			}
			
			ret_color += (float4)( 0.01F , 0.01F , 0.01F , 0.0F ) * photon_mul ;
			
			break;
		}
		
		color_mul *= new_mul;
		pRay = &new_ray;
		
	}
	
	return ret_color;
}

float4 ForwardRaytrace(PSRay pRay, const float4 Position)
{
	
	float4 color;
	float new_mul;
	
	for(int i = 0; i < REFRACT_DEPTH; i++)
	{
	
		SRay new_ray;		
		bool not_last = RaytraceStep(pRay, Position, &color, &new_mul, &new_ray);
		
		if(!not_last)
			return new_ray.Origin;
		
		pRay = &new_ray;
	}
	
	return (float4)( INFINITY,INFINITY,INFINITY, 0.0F );
}

__kernel void VoxelClean( __global uint4* voxelData )
{
	size_t index = ( get_global_size(1)*get_global_id(2) + get_global_id( 1 ) ) * get_global_size( 0 ) + get_global_id(0);
	voxelData[ index * 2 ] = (uint4)MAX_VOXEL;
	voxelData[ index * 2 + 1 ] = (uint4)MAX_VOXEL;
}

__kernel void SetupVoxels( __global PhotonMapType* photonMap , __global uint* voxelData )
{

	int index = get_global_size(0) * get_global_id(1) + get_global_id(0);
	uint4 curr_voxel = photonMap[ index ].voxel;

	if( curr_voxel.x >= MAX_VOXEL )
		return;
	
	uint4 prev_voxel = photonMap[ index>0 ? ( index - 1 ) : 0 ].voxel;
	uint4 next_voxel = photonMap[ ( index<PHOTON_MAP_SIZE - 1 ) ? ( index + 1 ) : 0 ].voxel;

	if( index == 0 || prev_voxel.z != curr_voxel.z || prev_voxel.y != curr_voxel.y || prev_voxel.x != curr_voxel.x )
		voxelData[ 2 * ( ( MAX_VOXEL * curr_voxel.x + curr_voxel.y ) * MAX_VOXEL + curr_voxel.z ) ] = index;
	if( index == PHOTON_MAP_SIZE - 1 || next_voxel.z != curr_voxel.z || next_voxel.y != curr_voxel.y || next_voxel.x != curr_voxel.x )
		voxelData[ 2 * ( ( MAX_VOXEL * curr_voxel.x + curr_voxel.y ) * MAX_VOXEL + curr_voxel.z ) + 1 ] = index + 1;
} 

//OpenCL kernels
__kernel void LightPass(__global PhotonMapType* photonMap,
			        const float4 Position,
				    const float4 Side,
				    const float4 Up,			//add or remove any args
				    const float4 View,
				    const float2 Scale)
{
	__private SRay ray;
	
	GenerateRay(&ray,
      			 Position,
				 Side,
				 Up,
				 View,
				 Scale);
	size_t index = get_global_size(0) * get_global_id(1) + get_global_id(0);
	float4 point = 	ForwardRaytrace(&ray, Position); 
	photonMap[ index ].point = point;
	photonMap[ index ].voxel = GetVoxel( point );
}
__kernel void ViewPass(
#ifdef USE_TEXTURE
					__global __write_only image2d_t texture,
#else
					__global float4* texture,
#endif
			        const float4 Position,
				    const float4 Side,
				    const float4 Up,
				    const float4 View,
				    const float2 Scale,
				    __global PhotonMapType* photonMap,
				    __global uint2* voxelData,
				    __global __read_only image2d_t photonImage
					)
{
	__private SRay ray;
	GenerateRay(&ray,
      			 Position,
				 Side,
				 Up,
				 View,
				 Scale);

#ifdef USE_TEXTURE
	write_imagef(texture,(int2)(get_global_id(0), get_global_id(1)), Raytrace(&ray, Position , photonMap , voxelData , photonImage ));
#else
	texture[get_global_size(0) * get_global_id(1) + get_global_id(0)] = Raytrace(&ray, Position, photonMap , voxelData , photonImage ));
#endif
}