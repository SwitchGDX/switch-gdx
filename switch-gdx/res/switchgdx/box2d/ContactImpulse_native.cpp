#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/ContactImpulse.h>


#include <Box2D/Box2D.h>
	 
void M_com_badlogic_gdx_physics_box2d_ContactImpulse_jniGetNormalImpulses_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject values_object) {
	auto values = (jfloat *)((Array *)values_object)->data;

		b2ContactImpulse* contactImpulse = (b2ContactImpulse*)addr;	
		values[0] = contactImpulse->normalImpulses[0];
		values[1] = contactImpulse->normalImpulses[1];
}

void M_com_badlogic_gdx_physics_box2d_ContactImpulse_jniGetTangentImpulses_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject values_object) {
	auto values = (jfloat *)((Array *)values_object)->data;

	  	b2ContactImpulse* contactImpulse = (b2ContactImpulse*)addr;	
		values[0] = contactImpulse->tangentImpulses[0];
		values[1] = contactImpulse->tangentImpulses[1];
}

jint M_com_badlogic_gdx_physics_box2d_ContactImpulse_jniGetCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2ContactImpulse* contactImpulse = (b2ContactImpulse*)addr;
		return contactImpulse->count;
}

