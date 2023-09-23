#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/PrismaticJoint.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetLocalAxisA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		anchor[0] = joint->GetLocalAxisA().x;
		anchor[1] = joint->GetLocalAxisA().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetJointTranslation_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetJointTranslation();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetJointSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetJointSpeed();
}

jbool M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniIsLimitEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->IsLimitEnabled();
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniEnableLimit_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		joint->EnableLimit(flag);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetLowerLimit_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetLowerLimit();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetUpperLimit_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetUpperLimit();
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniSetLimits_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat lower, jfloat upper) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		joint->SetLimits(lower, upper );
}

jbool M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniIsMotorEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->IsMotorEnabled();
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniEnableMotor_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		joint->EnableMotor(flag);
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniSetMotorSpeed_long_float(jcontext ctx, jobject self, jlong addr, jfloat speed) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		joint->SetMotorSpeed(speed);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetMotorSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetMotorSpeed();
}

void M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniSetMaxMotorForce_long_float(jcontext ctx, jobject self, jlong addr, jfloat force) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		joint->SetMaxMotorForce(force);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetMotorForce_long_float_R_float(jcontext ctx, jobject self, jlong addr, jfloat invDt) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetMotorForce(invDt);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetMaxMotorForce_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetMaxMotorForce();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PrismaticJoint_jniGetReferenceAngle_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PrismaticJoint* joint = (b2PrismaticJoint*)addr;
		return joint->GetReferenceAngle();
}

