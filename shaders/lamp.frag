out vec4 color;

uniform vec3 lampColor;

void main()
{
    color = vec4(lampColor, 1.0f); // Set alle 4 vector values to 1.0f
}
