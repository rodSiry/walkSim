#ifndef SIMULATION_H
#define SIMULATION_H
#include <btBulletDynamicsCommon.h>
#include <DebugDrawer.h>
#include <vector>
#include <glm/glm.hpp>
class Simulation
{
    public:
        Simulation();
        bool CheckFall();
	void Draw();
        void ChngTarget(double* v);
	btVector3 GetPos();
	void ComputeServos();
       	float GetFitness();
	void AddBody(btRigidBody* b);
        void AddParr(float mass, float x, float y, float z, btVector3 p);
        virtual ~Simulation();
    protected:
	double scal;
	btVector3 p;
	double fit;
        DebugDrawer deb;
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver*  solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
        btCollisionShape* groundShape;
        btCollisionShape* fallShape;
        std::vector<float>values;
        std::vector<btRigidBody*> bodies;
        std::vector<btCollisionShape*> shapes;
        std::vector<btHingeConstraint*> cs;
};

#endif // SIMULATION_H
