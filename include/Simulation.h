#ifndef SIMULATION_H
#define SIMULATION_H
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <iostream>
#include "Pave.h"
class Simulation
{
    public:
        Simulation();
        bool CheckFall();
        void Draw(glm::mat4 projection, glm::mat4 model, glm::mat4 view);
        void ChngTarget(double* v);
        btVector3 GetPos();
        void ComputeServos();
        float GetFitness();
        void AddBody(btRigidBody* b);
        std::vector<double> GetState();
        void AddParr(float mass, float x, float y, float z, btVector3 p);
        void AddPlane();
        virtual ~Simulation();
    protected:
        double scal;
        btVector3 p;
        double fit;
	std::queue<double> pP;
	std::queue<double> pScal;
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver*  solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
        btCollisionShape* groundShape;
        btCollisionShape* fallShape;
        std::vector<float>values;
        std::vector<btRigidBody*> bodies;
        std::vector<glm::vec3> dimensions;
        std::vector<btCollisionShape*> shapes;
        std::vector<btHingeConstraint*> cs;
        std::vector<double> prevState;
        Pave box;
        int count; 
};

#endif // SIMULATION_H
