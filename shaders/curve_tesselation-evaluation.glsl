#version 450 core

layout(isolines) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 basis; // curve basis matrix (e.g., Bezier, B-spline, etc.)

void main()
{

	float t = gl_TessCoord.x;

	//points
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;
	vec3 p3 = gl_in[3].gl_Position.xyz;

	mat4 G = mat4(
		vec4(p0,0.0),
		vec4(p1,0.0),
		vec4(p2,0.0),
		vec4(p3,0.0)
	);

	// The parameter vector for all cubic curves
	vec4 T = vec4(t*t*t, t*t, t, 1.0);

	// curve evaluation
	vec4 result = G * basis * T;
	vec3 p = result.xyz;

	//Old Bezier specific code
	//vec4 result = T * basis * G;
	//Bezier coefficients
	//float b0 = pow((1.0 - t),3);
	//float b1 = 3.0 * t * (1.0 - t) * (1.0 - t);
	//float b2 = 3.0 * t * t * (1.0 - t);
	//float b3 = pow(t, 3);

	//curve evaluation
	//vec3 p = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;

	gl_Position = projection * view * model * vec4(p, 1.0);

}