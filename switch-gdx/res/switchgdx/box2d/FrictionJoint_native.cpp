#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/FrictionJoint.h>


#include <Box2D/Box2D.h> 
	 
void M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniSetMaxForce_long_float(jcontext ctx, jobject self, jlong addr, jfloat force) {

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		joint->SetMaxForce( force );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniGetMaxForce_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		return joint->GetMaxForce();
}

void M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniSetMaxTorque_long_float(jcontext ctx, jobject self, jlong addr, jfloat torque) {

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		joint->SetMaxTorque( torque );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_FrictionJoint_jniGetMaxTorque_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2FrictionJoint* joint = (b2FrictionJoint*)addr;
		return joint->GetMaxTorque();
}

