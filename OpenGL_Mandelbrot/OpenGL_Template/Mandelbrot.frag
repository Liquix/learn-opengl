#version 330 core

out vec4 FragColor;

in vec2 fragCoord;

// MANDELBROT SET SHADER
// The functions in question have the form f(z) = z^2 + c, where c is a complex number.
// For each c, we seed the function by plugging in 0 as the initial value for z.
// Then, we take whatever we get and plug it back in $NUM_ITERATIONS times.
// As we iterate, one of two things can happen:
//		A) The iterates rapidly escape to infinity (or, for practial purposes, anywhere past 2). The input c is not in the mandelbrot set.
//			- Color varies based on how quickly iterates escape to infinity.
//		B) The iterates never exceed 2. The input c is in the mandelbrot set.
//			- Color is usually black (constant) for all values in set.

uniform float scale;
uniform vec2 center;
uniform int max_iterations;
uniform vec3 colors[255];

int X_RESOLUTION = 720;
int Y_RESOLUTION = 720;

// value, curMin, curMax, finalMin, finalMax
float map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

void main()
{
	vec2 z, c;

	vec2 iCenter = vec2(map(center.x, 0, X_RESOLUTION, -2, 1) * 0.3333, map(center.y, 0, Y_RESOLUTION, 0, 1));

	float imag = map(fragCoord.x, 0, 1, -2, 1) * 0.3333;
	float real = fragCoord.y;

	c = vec2(imag + 0.6667, real);

	c *= scale;

	int i;
	z = c;
	for(i = 0; i < max_iterations; i++)
	{
		float x = (z.x * z.x - z.y * z.y) + iCenter.x;
		float y = (z.y * z.x + z.x * z.y) + iCenter.y;

		if((x * x + y * y) > 4.0)	break;
		z.x = x;
		z.y = y;
	}

	float q = float(i) / 100.0;
	vec3 desiredColor = colors[i].xyz / 255.0f;
	desiredColor.x /= 4.0f;

	if(i == max_iterations)		FragColor = vec4(0, 0, 0, 1.0);
	else						FragColor = vec4(desiredColor, 1.0);//FragColor = vec4(q * 0.2, q * 0.7, q, 1.0);
	/*
	else // Experimental continuous coloring escape-time algorithm
	{
		float log_two = log(2.0);
		float log_zn = log(x*x + y*y) / 2.0;
		float nu = log(log_zn / log_two) / log_two;
		float normalized_iteration = float(i) + 1.0 - nu;
		float interp = fract(normalized_iteration);
		vec3 first_color = colors[int(mod(i, color_count))];
		vec3 second_color = colors[int(mod(i + 1.0, color_count))];
		FragColor = vec4(mix(first_color, second_color, interp), 1.0);
	}*/
	
}