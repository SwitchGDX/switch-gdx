#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/PolygonShape.h>


     #include <Box2D/Box2D.h>
	 
jlong M_com_badlogic_gdx_physics_box2d_PolygonShape_newPolygonShape_R_long(jcontext ctx, jobject self) {

		b2PolygonShape* poly = new b2PolygonShape();
		return (jlong)poly;
}

void M_com_badlogic_gdx_physics_box2d_PolygonShape_jniSet_long_Array1_float_int_int(jcontext ctx, jobject self, jlong addr, jobject verts_object, jint offset, jint len) {
	auto verts = (jfloat *)((Array *)verts_object)->data;

		b2PolygonShape* poly = (b2PolygonShape*)addr;
		int numVertices = len / 2;
		b2Vec2* verticesOut = new b2Vec2[numVertices];
		for(int i = 0; i < numVertices; i++) { 
			verticesOut[i] = b2Vec2(verts[(i<<1) + offset], verts[(i<<1) + offset + 1]);
		}
		poly->Set(verticesOut, numVertices);
		delete[] verticesOut;
	 }

void M_com_badlogic_gdx_physics_box2d_PolygonShape_jniSetAsBox_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat hx, jfloat hy) {

		b2PolygonShape* poly = (b2PolygonShape*)addr;
		poly->SetAsBox(hx, hy);
}

void M_com_badlogic_gdx_physics_box2d_PolygonShape_jniSetAsBox_long_float_float_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat hx, jfloat hy, jfloat centerX, jfloat centerY, jfloat angle) {

		b2PolygonShape* poly = (b2PolygonShape*)addr;
		poly->SetAsBox( hx, hy, b2Vec2( centerX, centerY ), angle );
}

jint M_com_badlogic_gdx_physics_box2d_PolygonShape_jniGetVertexCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2PolygonShape* poly = (b2PolygonShape*)addr;
		return poly->GetVertexCount();
}

void M_com_badlogic_gdx_physics_box2d_PolygonShape_jniGetVertex_long_int_Array1_float(jcontext ctx, jobject self, jlong addr, jint index, jobject verts_object) {
	auto verts = (jfloat *)((Array *)verts_object)->data;

		b2PolygonShape* poly = (b2PolygonShape*)addr;
		const b2Vec2 v = poly->GetVertex( index );
		verts[0] = v.x;
		verts[1] = v.y;
}

