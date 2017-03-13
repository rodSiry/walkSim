#include "DebugDrawer.h"
#include <iostream>
DebugDrawer::DebugDrawer()
{

}
void DebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    glBegin(GL_LINES);
    glColor3f(0.f,0.f, 0.f);
    glVertex3f(from.getX(),from.getY(), from.getZ());
    glVertex3f(to.getX(), to.getY(), to.getZ());
    glEnd();

}
DebugDrawer::~DebugDrawer()
{
    //dtor
}
