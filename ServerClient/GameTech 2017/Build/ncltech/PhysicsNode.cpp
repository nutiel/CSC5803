#include "PhysicsNode.h"
#include "PhysicsEngine.h"


void PhysicsNode::IntegrateForVelocity(float dt)
{
	if (invMass > 0.0f) {
		/*//Semi-Implicit Euler
		//linear Velocity
		linVelocity += PhysicsEngine::Instance()->GetGravity() * dt; //force from gravity
		linVelocity += force * invMass * dt; //force from other factors, ->Semi-Implicit Euler
		linVelocity = linVelocity * PhysicsEngine::Instance()->GetDampingFactor(); //damping factor to reduce the force over time to account for errors that might occur

		//angular Velocity
		angVelocity += invInertia * torque * dt; //Calculated according to all the forces, is equivalent to the two calculations above
		angVelocity = angVelocity * PhysicsEngine::Instance()->GetDampingFactor(); //damping factor for the same reasons as above*/

		//RK2
		Vector3 linVelocity_temp = linVelocity;

		linVelocity += PhysicsEngine::Instance()->GetGravity() * dt; //force from gravity

		linVelocity += force * invMass * dt; //force from other factors, ->Semi-Implicit Euler

		linVelocity = linVelocity * PhysicsEngine::Instance()->GetDampingFactor(); //damping factor to reduce the force over time to account for errors that might occur
		linVelocity = (linVelocity + linVelocity_temp) / 2;

		Vector3 angVelocity_temp = angVelocity;

		angVelocity += invInertia * torque * dt; //Calculated according to all the forces, is equivalent to the two calculations above

		angVelocity = angVelocity * PhysicsEngine::Instance()->GetDampingFactor(); //damping factor for the same reasons as above
		angVelocity = (angVelocity + angVelocity_temp) / 2;
	}
}

/* Between these two functions the physics engine will solve for velocity
   based on collisions/constraints etc. So we need to integrate velocity, solve 
   constraints, then use final velocity to update position. 
*/

void PhysicsNode::IntegrateForPosition(float dt)
{
	position += linVelocity * dt;

	orientation = orientation + Quaternion(angVelocity * dt * 0.5f, 0.0f) * orientation;
	orientation.Normalise();
	//Finally: Notify any listener's that this PhysicsNode has a new world transform.
	// - This is used by GameObject to set the worldTransform of any RenderNode's. 
	//   Please don't delete this!!!!!
	FireOnUpdateCallback();
}