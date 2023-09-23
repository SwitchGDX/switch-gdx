#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/WeldJoint.h>


		#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniGetReferenceAngle_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		return joint->GetReferenceAngle();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniGetFrequency_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		return joint->GetFrequency();
}

void M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniSetFrequency_long_float(jcontext ctx, jobject self, jlong addr, jfloat hz) {

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		joint->SetFrequency(hz);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniGetDampingRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		return joint->GetDampingRatio();
}

void M_com_badlogic_gdx_physics_box2d_joints_WeldJoint_jniSetDampingRatio_long_float(jcontext ctx, jobject self, jlong addr, jfloat ratio) {

		b2WeldJoint* joint = (b2WeldJoint*)addr;
		joint->SetDampingRatio(ratio);
}

