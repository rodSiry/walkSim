/*
 * =====================================================================================
 *
 *       Filename:  Pave.h
 *
 *    Description:  Affichage parallélépipède Rectangle
 *
 *        Version:  1.0
 *        Created:  16/07/2017 22:00:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigue Siry, rodsiry@gmail.com
 *   Organization:  Ecole supérieure d'électricité
 *
 * =====================================================================================
 */

#ifndef  PAVE_INC
#define  PAVE_INC
#include <GL/glew.h>
#include <string>
#include <GL/glx.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include "vao.h"
#include "shader.h"
#include "mesh.h"
#include <vector>

class Pave{
	public: 
		Pave();
		~Pave();
	void	Draw(float x, float y, float z, glm::mat4 projection, glm::mat4 model, glm::mat4 view, glm::vec3 dim);
	private:
		Program shader;
		vao data;
};
#endif   /* ----- #ifndef PAVE_INC  ----- */
