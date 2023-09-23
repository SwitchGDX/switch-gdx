#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Shape.h>


#include <Box2D/Box2D.h>
	 
jfloat M_com_badlogic_gdx_physics_box2d_Shape_jniGetRadius_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Shape* shape = (b2Shape*)addr;
		return shape->m_radius;
}

void M_com_badlogic_gdx_physics_box2d_Shape_jniSetRadius_long_float(jcontext ctx, jobject self, jlong addr, jfloat radius) {

		b2Shape* shape = (b2Shape*)addr;
		shape->m_radius = radius;
}

void M_com_badlogic_gdx_physics_box2d_Shape_jniDispose_long(jcontext ctx, jobject self, jlong addr) {

		b2Shape* shape = (b2Shape*)addr;
		delete shape;
}

jint SM_com_badlogic_gdx_physics_box2d_Shape_jniGetType_long_R_int(jcontext ctx, jlong addr) {

		b2Shape* shape = (b2Shape*)addr;
		switch(shape->m_type) {
		case b2Shape::e_circle: return 0;
		case b2Shape::e_edge: return 1;
		case b2Shape::e_polygon: return 2;
		case b2Shape::e_chain: return 3;
		default: return -1;
		}
}

jint M_com_badlogic_gdx_physics_box2d_Shape_jniGetChildCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Shape* shape = (b2Shape*)addr;
		return shape->GetChildCount();
}

