#define DELTA 0.3f

/////////////////////////////////////////////////////////////

#define USE_TEXTURE

/////////////////////////////////////////////////////////////

typedef struct _SRay
{
	float4 Origin;
	
	float4 Direction;

}SRay, *PSRay;

/////////////////////////////////////////////////////////////

#define PLAIN_NORMAL (float4)(-1.0F, 0.0F, 0.0F, 0.0F)
#define PLAIN_X	     (float4)(0.0F, 1.0F, 0.0F, 0.0F)
#define PLAIN_Y	     (float4)(0.0F, 0.0F, 1.0F, 0.0F)
#define PLAIN_MIN_X  -50.0F
#define PLAIN_MAX_X   50.0F
#define PLAIN_MIN_Y  -50.0F
#define PLAIN_MAX_Y   50.0F

/////////////////////////////////////////////////////////////

#define SPHERE_CENTER (float4)(-10.0F, 0.0F,0.0F, 0.0F)
#define SPHERE_RADIUS 10.0F

/////////////////////////////////////////////////////////////

#define K_A 0.2F

#define K_D 0.8F

#define K_S 0.8F

#define P 64.0F

/////////////////////////////////////////////////////////////

#define LIGHT_POSITION (float4)(-100.0F, 0.0F, 0.0F, 0.0F)

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
	float2 coords =(float2)((float)get_global_id(0) / 256.0f - 1.0f, (float)get_global_id(1) / 256.0f - 1.0f) * Scale;
	
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

float4 Phong(float4 point,
			  float4 normal,
			  float4 color,
			  __constant float4 Position)
{
	float4 Unit  = (float4)(1.0F, 1.0F, 1.0F, 1.0F);

	float4 view  = normalize(Position - point);
	float4 light = normalize(LIGHT_POSITION - point);

	float diffuse = max(dot(light, normal), 0.0F);

	float4 reflectVec = reflect(-light, normal);

	float specular = native_powr(max(dot(reflectVec, view), 0.0F), P);

	return K_A * Unit + diffuse * (K_D * color + K_S * specular * Unit);
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
	
		*color	   =(float4)(0.0F, 0.0F, 0.0F, 0.0F); 
		*color_mul = 1.0F;
		
		float4 point = pRay->Origin + sp_time * pRay->Direction;
		
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
		
		*color = Phong(point, pl_normal, pl_color, Position);
		
		return false;	
	}
	
	*color	   = (float4)(0.0F, 0.0F, 0.0F, 0.0F); 
	*color_mul = 1.0F;
	new_ray->Origin = (float4)( INFINITY , INFINITY , INFINITY , INFINITY );
	
	return false;
	
}

float4 Raytrace(PSRay pRay, const float4 Position)
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

//OpenCL kernels
__kernel void LightPass(__global float4* photonMap,
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
				 
	photonMap[ get_global_size(0) * get_global_id(1) + get_global_id(0) ] = ForwardRaytrace(&ray, Position);
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
				    __global float4* photonMap
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
	write_imagef(texture,(int2)(get_global_id(0), get_global_id(1)), Raytrace(&ray, Position));
#else
	texture[get_global_size(0) * get_global_id(1) + get_global_id(0)] = Raytrace(&ray, Position));
#endif
}