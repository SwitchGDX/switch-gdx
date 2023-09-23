#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Contact.h>


#include <Box2D/Box2D.h>
	 
jint M_com_badlogic_gdx_physics_box2d_Contact_jniGetWorldManifold_long_Array1_float_R_int(jcontext ctx, jobject self, jlong addr, jobject tmp_object) {
	auto tmp = (jfloat *)((Array *)tmp_object)->data;

		b2Contact* contact = (b2Contact*)addr;
		b2WorldManifold manifold;
		contact->GetWorldManifold(&manifold);
		int numPoints = contact->GetManifold()->pointCount;
	
		tmp[0] = manifold.normal.x;
		tmp[1] = manifold.normal.y;
	
		for( int i = 0; i < numPoints; i++ )
		{
			tmp[2 + i*2] = manifold.points[i].x;
			tmp[2 + i*2+1] = manifold.points[i].y;
		}
	
		tmp[6] = manifold.separations[0];
		tmp[7] = manifold.separations[1];

		return numPoints;
}

jbool M_com_badlogic_gdx_physics_box2d_Contact_jniIsTouching_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return contact->IsTouching();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniSetEnabled_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Contact* contact = (b2Contact*)addr;
		contact->SetEnabled(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Contact_jniIsEnabled_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return contact->IsEnabled();
}

jlong M_com_badlogic_gdx_physics_box2d_Contact_jniGetFixtureA_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return (jlong)contact->GetFixtureA();
}

jlong M_com_badlogic_gdx_physics_box2d_Contact_jniGetFixtureB_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return (jlong)contact->GetFixtureB();
}

jint M_com_badlogic_gdx_physics_box2d_Contact_jniGetChildIndexA_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return contact->GetChildIndexA();
}

jint M_com_badlogic_gdx_physics_box2d_Contact_jniGetChildIndexB_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return contact->GetChildIndexB();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniSetFriction_long_float(jcontext ctx, jobject self, jlong addr, jfloat friction) {

		b2Contact* contact = (b2Contact*)addr;
		contact->SetFriction(friction);
}

jfloat M_com_badlogic_gdx_physics_box2d_Contact_jniGetFriction_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Contact* contact = (b2Contact*)addr;
		return contact->GetFriction();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniResetFriction_long(jcontext ctx, jobject self, jlong addr) {

	  	b2Contact* contact = (b2Contact*)addr;
		contact->ResetFriction();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniSetRestitution_long_float(jcontext ctx, jobject self, jlong addr, jfloat restitution) {

	  	b2Contact* contact = (b2Contact*)addr;
		contact->SetRestitution(restitution);
}

jfloat M_com_badlogic_gdx_physics_box2d_Contact_jniGetRestitution_long_R_float(jcontext ctx, jobject self, jlong addr) {

	  	b2Contact* contact = (b2Contact*)addr;
		return contact->GetRestitution();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniResetRestitution_long(jcontext ctx, jobject self, jlong addr) {

	  	b2Contact* contact = (b2Contact*)addr;
		contact->ResetRestitution();
}

jfloat M_com_badlogic_gdx_physics_box2d_Contact_jniGetTangentSpeed_long_R_float(jcontext ctx, jobject self, jlong addr) {

	  	b2Contact* contact = (b2Contact*)addr;
		return contact->GetTangentSpeed();
}

void M_com_badlogic_gdx_physics_box2d_Contact_jniSetTangentSpeed_long_float(jcontext ctx, jobject self, jlong addr, jfloat speed) {

	  	b2Contact* contact = (b2Contact*)addr;
		contact->SetTangentSpeed(speed);
}

