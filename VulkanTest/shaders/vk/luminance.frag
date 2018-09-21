#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (input_attachment_index = 0, binding = 0) uniform subpassInput colorInput;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 c = subpassLoad(colorInput).rgb;
	float l = 0.212671 * c.x + 0.715160 * c.y + 0.072169 * c.z; 
    outColor = vec4(vec3(l), 1);
    //outColor = vec4(c, 1);
}