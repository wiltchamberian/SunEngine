#include "Game.h"

const char* g_rcdir="D:/GIT/testproj/MeteorEngine/GameDemo/";

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

std::shared_ptr<MeteorGameObject> buildCube()
{
    std::vector<std::string> filenames = {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    std::vector<float> pts = {
        0,0,
        1,0,
        1,1,
        1,1,
        0,1,
        0,0
    };
    std::shared_ptr<MeteorGameObject> ptr(new MeteorGameObject);
    for (int i = 0; i < 6; ++i) {
        GLMesh& mesh = ptr->addMesh();
        for (int j = 0; j < 6; ++j) {
            GLVertex v;
            v._pt.setX(skyboxVertices[(i * 6+j)*3 + 0]);
            v._pt.setY(skyboxVertices[(i * 6 + j) * 3 + 1]);
            v._pt.setZ(skyboxVertices[(i * 6 + j) * 3 + 2]);
            v._textureCoord.set(pts[j*2+0],pts[j*2+1]);

            mesh.addVertex(v);
        }
        mesh.loadTexture(std::string(g_rcdir) + filenames[i]);
        mesh.setIsDrawElements(false);
        mesh.initVAO();
    }
    return ptr;
}


