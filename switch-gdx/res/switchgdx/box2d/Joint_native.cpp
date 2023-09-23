#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Joint.h>


#include <Box2D/Box2D.h> 
	 
jint M_com_badlogic_gdx_physics_box2d_Joint_jniGetType_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Joint* joint = (b2Joint*)addr;
		return joint->GetType();
}

jlong M_com_badlogic_gdx_physics_box2d_Joint_jniGetBodyA_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2Joint* joint = (b2Joint*)addr;
		return (jlong)joint->GetBodyA();
}

jlong M_com_badlogic_gdx_physics_box2d_Joint_jniGetBodyB_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2Joint* joint = (b2Joint*)addr;
		return (jlong)joint->GetBodyB();
}

void M_com_badlogic_gdx_physics_box2d_Joint_jniGetAnchorA_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchorA_object) {
	auto anchorA = (jfloat *)((Array *)anchorA_object)->data;

		b2Joint* joint = (b2Joint*)addr;
		b2Vec2 a = joint->GetAnchorA();
		anchorA[0] = a.x;
		anchorA[1] = a.y;
}

void M_com_badlogic_gdx_physics_box2d_Joint_jniGetAnchorB_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject anchorB_object) {
	auto anchorB = (jfloat *)((Array *)anchorB_object)->data;

		b2Joint* joint = (b2Joint*)addr;
		b2Vec2 a = joint->GetAnchorB();
		anchorB[0] = a.x;
		anchorB[1] = a.y;
}

jbool M_com_badlogic_gdx_physics_box2d_Joint_jniGetCollideConnected_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Joint* joint = (b2Joint*) addr;
		return joint->GetCollideConnected();
}

void M_com_badlogic_gdx_physics_box2d_Joint_jniGetReactionForce_long_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat inv_dt, jobject reactionForce_object) {
	auto reactionForce = (jfloat *)((Array *)reactionForce_object)->data;

		b2Joint* joint = (b2Joint*)addr;
		b2Vec2 f = joint->GetReactionForce(inv_dt);
		reactionForce[0] = f.x;
		reactionForce[1] = f.y;
}

jfloat M_com_badlogic_gdx_physics_box2d_Joint_jniGetReactionTorque_long_float_R_float(jcontext ctx, jobject self, jlong addr, jfloat inv_dt) {

		b2Joint* joint = (b2Joint*)addr;
		return joint->GetReactionTorque(inv_dt);
}

jbool M_com_badlogic_gdx_physics_box2d_Joint_jniIsActive_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Joint* joint = (b2Joint*)addr;
		return joint->IsActive();
}

