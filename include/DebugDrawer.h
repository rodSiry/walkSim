#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H
#include <btBulletDynamicsCommon.h>
#include <GL/gl.h>
#include <iostream>
class DebugDrawer : public btIDebugDraw
{
    public:
        DebugDrawer();
        void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
        void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){ }
        void reportErrorWarning(const char* warningString){}
        void draw3dText(const btVector3& location,const char* textString){}
        void setDebugMode(int debugMode){mode=debugMode;}
        int	getDebugMode() const {return mode;}
        virtual ~DebugDrawer();
    private:
        int mode;
};

#endif // DEBUGDRAWER_H
