/*
 * =====================================================================================
 *
 *       Filename:  mesh.h
 *
 *    Description:  Creation mesh
 *
 *        Version:  1.0
 *        Created:  17/07/2017 23:07:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigue Siry, rodsiry@gmail.com
 *   Organization:  Ecole supérieure d'électricité
 *
 * =====================================================================================
 */

#ifndef  MESH_INC
#define  MESH_INC
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void addSquare(std::vector<float>* vert, std::vector<float>* nm, glm::mat4 trans); 
void addCube(std::vector<float>* vert, std::vector<float>* nm, glm::mat4 trans); 
#endif   /* ----- #ifndef MESH_INC  ----- */
