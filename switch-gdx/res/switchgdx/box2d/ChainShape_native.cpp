#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/ChainShape.h>


#include <Box2D/Box2D.h>
	 
jlong M_com_badlogic_gdx_physics_box2d_ChainShape_newChainShape_R_long(jcontext ctx, jobject self) {

		return (jlong)(new b2ChainShape());
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniCreateLoop_long_Array1_float_int_int(jcontext ctx, jobject self, jlong addr, jobject verts_object, jint offset, jint numVertices) {
	auto verts = (jfloat *)((Array *)verts_object)->data;

		b2ChainShape* chain = (b2ChainShape*)addr;
		b2Vec2* verticesOut = new b2Vec2[numVertices];
		for( int i = 0; i < numVertices; i++ )
			verticesOut[i] = b2Vec2(verts[offset+(i<<1)], verts[offset+(i<<1)+1]);
		chain->CreateLoop( verticesOut, numVertices );
		delete[] verticesOut;
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniCreateChain_long_Array1_float_int_int(jcontext ctx, jobject self, jlong addr, jobject verts_object, jint offset, jint numVertices) {
	auto verts = (jfloat *)((Array *)verts_object)->data;

		b2ChainShape* chain = (b2ChainShape*)addr;
		b2Vec2* verticesOut = new b2Vec2[numVertices];
		for( int i = 0; i < numVertices; i++ )
			verticesOut[i] = b2Vec2(verts[offset+(i<<1)], verts[offset+(i<<1)+1]);
		chain->CreateChain( verticesOut, numVertices );
		delete[] verticesOut;
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniSetPrevVertex_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2ChainShape* chain = (b2ChainShape*)addr;
		chain->SetPrevVertex(b2Vec2(x, y));
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniSetNextVertex_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2ChainShape* chain = (b2ChainShape*)addr;
		chain->SetNextVertex(b2Vec2(x, y));
}

jint M_com_badlogic_gdx_physics_box2d_ChainShape_jniGetVertexCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2ChainShape* chain = (b2ChainShape*)addr;
		return chain->GetVertexCount();
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniGetVertex_long_int_Array1_float(jcontext ctx, jobject self, jlong addr, jint index, jobject verts_object) {
	auto verts = (jfloat *)((Array *)verts_object)->data;

		b2ChainShape* chain = (b2ChainShape*)addr;
		const b2Vec2 v = chain->GetVertex( index );
		verts[0] = v.x;
		verts[1] = v.y;
}

void M_com_badlogic_gdx_physics_box2d_ChainShape_jniClear_long(jcontext ctx, jobject self, jlong addr) {

    b2ChainShape* chain = (b2ChainShape*)addr;
    chain->Clear();
}
