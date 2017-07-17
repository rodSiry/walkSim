#version 330 

in vec3 in_Vertex;
in vec3 in_Normals;
uniform mat4 projection; 
uniform mat4 model; 
uniform mat4 view; 
out vec3 normals;
void main()
{
    gl_Position=projection*view*model*vec4(0.5*in_Vertex,1.);
    normals=vec3(model*vec4(in_Normals,0.));
}


