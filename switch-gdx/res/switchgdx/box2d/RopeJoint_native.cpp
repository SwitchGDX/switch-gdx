#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/RopeJoint.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_RopeJoint_jniGetLocalAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2RopeJoint* joint = (b2RopeJoint*)addr;
		anchor[0] = joint->GetLocalAnchorA().x;
		anchor[1] = joint->GetLocalAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_RopeJoint_jniGetLocalAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2RopeJoint* joint = (b2RopeJoint*)addr;
		anchor[0] = joint->GetLocalAnchorB().x;
		anchor[1] = joint->GetLocalAnchorB().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_RopeJoint_jniGetMaxLength_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2RopeJoint* rope = (b2RopeJoint*)addr;
		return rope->GetMaxLength();
}

void M_com_badlogic_gdx_physics_box2d_joints_RopeJoint_jniSetMaxLength_long_float(jcontext ctx, jobject self, jlong addr, jfloat length) {

		b2RopeJoint* rope = (b2RopeJoint*)addr;
		rope->SetMaxLength(length);
}

