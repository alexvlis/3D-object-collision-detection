#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "Eigen/Core"

bool loadOBJ(
        const char * path, //model path.
        std::vector<Eigen::Vector3d> & vertices, //vertices. 
		std::vector<Eigen::Vector3d> & vertexIndices //index of vertives for faces.
);
#endif