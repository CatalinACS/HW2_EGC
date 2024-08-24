#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 carPos;
uniform vec3 object_color;

// Output
out vec3 frag_normal;
out vec3 frag_position;
out vec3 frag_texture;
out vec3 color;

void main()
{
    frag_position = v_position;
    frag_texture = v_texture;
    color = object_color;
    gl_Position = vec4(v_position, 1);
    gl_Position = Projection * View * Model * gl_Position;
    gl_Position.y = gl_Position.y - dot(vec3(carPos.x / 100 - gl_Position.x,carPos.y / 100 - gl_Position.y,carPos.z / 100 - gl_Position.z),
    vec3(carPos.x / 100 - gl_Position.x,carPos.y / 100 - gl_Position.y,carPos.z / 100 - gl_Position.z)) * 0.0101;
}
