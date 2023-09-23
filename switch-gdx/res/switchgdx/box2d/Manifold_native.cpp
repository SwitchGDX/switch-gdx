#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Manifold.h>


#include <Box2D/Box2D.h>
	 
jint M_com_badlogic_gdx_physics_box2d_Manifold_jniGetType_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Manifold* manifold = (b2Manifold*)addr;
		return manifold->type;
}

jint M_com_badlogic_gdx_physics_box2d_Manifold_jniGetPointCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

	  	b2Manifold* manifold = (b2Manifold*)addr;
		return manifold->pointCount;
}

void M_com_badlogic_gdx_physics_box2d_Manifold_jniGetLocalNormal_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject values_object) {
	auto values = (jfloat *)((Array *)values_object)->data;

		b2Manifold* manifold = (b2Manifold*)addr;
		values[0] = manifold->localNormal.x;
		values[1] = manifold->localNormal.y;
}

void M_com_badlogic_gdx_physics_box2d_Manifold_jniGetLocalPoint_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject values_object) {
	auto values = (jfloat *)((Array *)values_object)->data;

		b2Manifold* manifold = (b2Manifold*)addr;
		values[0] = manifold->localPoint.x;
		values[1] = manifold->localPoint.y;
}

jint M_com_badlogic_gdx_physics_box2d_Manifold_jniGetPoint_long_Array1_float_int_R_int(jcontext ctx, jobject self, jlong addr, jobject values_object, jint idx) {
	auto values = (jfloat *)((Array *)values_object)->data;

		b2Manifold* manifold = (b2Manifold*)addr;
		  
		values[0] = manifold->points[idx].localPoint.x;
		values[1] = manifold->points[idx].localPoint.y;
		values[2] = manifold->points[idx].normalImpulse;
		values[3] = manifold->points[idx].tangentImpulse;  
		  
		return (jint)manifold->points[idx].id.key;
}

