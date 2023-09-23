#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/PulleyJoint.h>


#include <Box2D/Box2D.h> 
	 
void M_com_badlogic_gdx_physics_box2d_joints_PulleyJoint_jniGetGroundAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2PulleyJoint* joint = (b2PulleyJoint*)addr;
		anchor[0] = joint->GetGroundAnchorA().x;
		anchor[1] = joint->GetGroundAnchorA().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_PulleyJoint_jniGetGroundAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchor_object) {
	auto anchor = (jfloat *)((Array *)anchor_object)->data;

		b2PulleyJoint* joint = (b2PulleyJoint*)addr;
		anchor[0] = joint->GetGroundAnchorB().x;
		anchor[1] = joint->GetGroundAnchorB().y;
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PulleyJoint_jniGetLength1_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PulleyJoint* joint = (b2PulleyJoint*)addr;
		return joint->GetLengthA();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PulleyJoint_jniGetLength2_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PulleyJoint* joint = (b2PulleyJoint*)addr;
		return joint->GetLengthB();
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_PulleyJoint_jniGetRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2PulleyJoint* joint = (b2PulleyJoint*)addr;
		return joint->GetRatio();
}

