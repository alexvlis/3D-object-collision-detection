#include <vector>
#include "Eigen/Core"
#include "objloader.h"


// Very, VERY simple OBJ loader.
// Ignores everything except vertices and faces.
// Writen by Yongliang Yang, modified by Andrew Chinery to use Eigen rather than GLM.

bool loadOBJ(
             const char* path,
             std::vector<Eigen::Vector3d> &vertices,
             std::vector<Eigen::Vector3d> &vertexIndices
             ){
    
    printf("Loading OBJ file %s...\n", path);
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ? \n");
        return false;
    }
    
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) {
            break; // EOF = End Of File. Quit the loop.
        }
        
        // else : parse lineHeader
        
        if ( strcmp( lineHeader, "v" ) == 0 ){
            Eigen::Vector3d vertex;
            fscanf(file, "%lf %lf %lf\n", &vertex(0), &vertex(1), &vertex(2) );
            vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            Eigen::Vector3d vertexIndex;
            fscanf(file, "%lf %lf %lf\n", &vertexIndex(0), &vertexIndex(1), &vertexIndex(2) );
            vertexIndices.push_back(vertexIndex);
        }else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
        
    }
    
    return true;
}
