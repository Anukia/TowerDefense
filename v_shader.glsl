#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Pozycja èrÛde≥ úwiat≥a
uniform vec4 lightDir=vec4(5,10,3,0);
uniform vec4 lightDir2=vec4(-5,10,5,0);

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w wierzcholku
layout (location=2) in vec2 texCoord; //wspÛ≥rzÍdne teksturowania

out vec4 vs_vertex;
out vec4 vs_normal;
out vec2 vs_texcoord;

//Zmienne interpolowane
out vec2 i_tc;
out float i_nl;
out float i_nl2;

void main(void) {
    gl_Position=P*V*M*vertex;

    vs_vertex = M * vertex;
    vs_texcoord = vec2(texCoord.x, texCoord.y * -1.0f);
    vs_normal = mat4(M) * vec4(normal.xyz, -1.0f);

    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl=clamp(dot(n,lightDir),0,1); //dot - iloczyn skalarny
    i_nl2=clamp(dot(n,lightDir2),0,1);
    i_tc=texCoord;
}