#version 460
#extension GL_EXT_ray_tracing : require

/*
 * inputs
 */
layout(location = 1) rayPayloadInEXT bool shadowed;

/*
 * functions
 */
void main() {
	shadowed = false;
}