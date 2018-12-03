#version 330 core

out vec4 FragColor;

in vec2 fragCoord;

void main()
{
	vec2 z, c;

	//c.x = 1.3333 * (fragCoord.x - 0.5);
	//c.y = (fragCoord.y - 0.5);

	c.x = 1.3333 * (fragCoord.x - 0.5);
	c.y = fragCoord.y;

	// zoom
	c = c / 2;

	int i;
	z = c;
	for(i = 0; i < 50; i++)
	{
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 4.0)	break;
		z.x = x;
		z.y = y;
	}

	float q = float(i) / 100.0;

	if(i == 50)		FragColor = vec4(0, 0, 0, 1.0);
	else			FragColor = vec4(q * 0.2, q * 0.7, q, 1.0);
}
