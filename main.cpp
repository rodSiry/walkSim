#include <iostream>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <DebugDrawer.h>
#include <Simulation.h>
#include <unistd.h>
#include "IO.h"

GLint			 att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo*             vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;
Display                *dpy;
Window                 root;

int main(int argc, char *argv[]) {

	dpy = XOpenDisplay(NULL);

	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(0);
	}
	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0.,att);

	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	} else {
		printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}


	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;

	win = XCreateWindow(dpy, root, 0, 0, 1, 1, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Bullet Debug Draw");

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	glEnable(GL_DEPTH_TEST);

	XNextEvent(dpy, &xev);
	Simulation* world=new Simulation();
	IOSocket sock("socket");
	while(1) {

		if(xev.type == Expose) {
			XGetWindowAttributes(dpy, win, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

		}
		else if(xev.type == KeyPress) {
			glXMakeCurrent(dpy, None, NULL);
			glXDestroyContext(dpy, glc);
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
			exit(0);
		}
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float rpt=gwa.width/gwa.height;
		glFrustum(-1.f,  1.f,  -1.f, 1.f,  1.f,  100.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		btVector3 pos=world->GetPos();
		gluLookAt(10.,5.,10.,pos.getX(),pos.getY(),pos.getZ(),0.,1.,0.);
		world->ComputeServos();
		world->Draw();
		double f=world->GetFitness();
		sock.Write(&f, sizeof(double));
		double rData[6];
		sock.Recv(rData);
		world->ChngTarget(rData);
		if(world->CheckFall())
		{
			delete world;
			world=new Simulation();
		}
			glXSwapBuffers(dpy, win);
	}
	delete world;
}
