static const char* glsl_rchit_ambient = R"glsl(

#version 460
#extension GL_EXT_ray_tracing : require

struct hitPayload
{
	vec3 hitPosition;
	float hitAttenuation;
	int hitSurfaceIndex;
};

struct SurfaceInfo
{
	vec3 Normal;
	float EmissiveDistance;
	vec3 EmissiveColor;
	float EmissiveIntensity;
	float Sky;
	float SamplingDistance;
	float Padding1, Padding2;
};

layout(location = 0) rayPayloadInEXT hitPayload payload;

layout(set = 0, binding = 5) buffer SurfaceIndexBuffer { int surfaceIndices[]; };
layout(set = 0, binding = 6) buffer SurfaceBuffer { SurfaceInfo surfaces[]; };

void main()
{
	SurfaceInfo surface = surfaces[surfaceIndices[gl_PrimitiveID]];

	if(surface.Sky > 0.0)
	{
		payload.hitAttenuation = 100000.0;
	}
	else
	{
		payload.hitAttenuation = gl_HitTEXT;
	}
}

)glsl";
