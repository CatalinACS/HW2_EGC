#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 color;
in vec3 frag_normal;
in vec3 frag_position;
in vec3 frag_texture;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_texture;
layout(location = 3) out vec4 out_position;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color,1);
    out_normal = vec4(frag_normal,1);
    out_texture = vec4(frag_texture,1);
    //out_position = vec4(frag_position, 1);
}
