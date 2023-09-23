#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/EdgeShape.h>


#include <Box2D/Box2D.h>
	 
jlong M_com_badlogic_gdx_physics_box2d_EdgeShape_newEdgeShape_R_long(jcontext ctx, jobject self) {

		return (jlong)(new b2EdgeShape());
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniSet_long_float_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat v1x, jfloat v1y, jfloat v2x, jfloat v2y) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		edge->Set(b2Vec2(v1x, v1y), b2Vec2(v2x, v2y));
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniGetVertex1_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject vertex_object) {
	auto vertex = (jfloat *)((Array *)vertex_object)->data;

		b2EdgeShape* edge = (b2EdgeShape*)addr; 
		vertex[0] = edge->m_vertex1.x;
		vertex[1] = edge->m_vertex1.y;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniGetVertex2_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject vertex_object) {
	auto vertex = (jfloat *)((Array *)vertex_object)->data;

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		vertex[0] = edge->m_vertex2.x;
		vertex[1] = edge->m_vertex2.y;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniGetVertex0_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject vertex_object) {
	auto vertex = (jfloat *)((Array *)vertex_object)->data;

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		vertex[0] = edge->m_vertex0.x;
		vertex[1] = edge->m_vertex0.y;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniSetVertex0_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		edge->m_vertex0.x = x;
		edge->m_vertex0.y = y;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniGetVertex3_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject vertex_object) {
	auto vertex = (jfloat *)((Array *)vertex_object)->data;

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		vertex[0] = edge->m_vertex3.x;
		vertex[1] = edge->m_vertex3.y;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniSetVertex3_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		edge->m_vertex3.x = x;
		edge->m_vertex3.y = y;
}

jbool M_com_badlogic_gdx_physics_box2d_EdgeShape_jniHasVertex0_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		return edge->m_hasVertex0;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniSetHasVertex0_long_boolean(jcontext ctx, jobject self, jlong addr, jbool hasVertex0) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		edge->m_hasVertex0 = hasVertex0;
}

jbool M_com_badlogic_gdx_physics_box2d_EdgeShape_jniHasVertex3_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		return edge->m_hasVertex3;
}

void M_com_badlogic_gdx_physics_box2d_EdgeShape_jniSetHasVertex3_long_boolean(jcontext ctx, jobject self, jlong addr, jbool hasVertex3) {

		b2EdgeShape* edge = (b2EdgeShape*)addr;
		edge->m_hasVertex3 = hasVertex3;
}

