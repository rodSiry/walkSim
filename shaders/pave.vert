#version 330 

in vec3 in_Vertex;
in vec3 in_Normals;
uniform mat4 projection; 
uniform mat4 model; 
uniform mat4 view; 
uniform vec3 dim;
out vec3 normals;
void main()
{
    vec3 vert=vec3(in_Vertex.x*dim.x,in_Vertex.y*dim.y,in_Vertex.z*dim.z);
    gl_Position=projection*view*model*vec4(vert,1.);
    normals=vec3(model*vec4(in_Normals,0.));
}


