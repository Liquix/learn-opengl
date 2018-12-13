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

//uniform vec2 center;
uniform float scale;
uniform vec2 center;

void main()
{
	vec2 z, c;
	int max_iterations = 50;

	//c.x = 1.3333 * fragCoord.x - 0.5;
	//c.y = fragCoord.y;
	c = fragCoord + center;
	c.x = c.x * 1.3333 - 0.5;
	//c.x = fragCoord.x * (1.3333) - 0.5;		// Why multiply by aspect ratio here? I understand the X offset
	//c.y = fragCoord.y * (480 - 0) / 480 + 0;

	// zoom
	c = c * scale;

	int i;
	z = c;
	for(i = 0; i < max_iterations; i++)
	{
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 4.0)	break;
		z.x = x;
		z.y = y;
	}

	float q = float(i) / 100.0;

	if(i == max_iterations)		FragColor = vec4(0, 0, 0, 1.0);
	else						FragColor = vec4(q * 0.2, q * 0.7, q, 1.0);
}
