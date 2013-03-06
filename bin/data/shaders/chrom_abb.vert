uniform vec2 uAberrationOffset; // passed in from openFrameworks

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
