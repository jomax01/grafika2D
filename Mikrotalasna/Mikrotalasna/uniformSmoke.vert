#version 330 core

layout(location = 0) in vec2 inPos;    // Input vertex position
layout(location = 1) in vec4 inCol;    // Input color
out vec4 chCol;                        // Output color to fragment shader
uniform vec2 uPos;                     // Uniform vector (position offset)
uniform vec2 center;                   // Uniform vector (circle center)

void main()
{
    vec2 adjustedCenter = inPos - center;

    vec2 scaledCenter = adjustedCenter * (1 + uPos[1] * 3);

    vec2 finalPosition = scaledCenter + center;

    gl_Position = vec4(finalPosition + uPos, 0.0, 1.0);

    chCol = inCol;
}
