#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/WheelJoint.h>


#include <Box2D/Box2D.h> 
	 
void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetLocalAxisA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		anchor[0] = joint->GetLocalAxisA().x;
		anchor[1] = joint->GetLocalAxisA().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetJointTranslation_long_R_float(jcontext ctx, jobject self, jlong addr) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetJointTranslation();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetJointSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetJointSpeed();
}

jbool M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniIsMotorEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->IsMotorEnabled();
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniEnableMotor_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		joint->EnableMotor(flag);
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniSetMotorSpeed_long_float(jcontext ctx, jobject self, jlong addr, jfloat speed) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		joint->SetMotorSpeed(speed);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetMotorSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetMotorSpeed();
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniSetMaxMotorTorque_long_float(jcontext ctx, jobject self, jlong addr, jfloat torque) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		joint->SetMaxMotorTorque(torque);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetMaxMotorTorque_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetMaxMotorTorque();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetMotorTorque_long_float_R_float(jcontext ctx, jobject self, jlong addr, jfloat invDt) {

	  	b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetMotorTorque(invDt);
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniSetSpringFrequencyHz_long_float(jcontext ctx, jobject self, jlong addr, jfloat hz) {

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		joint->SetSpringFrequencyHz(hz);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetSpringFrequencyHz_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetSpringFrequencyHz();
}

void M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniSetSpringDampingRatio_long_float(jcontext ctx, jobject self, jlong addr, jfloat ratio) {

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		joint->SetSpringDampingRatio(ratio);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WheelJoint_jniGetSpringDampingRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WheelJoint* joint = (b2WheelJoint*)addr;
		return joint->GetSpringDampingRatio();
}

