// Basic diffuse Shader
#type vertex

#version 460 core
	
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;

out vec2 v_uv;

void main(){
	v_uv = a_uv;
	gl_Position = vec4(a_pos,1.0);
}

#type fragment

#version 460 core
layout(location = 0) out vec4 fragColor;	
layout(location = 1) out int id;	

uniform sampler2DMS u_Texture;
uniform isampler2DMS u_IDs;

in vec2 v_uv;

void main()
{
	ivec2 texturePosition = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	vec4 colorSample0 = texelFetch(u_Texture, texturePosition, 0);
	vec4 colorSample1 = texelFetch(u_Texture, texturePosition, 1);
	vec4 colorSample2 = texelFetch(u_Texture, texturePosition, 2);
	vec4 colorSample3 = texelFetch(u_Texture, texturePosition, 3);

	int textureID = texelFetch(u_IDs, texturePosition, 3).r;
	vec4 antialiased = (colorSample0 + colorSample1 + colorSample2 + colorSample3) / 4.0f;
	
	id = int(textureID);
	fragColor = antialiased;
}