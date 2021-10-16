#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
};

#shader fragment
#version 330 core

out vec4 color;

void main()
{
    gl_FragColor = vec4(0.2, 0.3, 0.5, 1.0);
};