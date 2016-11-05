out vec4 color;

uniform vec3 fillColor;

void main()
{
    color = vec4(fillColor, 1.0f); // Set alle 4 vector values to 1.0f
}
