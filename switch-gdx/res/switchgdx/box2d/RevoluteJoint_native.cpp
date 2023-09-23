#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/RevoluteJoint.h>


#include <Box2D/Box2D.h> 
	 
jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetJointAngle_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetJointAngle();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetJointSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetJointSpeed();
}

jbool M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniIsLimitEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->IsLimitEnabled();
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniEnableLimit_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		joint->EnableLimit(flag);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetLowerLimit_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetLowerLimit();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetUpperLimit_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetUpperLimit();
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniSetLimits_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat lower, jfloat upper) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		joint->SetLimits(lower, upper );
}

jbool M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniIsMotorEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->IsMotorEnabled();
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniEnableMotor_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		joint->EnableMotor(flag);
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniSetMotorSpeed_long_float(jcontext ctx, jobject self, jlong addr, jfloat speed) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		joint->SetMotorSpeed(speed);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetMotorSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetMotorSpeed();
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniSetMaxMotorTorque_long_float(jcontext ctx, jobject self, jlong addr, jfloat torque) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		joint->SetMaxMotorTorque(torque);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetMotorTorque_long_float_R_float(jcontext ctx, jobject self, jlong addr, jfloat invDt) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetMotorTorque(invDt);
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetReferenceAngle_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetReferenceAngle();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RevoluteJoint_jniGetMaxMotorTorque_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RevoluteJoint* joint = (b2RevoluteJoint*)addr;
		return joint->GetMaxMotorTorque();
}

