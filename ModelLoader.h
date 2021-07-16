#pragma once
#include "Types.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelResource loadModel(const std::string& file);