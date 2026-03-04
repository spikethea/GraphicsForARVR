#version 450 core
layout(quads, fractional_even_spacing, ccw) in;
void main() {
    gl_Position = gl_in[0].gl_Position;
    vcfhv
}