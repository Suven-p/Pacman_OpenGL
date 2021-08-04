#version 330 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

uniform float textureColorMix = 0.0;

void main() {
    vec4 tempColor, tempColor2;
    tempColor = texture(texture1, TexCoord);
    tempColor2 = mix(tempColor, ourColor, textureColorMix);
    // if (tempColor2.a < 0.1) {
    //     discard;
    // }
    FragColor = tempColor2;
}
