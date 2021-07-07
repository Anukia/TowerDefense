#version 330

in vec4 vs_vertex;
in vec4 vs_normal;
in vec2 vs_texcoord;

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in float i_nl;
in float i_nl2;
in vec2 i_tc;

void main(void) {
    vec4 color=texture(tex,i_tc);

	//Ambient light
	vec3 ambientLight = vec3(0.1f,0.1f,0.1f);

	//Diffuse light
	vec3 posToLightDirVec = normalize(vs_vertex.xyz - (i_nl + i_nl2) / 2.0f);
	vec3 diffuseColor = vec3(1.0f,1.0f,1.0f);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal.xyz), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;

	pixelColor=vec4(color.rgb* i_nl * i_nl2,color.a) + vec4(diffuseFinal,1.0f); //+ albo * albo -
}