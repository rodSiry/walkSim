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
    btMatrix3x3 trans= t.getBasis();
	btVector3 v=(trans.getColumn(1)).normalize();
	scal=v.dot(btVector3(0.f, 1.f, 0.f)); /*  
                                              

		return true;
	else
		return false;*/
    btVector3 pP=p;
	p=bodies[1]->getCenterOfMassPosition();
    if(count>1000|| p.getY()<1)
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
    count=0;
	fit=0.;
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	float up=1.5f;
	AddPlane();
	AddParr(1., 0.25, 1., 0.25, btVector3(0.,3.75+up,0.));
	AddParr(1., 0.25, 0.25, 0.25, btVector3(1.,3.25+up,0.));
	AddParr(1., 0.25, 0.25, 0.25, btVector3(-1.,3.25+up,0.));
	AddParr(1., 0.25, 0.5, 0.25, btVector3(1.,1.75+up,0.));
	AddParr(1., 0.25, 0.5, 0.25, btVector3(-1.,1.75+up,0.));
	AddParr(1., 0.25, 1., 0.25, btVector3(1.5,1.+up,0.75));
	AddParr(1., 0.25, 1., 0.25, btVector3(-1.5,1.+up,0.75));
	AddParr(1., 0.25, 0.5, 0.25, btVector3(1.25,-0.5+up,0.75));
	AddParr(1., 0.25, 0.5, 0.25, btVector3(-1.25,-0.5+up,0.75));
	values.push_back(30.f);
	values.push_back(-30.f);
	values.push_back(-45.f);
	values.push_back(15.f);
	values.push_back(-15.f);
	values.push_back(-45.f);
	values.push_back(-0.f);
	values.push_back(-0.f);
	btVector3 vel(0,0,0);
	btHingeConstraint* c= new btHingeConstraint(*bodies[1],*bodies[2],btVector3(0.5,-0.5,0.),btVector3(-0.25,0.,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[2],*bodies[4],btVector3(0.,-0.5,0.),btVector3(-0.,0.5,-0.),btVector3(0.,0.,1.),btVector3(0.,0.,1.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[4],*bodies[6],btVector3(0.25,-0.75,0.),btVector3(-0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[1],*bodies[3],btVector3(-0.5,-0.5,0.),btVector3(0.25,0.,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[3],*bodies[5],btVector3(0.,-0.5,0.),btVector3(-0.,0.5,-0.),btVector3(0.,0.,-1.),btVector3(0.,0.,-1.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[5],*bodies[7],btVector3(-0.25,-0.75,0.),btVector3(0.25,0.75,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[6],*bodies[8],btVector3(-0.25,-0.75,0.),btVector3(0.25,0.5,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
	c= new btHingeConstraint(*bodies[7],*bodies[9],btVector3(0.25,-0.75,0.),btVector3(-0.25,0.5,0.),btVector3(1.,0.,0.),btVector3(1.,0.,0.),false);
	c->enableAngularMotor(true, 0., 20.);
	dynamicsWorld->addConstraint(c, false);
	cs.push_back(c);
    for(int i(0);i<cs.size();i++){
        prevState.push_back(0);
        cs[i]->setLimit(-1.5, 1.5);
    }

   //     cs[1]->setLimit(-0.5, 0.5);
    //    cs[4]->setLimit(-0.5, 0.5);
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
    fallRigidBodyCI.m_friction=1.;
	btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
	AddBody(rigidBody);
}	
void Simulation::Draw(mat4 projection, mat4 model, mat4 view)
{
	btTransform t;
	bodies[1]->getMotionState()->getWorldTransform(t);
    btMatrix3x3 trans= t.getBasis();
	btVector3 v=(trans.getColumn(1)).normalize();
	btVector3 pP=p;
	p=bodies[1]->getCenterOfMassPosition();
	double pScal=scal;
	scal=v.dot(btVector3(0.f, 1.f, 0.f));
	fit=(p.getY()+scal);
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
	for(int i(1);i<bodies.size();i++)
		box.Draw(1,1,1,projection, getMat4(bodies[i]), view, dimensions[i-1]);
    count+=1;
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
		float targetVelocity = 0.f;
		if(values[i]!=0.f)
			targetVelocity = values[i]/abs(values[i])*9999.f;
		float maxMotorImpulse = values[i];
		cs[i]->enableAngularMotor(false, targetVelocity,abs(values[i]));
		cs[i]->enableMotor(true);
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
		if(abs(values[i])>=90.f)
			values[i]=70.f*values[i]/abs(values[i]);
	}
}
float Simulation::GetFitness()
{
	return fit;  
}
std::vector<double> Simulation::GetState(){
	std::vector<double> res;
	for(int i(0);i<cs.size();i++)
	{
		res.push_back(90/M_PI*cs[i]->getHingeAngle());
	}
	for(int i(0);i<cs.size();i++)
    {
       res.push_back(90/M_PI*cs[i]->getHingeAngle()-prevState[i]);
       prevState[i]=90/M_PI*cs[i]->getHingeAngle();
    }
    btTransform t;
	bodies[1]->getMotionState()->getWorldTransform(t);
    btMatrix3x3 trans= t.getBasis();
	btVector3 v=(trans.getColumn(1)).normalize();
    res.push_back(v.dot(btVector3(0.f,1.f,0.f)));
	return res;
			
}
