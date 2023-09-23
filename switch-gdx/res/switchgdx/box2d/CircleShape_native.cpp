#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/CircleShape.h>


#include <Box2D/Box2D.h>
	 
jlong M_com_badlogic_gdx_physics_box2d_CircleShape_newCircleShape_R_long(jcontext ctx, jobject self) {

		return (jlong)(new b2CircleShape( ));
}

void M_com_badlogic_gdx_physics_box2d_CircleShape_jniGetPosition_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject position_object) {
	auto position = (jfloat *)((Array *)position_object)->data;

		b2CircleShape* circle = (b2CircleShape*)addr;
		position[0] = circle->m_p.x;
		position[1] = circle->m_p.y;
}

void M_com_badlogic_gdx_physics_box2d_CircleShape_jniSetPosition_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat positionX, jfloat positionY) {

		b2CircleShape* circle = (b2CircleShape*)addr;
		circle->m_p.x = positionX;
		circle->m_p.y = positionY;
}

