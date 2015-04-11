#version 400 

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
out vec4 color;

uniform vec4 vOffset;
uniform vec4 vColor;
uniform vec4 DiffuseMat, SpecularMat;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform float Shininess;

void main()
{
	// Tranform position into eye coords, factoring in offset
    vec3 pos = (ModelView * vPosition).xyz;
	
	// Calc the ambient light
    // Compute terms in the illumination equation
    vec4 Ambient = 0.2f * LightColor;

	// Calc the diffuse light
	vec3 Norm = normalize(vNormal);
    vec3 LightDir = normalize(LightPosition.xyz - vPosition.xyz);
    float Diff = max(dot(Norm, LightDir), 0.0);
    vec4 Diffuse = Diff * LightColor;

	// Calc the specular light
	vec3 ViewDir = normalize(pos.xyz - vPosition.xyz);
    vec3 ReflectDir = reflect(-LightDir, Norm).xyz;
	  
    float SpecStr = pow(max(dot(ViewDir, ReflectDir), 0.0), Shininess);

    vec4 Specular = SpecStr * SpecularMat * LightColor;  
    
    gl_Position = Projection * ModelView * (vPosition + vOffset);

    color = (Ambient + Diffuse + Specular) * vColor;
    color.a = 1.0;	
}
