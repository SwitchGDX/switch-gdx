#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/DistanceJoint.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniSetLength_long_float(jcontext ctx, jobject self, jlong addr, jfloat length) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		joint->SetLength( length );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniGetLength_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		return joint->GetLength();
}

void M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniSetFrequency_long_float(jcontext ctx, jobject self, jlong addr, jfloat hz) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		joint->SetFrequency( hz );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniGetFrequency_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		return joint->GetFrequency();
}

void M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniSetDampingRatio_long_float(jcontext ctx, jobject self, jlong addr, jfloat ratio) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		joint->SetDampingRatio( ratio );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_DistanceJoint_jniGetDampingRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2DistanceJoint* joint = (b2DistanceJoint*)addr;
		return joint->GetDampingRatio();
}

