#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/joints/MotorJoint.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniGetLinearOffset_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject linearOffset_object) {
	auto linearOffset = (jfloat *)((Array *)linearOffset_object)->data;

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		linearOffset[0] = joint->GetLinearOffset().x;
		linearOffset[1] = joint->GetLinearOffset().y;
}

void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniSetLinearOffset_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat linearOffsetX, jfloat linearOffsetY) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		joint->SetLinearOffset(b2Vec2(linearOffsetX, linearOffsetY));
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniGetAngularOffset_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		return joint->GetAngularOffset();
}

void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniSetAngularOffset_long_float(jcontext ctx, jobject self, jlong addr, jfloat angularOffset) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		joint->SetAngularOffset(angularOffset);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniGetMaxForce_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		return joint->GetMaxForce();
}

void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniSetMaxForce_long_float(jcontext ctx, jobject self, jlong addr, jfloat maxForce) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		joint->SetMaxForce(maxForce);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniGetMaxTorque_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		return joint->GetMaxTorque();
}

void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniSetMaxTorque_long_float(jcontext ctx, jobject self, jlong addr, jfloat maxTorque) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		joint->SetMaxTorque(maxTorque);
}

jfloat M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniGetCorrectionFactor_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		return joint->GetCorrectionFactor();
}

void M_com_badlogic_gdx_physics_box2d_joints_MotorJoint_jniSetCorrectionFactor_long_float(jcontext ctx, jobject self, jlong addr, jfloat correctionFactor) {

		b2MotorJoint* joint = (b2MotorJoint*)addr;
		joint->SetCorrectionFactor(correctionFactor);
}

