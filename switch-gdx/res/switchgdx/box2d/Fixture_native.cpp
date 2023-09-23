#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Fixture.h>


#include <Box2D/Box2D.h>
	 
jint M_com_badlogic_gdx_physics_box2d_Fixture_jniGetType_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		b2Shape::Type type = fixture->GetType();
		switch( type )
		{
		case b2Shape::e_circle: return 0;
		case b2Shape::e_edge: return 1;
		case b2Shape::e_polygon: return 2;
		case b2Shape::e_chain: return 3;
		default:
			return -1;
		}
}

jlong M_com_badlogic_gdx_physics_box2d_Fixture_jniGetShape_long_R_long(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return (jlong)fixture->GetShape();
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniSetSensor_long_boolean(jcontext ctx, jobject self, jlong addr, jbool sensor) {

		b2Fixture* fixture = (b2Fixture*)addr;
		fixture->SetSensor(sensor);
}

jbool M_com_badlogic_gdx_physics_box2d_Fixture_jniIsSensor_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return fixture->IsSensor();
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniSetFilterData_long_short_short_short(jcontext ctx, jobject self, jlong addr, jshort categoryBits, jshort maskBits, jshort groupIndex) {

		b2Fixture* fixture = (b2Fixture*)addr;
		b2Filter filter;
		filter.categoryBits = categoryBits;
		filter.maskBits = maskBits;
		filter.groupIndex = groupIndex;
		fixture->SetFilterData(filter);
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniGetFilterData_long_Array1_short(jcontext ctx, jobject self, jlong addr, jobject filter_object) {
	auto filter = (jshort *)((Array *)filter_object)->data;

		b2Fixture* fixture = (b2Fixture*)addr;
		unsigned short* filterOut = (unsigned short*)filter;
		b2Filter f = fixture->GetFilterData();
		filterOut[0] = f.maskBits;
		filterOut[1] = f.categoryBits;
		filterOut[2] = f.groupIndex;
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniRefilter_long(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		fixture->Refilter();
}

jbool M_com_badlogic_gdx_physics_box2d_Fixture_jniTestPoint_long_float_float_R_boolean(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return fixture->TestPoint( b2Vec2( x, y ) );
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniSetDensity_long_float(jcontext ctx, jobject self, jlong addr, jfloat density) {

		b2Fixture* fixture = (b2Fixture*)addr;
		fixture->SetDensity(density);
}

jfloat M_com_badlogic_gdx_physics_box2d_Fixture_jniGetDensity_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return fixture->GetDensity();
}

jfloat M_com_badlogic_gdx_physics_box2d_Fixture_jniGetFriction_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return fixture->GetFriction();
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniSetFriction_long_float(jcontext ctx, jobject self, jlong addr, jfloat friction) {

		b2Fixture* fixture = (b2Fixture*)addr;
		fixture->SetFriction(friction);
}

jfloat M_com_badlogic_gdx_physics_box2d_Fixture_jniGetRestitution_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Fixture* fixture = (b2Fixture*)addr;
		return fixture->GetRestitution();
}

void M_com_badlogic_gdx_physics_box2d_Fixture_jniSetRestitution_long_float(jcontext ctx, jobject self, jlong addr, jfloat restitution) {

		b2Fixture* fixture = (b2Fixture*)addr;
		fixture->SetRestitution(restitution);
}

