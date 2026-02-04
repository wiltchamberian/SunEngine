#version 430 core

layout(rgba32f, binding = 0) uniform image2D output_image;

//输出的颜色信息
layout(std430, binding =10) buffer ColorBuf{
  vec3 colors[];
}

layout (local_size_x = 1, local_size_y =1) in;

void main(){

    vec3 finalColor = vec3(0,0,0);
    //hardcode ,fixme 
    const int n = 6;
    const int siz = n*n;
    uint startIndex = (gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x) * siz;
    for(int i=0;i<siz;++i){
        finalColor += colors[startIndex];
        startIndex +=1;
    }
    finalColor = finalColor * (1/float(siz));
    imageStore(output_image,ivec2(gl_WorkGroupID.xy),vec4(finalColor,1));
}

/* void main(){

    vec4 finalColor = vec3(0,0,0,0);
    //hardcode ,fixme 
    const uint n = 6;
    const uint siz = n * n;
    uint startIndex = (gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x)*n*n;

    uint x = gl_WorkGroupID.x * n;
    uint y = gl_WorkGroupID.y * n;
  
    for(int i=0;i<siz;++i){
        //finalColor += colors[startIndex];
        uint dy = i/n;
        uint dx = i- dy*n;
        finalColor += texture(output_image,vec2(x+dx,y+dy));
    }
    finalColor = finalColor * (1/float(siz));
    imageStore(output_image,ivec2(gl_WorkGroupID.xy),vec4(finalColor,1));
} */