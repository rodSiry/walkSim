/*
 * =====================================================================================
 *
 *       Filename:  camera.cpp
 *
 *    Description:  Implémentation fonctions de positionnement spatial
 *
 *        Version:  1.0
 *        Created:  15/06/2017 19:12:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rodrigue Siry, rodsiry@gmail.com
 *   Organization:  Ecole Supérieure d'électricité
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../include/camera.h"
using namespace glm;
#define MOUSE_SENS 0.005
void cameraUpdate(camera* cam, const Uint8* keys, SDL_Event* e)
{
        float speed=0.2f;
        float aSpeed=0.1f;
        mat4 rot=rotate(cam->x,vec3(0.f,1.f,0.f))*rotate(cam->y,vec3(1.f,0.f,0.f));
        vec3 front=speed*vec3(rot*vec4(0.f,0.f,1.f,0.f));
        vec3 side=cross(front, vec3(0.f,1.f,0.f));
        if (keys[SDL_SCANCODE_UP])
            cam->pos-=front;
        if (keys[SDL_SCANCODE_DOWN])
            cam->pos+=front;
        if (keys[SDL_SCANCODE_LEFT])
            cam->pos+=side;
        if (keys[SDL_SCANCODE_RIGHT])
            cam->pos-=side;
   	while(SDL_PollEvent(e))
	    switch(e->type){
	    	case(SDL_MOUSEMOTION):
			cam->x-=float(e->motion.xrel)*MOUSE_SENS;
			cam->y-=float(e->motion.yrel)*MOUSE_SENS;
			break;
    	    }	    
	 if(abs(cam->y)>M_PI_2)
		cam->y=M_PI_2*float(cam->y)/float(abs(cam->y));

}
mat4 cameraView(camera* cam)
{
   return inverse(translate(cam->pos)*rotate(cam->x,vec3(0.f,1.f,0.f))*rotate(cam->y,vec3(1.f,0.f,0.f)));
}
