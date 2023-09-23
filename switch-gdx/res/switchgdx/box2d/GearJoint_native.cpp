#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/GearJoint.h>


#include <Box2D/Box2D.h> 
	 
jlong M_com_badlogic_gdx_physics_box2d_joints_GearJoint_jniGetJoint1_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2GearJoint* joint =  (b2GearJoint*)addr;
		b2Joint* joint1 = joint->GetJoint1();
		return (jlong)joint1;
}

jlong M_com_badlogic_gdx_physics_box2d_joints_GearJoint_jniGetJoint2_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2GearJoint* joint =  (b2GearJoint*)addr;
		b2Joint* joint2 = joint->GetJoint2();
		return (jlong)joint2;
}

void M_com_badlogic_gdx_physics_box2d_joints_GearJoint_jniSetRatio_long_float(jcontext ctx, jobject self, jlong addr, jfloat ratio) {

		b2GearJoint* joint =  (b2GearJoint*)addr;
		joint->SetRatio( ratio );
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_GearJoint_jniGetRatio_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2GearJoint* joint =  (b2GearJoint*)addr;
		return joint->GetRatio();
}

