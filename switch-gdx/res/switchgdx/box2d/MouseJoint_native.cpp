#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/MouseJoint.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniSetTarget_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		joint->SetTarget( b2Vec2(x, y ) );
}

void M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniGetTarget_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject target_object) {
	auto target = (jfloat *)((Array *)target_object)->data;

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		target[0] = joint->GetTarget().x;
		target[1] = joint->GetTarget().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniSetMaxForce_long_float(jcontext ctx, jobject self, jlong addr, jfloat force) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		joint->SetMaxForce( force );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniGetMaxForce_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		return joint->GetMaxForce();
}

void M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniSetFrequency_long_float(jcontext ctx, jobject self, jlong addr, jfloat hz) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		joint->SetFrequency(hz);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniGetFrequency_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		return joint->GetFrequency();
}

void M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniSetDampingRatio_long_float(jcontext ctx, jobject self, jlong addr, jfloat ratio) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		joint->SetDampingRatio( ratio );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MouseJoint_jniGetDampingRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MouseJoint* joint = (b2MouseJoint*)addr;
		return joint->GetDampingRatio();
}

