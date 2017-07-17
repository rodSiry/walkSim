/*
 * =====================================================================================
 *
 *       Filename:  mesh.cpp
 *
 *    Description:  Implémentation fonctions mesh
 *
 *        Version:  1.0
 *        Created:  17/07/2017 23:13:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigue Siry, rodsiry@gmail.com
 *   Organization:  Ecole supérieure d'électricité
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../include/mesh.h"
using namespace std;
using namespace glm;
void addSquare(std::vector<float>* vert, std::vector<float>* nm, mat4 trans){
	vector<vec3>base;
	base.push_back(vec3(-1.0f,0.f,1.0f));
	base.push_back(vec3(1.0f,0.f, 1.0f));
	base.push_back(vec3(1.0f,0.f, -1.0f));
	base.push_back(vec3(-1.0f, 0.f,1.0f));
	base.push_back(vec3(1.0f,0.f,-1.0f));
	base.push_back(vec3(-1.0f, 0.f,-1.0f));

	vec3 n=vec3(trans*vec4(0.,1.,0.,0.));
	for(int i(0);i<base.size();i++){
		vec3 cur=vec3(trans*vec4(base[i],1.));
		for(int j(0);j<3;j++){
			nm->push_back(n[j]);
			vert->push_back(cur[j]);
		}
	}
}
void addCube(std::vector<float>* vert, std::vector<float>* nm, mat4 trans){
	for(int i(0);i<4;i++)
		addSquare(vert, nm, trans*rotate(radians(90.f*i), vec3(0.f,1.f,0.f))*rotate(radians(-90.f), vec3(1.f,0.f,0.f))*translate(vec3(0.f,1.f,0.f)));
	for(int i(0);i<2;i++)
		addSquare(vert, nm, trans*rotate(radians(180.f*i), vec3(1.f,0.f,0.f))*translate(vec3(0.f,1.f,0.f)));

}
