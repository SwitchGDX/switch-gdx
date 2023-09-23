#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/Body.h>


#include <Box2D/Box2D.h>
	 
jlong M_com_badlogic_gdx_physics_box2d_Body_jniCreateFixture_long_long_float_float_float_boolean_short_short_short_R_long(jcontext ctx, jobject self, jlong addr, jlong shapeAddr, jfloat friction, jfloat restitution, jfloat density, jbool isSensor, jshort filterCategoryBits, jshort filterMaskBits, jshort filterGroupIndex) {

	b2Body* body = (b2Body*)addr;
	b2Shape* shape = (b2Shape*)shapeAddr;
	b2FixtureDef fixtureDef;

	fixtureDef.shape = shape;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter.maskBits = filterMaskBits;
	fixtureDef.filter.categoryBits = filterCategoryBits;
	fixtureDef.filter.groupIndex = filterGroupIndex;

	return (jlong)body->CreateFixture( &fixtureDef );
}

jlong M_com_badlogic_gdx_physics_box2d_Body_jniCreateFixture_long_long_float_R_long(jcontext ctx, jobject self, jlong addr, jlong shapeAddr, jfloat density) {

		b2Body* body = (b2Body*)addr;
		b2Shape* shape = (b2Shape*)shapeAddr;
		return (jlong)body->CreateFixture( shape, density );
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetTransform_long_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat positionX, jfloat positionY, jfloat angle) {

		b2Body* body = (b2Body*)addr;
		body->SetTransform(b2Vec2(positionX, positionY), angle);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetTransform_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject vals_object) {
	auto vals = (jfloat *)((Array *)vals_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Transform t = body->GetTransform();
		vals[0] = t.p.x;
		vals[1] = t.p.y;
		vals[2] = t.q.c;
		vals[3] = t.q.s;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetPosition_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject position_object) {
	auto position = (jfloat *)((Array *)position_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 p = body->GetPosition();
		position[0] = p.x;
		position[1] = p.y;
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetAngle_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetAngle();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetWorldCenter_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject worldCenter_object) {
	auto worldCenter = (jfloat *)((Array *)worldCenter_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetWorldCenter();
		worldCenter[0] = w.x;
		worldCenter[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLocalCenter_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject localCenter_object) {
	auto localCenter = (jfloat *)((Array *)localCenter_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetLocalCenter();
		localCenter[0] = w.x;
		localCenter[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetLinearVelocity_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat x, jfloat y) {

		b2Body* body = (b2Body*)addr;
		body->SetLinearVelocity(b2Vec2(x, y));
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLinearVelocity_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject linearVelocity_object) {
	auto linearVelocity = (jfloat *)((Array *)linearVelocity_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 l = body->GetLinearVelocity();
		linearVelocity[0] = l.x;
		linearVelocity[1] = l.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetAngularVelocity_long_float(jcontext ctx, jobject self, jlong addr, jfloat omega) {

		b2Body* body = (b2Body*)addr;
		body->SetAngularVelocity(omega);
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetAngularVelocity_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetAngularVelocity();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniApplyForce_long_float_float_float_float_boolean(jcontext ctx, jobject self, jlong addr, jfloat forceX, jfloat forceY, jfloat pointX, jfloat pointY, jbool wake) {

		b2Body* body = (b2Body*)addr;
		body->ApplyForce(b2Vec2(forceX, forceY), b2Vec2(pointX, pointY), wake);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniApplyForceToCenter_long_float_float_boolean(jcontext ctx, jobject self, jlong addr, jfloat forceX, jfloat forceY, jbool wake) {

		b2Body* body = (b2Body*)addr;
		body->ApplyForceToCenter(b2Vec2(forceX, forceY), wake);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniApplyTorque_long_float_boolean(jcontext ctx, jobject self, jlong addr, jfloat torque, jbool wake) {

		b2Body* body = (b2Body*)addr;
		body->ApplyTorque(torque, wake);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniApplyLinearImpulse_long_float_float_float_float_boolean(jcontext ctx, jobject self, jlong addr, jfloat impulseX, jfloat impulseY, jfloat pointX, jfloat pointY, jbool wake) {

		b2Body* body = (b2Body*)addr;
		body->ApplyLinearImpulse( b2Vec2( impulseX, impulseY ), b2Vec2( pointX, pointY ), wake);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniApplyAngularImpulse_long_float_boolean(jcontext ctx, jobject self, jlong addr, jfloat impulse, jbool wake) {

		b2Body* body = (b2Body*)addr;
		body->ApplyAngularImpulse(impulse, wake);
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetMass_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetMass();
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetInertia_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetInertia();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetMassData_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject massData_object) {
	auto massData = (jfloat *)((Array *)massData_object)->data;

		b2Body* body = (b2Body*)addr;
		b2MassData m;
		body->GetMassData(&m);
		massData[0] = m.mass;
		massData[1] = m.center.x;
		massData[2] = m.center.y;
		massData[3] = m.I;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetMassData_long_float_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat mass, jfloat centerX, jfloat centerY, jfloat I) {

		b2Body* body = (b2Body*)addr;
		b2MassData m;
		m.mass = mass;
		m.center.x = centerX;
		m.center.y = centerY;
		m.I = I;
		body->SetMassData(&m);
}

void M_com_badlogic_gdx_physics_box2d_Body_jniResetMassData_long(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		body->ResetMassData();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetWorldPoint_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat localPointX, jfloat localPointY, jobject worldPoint_object) {
	auto worldPoint = (jfloat *)((Array *)worldPoint_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetWorldPoint( b2Vec2( localPointX, localPointY ) );
		worldPoint[0] = w.x;
		worldPoint[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetWorldVector_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat localVectorX, jfloat localVectorY, jobject worldVector_object) {
	auto worldVector = (jfloat *)((Array *)worldVector_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetWorldVector( b2Vec2( localVectorX, localVectorY ) );
		worldVector[0] = w.x;
		worldVector[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLocalPoint_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat worldPointX, jfloat worldPointY, jobject localPoint_object) {
	auto localPoint = (jfloat *)((Array *)localPoint_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetLocalPoint( b2Vec2( worldPointX, worldPointY ) );
		localPoint[0] = w.x;
		localPoint[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLocalVector_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat worldVectorX, jfloat worldVectorY, jobject worldVector_object) {
	auto worldVector = (jfloat *)((Array *)worldVector_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetLocalVector( b2Vec2( worldVectorX, worldVectorY ) );
		worldVector[0] = w.x;
		worldVector[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLinearVelocityFromWorldPoint_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat worldPointX, jfloat worldPointY, jobject linVelWorld_object) {
	auto linVelWorld = (jfloat *)((Array *)linVelWorld_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetLinearVelocityFromWorldPoint( b2Vec2( worldPointX, worldPointY ) );
		linVelWorld[0] = w.x;
		linVelWorld[1] = w.y;
}

void M_com_badlogic_gdx_physics_box2d_Body_jniGetLinearVelocityFromLocalPoint_long_float_float_Array1_float(jcontext ctx, jobject self, jlong addr, jfloat localPointX, jfloat localPointY, jobject linVelLoc_object) {
	auto linVelLoc = (jfloat *)((Array *)linVelLoc_object)->data;

		b2Body* body = (b2Body*)addr;
		b2Vec2 w = body->GetLinearVelocityFromLocalPoint( b2Vec2( localPointX, localPointY ) );
		linVelLoc[0] = w.x;
		linVelLoc[1] = w.y;
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetLinearDamping_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetLinearDamping();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetLinearDamping_long_float(jcontext ctx, jobject self, jlong addr, jfloat linearDamping) {

		b2Body* body = (b2Body*)addr;
		body->SetLinearDamping(linearDamping);
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetAngularDamping_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetAngularDamping();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetAngularDamping_long_float(jcontext ctx, jobject self, jlong addr, jfloat angularDamping) {

		b2Body* body = (b2Body*)addr;
		body->SetAngularDamping(angularDamping);
}


inline b2BodyType getBodyType( int type )
{
	switch( type )
	{
	case 0: return b2_staticBody;
	case 1: return b2_kinematicBody;
	case 2: return b2_dynamicBody;
	default:
		return b2_staticBody;
	}
}	 

void M_com_badlogic_gdx_physics_box2d_Body_jniSetType_long_int(jcontext ctx, jobject self, jlong addr, jint type) {

		b2Body* body = (b2Body*)addr;
		body->SetType(getBodyType(type));
}

jint M_com_badlogic_gdx_physics_box2d_Body_jniGetType_long_R_int(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetType();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetBullet_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Body* body = (b2Body*)addr;
		body->SetBullet(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Body_jniIsBullet_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->IsBullet();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetSleepingAllowed_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Body* body = (b2Body*)addr;
		body->SetSleepingAllowed(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Body_jniIsSleepingAllowed_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->IsSleepingAllowed();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetAwake_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Body* body = (b2Body*)addr;
		body->SetAwake(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Body_jniIsAwake_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->IsAwake();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetActive_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Body* body = (b2Body*)addr;
		body->SetActive(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Body_jniIsActive_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->IsActive();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetFixedRotation_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

		b2Body* body = (b2Body*)addr;
		body->SetFixedRotation(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_Body_jniIsFixedRotation_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->IsFixedRotation();
}

jfloat M_com_badlogic_gdx_physics_box2d_Body_jniGetGravityScale_long_R_float(jcontext ctx, jobject self, jlong addr) {

		b2Body* body = (b2Body*)addr;
		return body->GetGravityScale();
}

void M_com_badlogic_gdx_physics_box2d_Body_jniSetGravityScale_long_float(jcontext ctx, jobject self, jlong addr, jfloat scale) {

		b2Body* body = (b2Body*)addr;
		body->SetGravityScale(scale);
}

