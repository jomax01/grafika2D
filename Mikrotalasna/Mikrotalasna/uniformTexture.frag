#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;
uniform float time;

void main() {
    // Calculate flicker intensity using a sine wave
    float intensity = abs(sin(2*time));
    vec4 texColor = texture(uTex, chTex);
    
    // Apply the flicker intensity to the texture color
    outCol = texColor * intensity;
}
