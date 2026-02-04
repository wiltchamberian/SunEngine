#version 430 core

layout(std140, binding = 0) uniform attractor_block
{
    vec4 attractor[64]; //xyz = position ,w =mass
};

//process particles in blocks of 128
layout (local_size_x = 128) in;

//Buffers containing the positions and velocities of the particles
layout (rgba32f, binding = 0) uniform imageBuffer position_buffer;
layout (rgba32f, binding = 1) uniform imageBuffer velocity_buffer;


//delta time
uniform float dt;

void main(void)
{
    vec4 vel = imageLoad(velocity_buffer, int(gl_GlobalInvocationID.x));
    vec4 pos = imageLoad(position_buffer, int(gl_GlobalInvocationID.x));
    
    int i;

    pos.xyz += vel.xyz * dt;
    pos.w -= 0.0001* dt;

    //for each attractor
    for(int i = 0; i<4; ++i) {
        vec3 dist = (attractor[i].xyz - pos.xyz);
        vel.xyz += dt * dt * attractor[i].w * normalize(dist)/(dot(dist,dist)+10.0);
    }

    if(pos.w<=0.0){
        pos.xyz = -pos.xyz*0.01;
        vel.xyz *=0.01;
        pos.w+=1.0;
    }

    // Store the new position and velocity back into the buffers
    imageStore(position_buffer, int(gl_GlobalInvocationID.x), pos);
    imageStore(velocity_buffer, int(gl_GlobalInvocationID.x), vel);
}