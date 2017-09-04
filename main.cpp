#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <glm/gtx/transform.hpp>
#include "include/IO.h"
#include "include/Simulation.h"
#include "include/camera.h"
#include <vector>

const Uint8 * keys;

int main(int argc, char *argv[]) { 
	int w;
	int h;
	SDL_Window* win(0);
	SDL_GLContext context(0);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	win=SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1800, 1600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context=SDL_GL_CreateContext(win);
	std::cout<<glGetString(GL_VERSION)<<std::endl;
	glewExperimental=GL_TRUE;
	if(glewInit()!=GLEW_OK)
		std::cout<<"glew failed."<<std::endl;
	float c;
	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	glm::mat4 projection=glm::perspective(20.f, float(w)/float(h),0.1f, 100.f);
	glm::mat4 view=glm::lookAt(glm::vec3(0.f,10.f,10.f),glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f));
	glm::mat4 model(glm::mat4(1.f));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Simulation* world=new Simulation();
	camera cam;
	cam.x=0;
	cam.y=0;
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	IOSocket sock("socket");
    std::vector<double> state=world->GetState();
	sock.Write(&state[0], state.size()*sizeof(double));
	while(1) {
		SDL_Event e;
		glViewport(0, 0, w, h);
		c+=0.01;
		glClearColor(.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		keys = SDL_GetKeyboardState(NULL);
		cameraUpdate(&cam, SDL_GetKeyboardState(NULL), &e);
		while(SDL_PollEvent(&e))
			if(e.type==SDL_WINDOWEVENT)
			{
				SDL_GetWindowSize(win, &w, &h);
				glViewport(0, 0, w, h);
				projection=glm::perspective(70.f, float(w)/float(h),0.1f, 100.f);
			}
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			break;
		}
		double rData[8];
		sock.Recv(rData);
		world->ChngTarget(rData);
		world->ComputeServos();
		std::vector<double> state=world->GetState();
		sock.Write(&state[0], state.size()*sizeof(double));
		double f=world->GetFitness();
		sock.Write(&f, sizeof(double));		
        double b;
		world->Draw(projection, model, view);
		if(world->CheckFall())
		{
			delete world;
            b=1.;
		    sock.Write(&b, sizeof(double));
			world=new Simulation();
            std::vector<double> state=world->GetState();
            sock.Write(&state[0], state.size()*sizeof(double));

		}
        else{
            b=0.;
		    sock.Write(&b, sizeof(double));

        }
		view=cameraView(&cam);
		SDL_GL_SwapWindow(win);
	}
	delete world;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
