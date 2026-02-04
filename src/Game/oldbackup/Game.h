#ifndef __GAME_H
#define __GAME_H

#include "MeteorGameObject.h"
#include <memory>

extern float skyboxVertices[];

extern const char* g_rcdir;

std::shared_ptr<MeteorGameObject> buildCube();

#endif