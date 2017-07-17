#version 130

out vec4 color;
in vec3 normals;
void main()
{
	vec3 light=vec3(0.,1.,1.);	
	float l=0.5+0.3*dot(light,normals);
	color=vec4(l,l,l, 1.);
}
