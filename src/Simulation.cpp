#include "Simulation.h"
using namespace glm;
mat4 getMat4(btRigidBody* body){
	btTransform t;
	body->getMotionState()->getWorldTransform(t);
	glm::mat4 res;
	t.getOpenGLMatrix(glm::value_ptr(res));
	return res;
	
	
}
bool Simulation::CheckFall()
{
	btTransform t;
	bodies[1]->getMotionState()->getWorldTransform(t);
	btVector3 v=(t*btVector3(0.f,1.f,0.f)).normalize();
	if(bodies[1]->getCenterOfMassPosition().getY()<4.)
		return true;
	else
		return false;
}
btVector3 Simulation::GetPos()
{
	btVector3 p;
	p=bodies[1]->getCenterOfMassPosition();
	return p;
}
Simulation::Simulation():p(btVector3(0.,0.,0.)), box(Pave())
{
	fit=0.;
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	/*
	   fallShape = new btSphereShape(1.);
	   btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
	   btScalar mass = 1;
	   btVector3 fallInertia(0, 0, 0);
	   fallShape->calculateLocalInertia(mass, fallInertia);
	   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	   btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	   AddBody(fallRigidBody);*/
	float up=1.5f;
	AddPlane();
	AddParr(5., 0.5, 1., 0.5, btVector3(0.,3.75+up,0.));
	AddParr(2., 0.5, 0.5, 0.5, btVector3(1.,3.25+up,0.));
	AddParr(2., 0.5, 0.5, 0.5, btVector3(-1.,3.25+up,0.));
	AddParr(2., 0.25, 1., 0.25, btVector3(1.,2.5+up,0.75));
	AddParr(2., 0.25, 1., 0.25, btVector3(-1.,2.5+up,0.75));
	AddParr(5., 0.25, 1., 0.25, btVector3(1.5,1.+up,0.75));
	AddParr(5., 0.25, 1., 0.25, btVector3(-1.5,1.+up,0.75));
	AddParr(5., 0.25, 1., 0.25, btVector3(1.25,-0.5+up,0.75));
	AddParr(5., 0.25, 1., 0.25, btVector3(-1.25,-0.5+up,0.75));
	values.push_back(30.f);
	values.push_back(-30.f);
	values.push_back(-45.f);
	values.push_back(15.f);
	values.push_back(-15.f);
	values.push_back(-45.f);
	values.push_back(-0.f);
	values.push_back(-0.f);
	btVector3 vel(0,0,0);
	btHingeConstraint* c= new btHingeConstraint(*bodies[1],*bodies[2],btVector3(0.5,-0.5,0.),btVector3(-0.5,0.,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[2],*bodies[4],btVector3(0.,0.,0.5),btVector3(-0.,0.75,-0.25),btVector3(0.,0.,1.),btVector3(0.,0.,1.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[4],*bodies[6],btVector3(0.25,-0.75,0.),btVector3(-0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[1],*bodies[3],btVector3(-0.5,-0.5,0.),btVector3(0.5,0.,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[3],*bodies[5],btVector3(0.,0.,0.5),btVector3(-0.,0.75,-0.25),btVector3(0.,0.,-1.),btVector3(0.,0.,-1.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[5],*bodies[7],btVector3(-0.25,-0.75,0.),btVector3(0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[6],*bodies[8],btVector3(-0.25,-0.75,0.),btVector3(0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[7],*bodies[9],btVector3(0.25,-0.75,0.),btVector3(-0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);



//	bodies[1]->setLinearVelocity(vel);
}

void Simulation::AddParr(float mass, float x, float y, float z, btVector3 p)
{
	dimensions.push_back(vec3(x, y, z));
	btCollisionShape* shp= new btBoxShape(btVector3(x,y,z));
	shapes.push_back(shp);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), p));
	btVector3 fallInertia(0, 0, 0);
	shp->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shp, fallInertia);
	btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
	AddBody(rigidBody);
}
void Simulation::AddPlane()
{
	btCollisionShape* shp= new btStaticPlaneShape(btVector3(0.,1.,0.),0.);
	shapes.push_back(shp);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), p));
	btVector3 fallInertia(0, 0, 0);
	shp->calculateLocalInertia(0, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, shp, fallInertia);
	btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
	AddBody(rigidBody);
}	
void Simulation::Draw(mat4 projection, mat4 model, mat4 view)
{
	btTransform t;
//	if(rand()%10==0)
		//bodies[1]->setLinearVelocity(btVector3(0.,3.,0.));
	bodies[1]->getMotionState()->getWorldTransform(t);
	btVector3 v=(t*btVector3(0.f,1.f,0.f)).normalize();
	btVector3 pP=p;
	p=bodies[1]->getCenterOfMassPosition();
	double pScal=scal;
	scal=v.dot(btVector3(0.f, 1.f, 0.f));
	fit=p.getZ()-pP.getZ();
	std::cout<<fit<<std::endl;
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
	for(int i(1);i<bodies.size();i++)
		box.Draw(1,1,1,projection, getMat4(bodies[i]), view, dimensions[i-1]);
}
void Simulation::AddBody(btRigidBody* b)
{
	bodies.push_back(b);
	dynamicsWorld->addRigidBody(b);
}
void Simulation::ComputeServos()
{
	for(int i(0);i<cs.size();i++)
	{
		float b=((int)values[i]%360)*M_PI/180.f;
		if(b>3.14)
			b=b-2.*3.14;
		cs[i]->setLimit(b,b);
	}

}
Simulation::~Simulation()
{
	for(int i(0);i<bodies.size();i++)
	{
		dynamicsWorld->removeRigidBody(bodies[i]);
		delete bodies[i]->getMotionState();
		delete bodies[i];
	}
	for(int i(0);i<shapes.size();i++)
	{
		delete shapes[i];
	}
	for(int i(0);i<cs.size();i++)
	{
		delete cs[i];
	}

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}
void Simulation::ChngTarget(double* v)
{
	for(int i(0);i<values.size();i++)
	{
		values[i]=v[i];	
	}
}
float Simulation::GetFitness()
{
	return fit;  
}
