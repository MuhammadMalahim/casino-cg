#version 430

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;


out vec4 fs_out_col;

uniform bool isLampOn = true;
uniform int state = 0;
uniform int chipCol = 0;
uniform int cardCount = 5;
uniform int cardType = 0;

uniform sampler2D texImage;

// light source properties
uniform vec4 lightPos = vec4( 0.0, 1.0, 0.0, 0.0);
uniform vec3 cameraPos = vec3(5);

uniform vec3 La = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );

uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;

// material properties

uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );
uniform vec3 Ks = vec3( 1.0 );

uniform float Shininess = 20.0;

struct LightProperties
{
	vec4 pos;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct MaterialProperties
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

vec3 lighting(LightProperties light, vec3 position, vec3 normal, MaterialProperties material)
{
	vec3 toLight = light.pos.xyz - position;
	if (light.pos.w == 0)
	{
		toLight = light.pos.xyz;
	}
	float dist = length(toLight);
	vec3 l = normalize(toLight);
	float di = max(dot(l,normal),0.0); // = length(l)*length(normal)*cosTheta
	vec3 diffuseCol = di*light.Ld*material.Kd;

	vec3 r = 2.0*dot(normal,l)*normal-l;
	vec3 v = normalize(cameraPos-position);
	float si = pow(clamp(dot(r,v),0.0,1.0),material.Shininess);
	if (di == 0) 
	{
		si = 0;
	}
	vec3 specularCol = si*light.Ls*material.Ks;
	float atten = dist*dist*light.quadraticAttenuation + dist*light.linearAttenuation + light.constantAttenuation;
	if (light.pos.w == 0)
	{
		atten = light.constantAttenuation;
	}
	return (diffuseCol+specularCol)/atten;
}


void main()
{
	//vec3 n = normalize(vs_out_norm);
	//outputColor = vec4(n,1);
	//return;
	
	LightProperties light;
	light.pos = lightPos;
	light.La = La;
	light.Ld = Ld;
	light.Ls = Ls;
	light.constantAttenuation = lightConstantAttenuation;
	light.linearAttenuation = lightLinearAttenuation;
	light.quadraticAttenuation = lightQuadraticAttenuation;

	MaterialProperties material;
	material.Ka = Ka;
	material.Kd = Kd;
	material.Ks = Ks;
	material.Shininess = Shininess;


	vec3 shadedColor = lighting(light, vs_out_pos, normalize(vs_out_norm), material);

	if(state == 2) {
		
		vec2 texCoord = vs_out_tex;
		texCoord.x /= float(5);
		texCoord.x += float(cardType) / float(5);

		vec4 texColor = texture( texImage, texCoord );
		fs_out_col = (isLampOn ? vec4(shadedColor, 1) : vec4(1)) * texColor;

		return;
	}



	vec4 texColor = texture( texImage, vs_out_tex );

	
	if(state == 1) {
		switch(chipCol) {
			case 0:
				texColor.rgb = vec3(texColor.r, texColor.g, texColor.b);
				break;
			case 1:
				texColor.rgb = vec3(texColor.g, texColor.r, texColor.b);
				break;
			case 2:
				texColor.rgb = vec3(texColor.b, texColor.g, texColor.r);
				break;
			case 3:
				texColor.rgb = vec3(texColor.r, texColor.r, texColor.b);
				break;
			case 4:
				texColor.rgb = vec3(texColor.r, texColor.g, texColor.r);
				break;
			case 5:
				texColor.rgb = vec3(texColor.g, texColor.r, texColor.r);
				break;
		}
	} 


	fs_out_col = (isLampOn ? vec4(shadedColor, 1) : vec4(1)) * texColor;
		
}
