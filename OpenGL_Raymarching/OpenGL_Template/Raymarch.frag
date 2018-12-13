#version 330 core

out vec4 FragColor;

in vec2 fragCoord;

// Raymarch shader

int X_RESOLUTION = 1280;
int Y_RESOLUTION = 720;
float EPSILON = 0.0001;

float sdfSphere(vec3 p, float r)
{
	return length(p) - r;
}

// copypaste
float sdfMandelbulb(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < 50; i++) {
		r = length(z);
		if (r> 4.0) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, 8-1.0)*8*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,8);
		theta = theta*8;
		phi = phi*8;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

float sdfScene(vec3 p)
{
	p.x = mod(p.x+1.0, 2.0) - 1.0;
	//float d = sdfSphere(p, 0.5);
	float d = sdfMandelbulb(p);
	return d;
}
vec3 estimateSdfNormal(vec3 p)
{
	return normalize(vec3(
		sdfScene(vec3(p.x + EPSILON, p.y, p.z)) - sdfScene(vec3(p.x - EPSILON, p.y, p.z)),
		sdfScene(vec3(p.x, p.y + EPSILON, p.z)) - sdfScene(vec3(p.x, p.y - EPSILON, p.z)),
		sdfScene(vec3(p.x, p.y, p.z + EPSILON)) - sdfScene(vec3(p.x, p.y, p.z - EPSILON))
	));
}

float iSphere(vec3 rayOrigin, vec3 rayDir, float radius)
{
	float b = 2.0 * dot(rayOrigin, rayDir);
	float c= dot(rayOrigin, rayOrigin) - radius*radius;
	float h = b*b - 4.0*c;
	if(h < 0.0) return -1.0;
	float t = (-b -sqrt(h))/2.0;
	return t;
}
float intersect(vec3 rayOrigin, vec3 rayDir)
{
	float t = iSphere(rayOrigin, rayDir, 1.0);
	return t;
}

/*
	fixed4 simpleLambert (fixed3 normal) {
	 fixed3 lightDir = _WorldSpaceLightPos0.xyz; // Light direction
	 fixed3 lightCol = _LightColor0.rgb; // Light color
 
	 fixed NdotL = max(dot(normal, lightDir),0);
	 fixed4 c;
	 c.rgb = _Color * lightCol * NdotL;
	 c.a = 1;
	 return c;
}*/

vec3 simpleLambert(vec3 normal, vec3 col)
{
	vec3 lightDir = vec3(1.0, 2.0, 4.0);
	vec3 lightCol = vec3(1.0, 1.0, 1.0);

	float NdotL = max(dot(normal, lightDir), 0);
	vec3 c = vec3(col * lightCol * NdotL);

	return c;
}

void main()
{
	// uv are fragcoords (pixels) mapped from [xres, yres] to [0, 1]
	vec2 uv = fragCoord.xy /* / vec2(X_RESOLUTION, Y_RESOLUTION))*/ * vec2(1.7777, 1.0);
	//vec2 uv = fragCoord - vec2(X_RESOLUTION, Y_RESOLUTION) / 2.0;
	
	vec3 rayOrigin = vec3(2.0, 0.0, 2.0);	// send rays from camera location
	vec3 rayDir = normalize(vec3(uv, -1.0));	// send one ray per pixel 1.0 clip units into the scene

	// iq video
	/*
	float iDist = intersect(rayOrigin, rayDir);

	vec3 col = vec3(0.0);	// no hit/default = black
	if(iDist > 0.0)
	{
		// if hit draw blue
		col = vec3(0.0, 0.0, 0.5);
	}

	FragColor = vec4(col, 1.0);
	*/
	// naive first raymarch attempt
	float rayDepth = 0.0;
	vec3 col = vec3(0);
	
	for(int i = 0; i < 50; i++)	// Raymarch
	{
		float d = sdfScene(rayOrigin + rayDepth * rayDir);

		if(d < EPSILON){
			col = simpleLambert(estimateSdfNormal(rayOrigin + rayDepth * rayDir), vec3(0.2, 0.2, 0.8));
			break;
		}

		rayDepth += d;

		if(rayDepth >= 100) col = vec3(0.2, 0.2, 0.2);
	}
	FragColor = vec4(col, 1.0);
}