#include "Clearwing.h"
#include "java/nio/Buffer.h"
#include <com/badlogic/gdx/physics/box2d/World.h>


#include <Box2D/Box2D.h>

//static jclass worldClass = 0;
//static jmethodID shouldCollideID = 0;
//static jmethodID beginContactID = 0;
//static jmethodID endContactID = 0;
//static jmethodID preSolveID = 0;
//static jmethodID postSolveID = 0;
//static jmethodID reportFixtureID = 0;
//static jmethodID reportRayFixtureID = 0;

class CustomRayCastCallback: public b2RayCastCallback
{
private:
    jcontext ctx;
    jobject obj;

public:
    CustomRayCastCallback( jcontext ctx, jobject obj )
    {
        this->ctx = ctx;
        this->obj = obj;
    }

    virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        return M_com_badlogic_gdx_physics_box2d_World_reportRayFixture_long_float_float_float_float_float_R_float(ctx, obj, (jlong)fixture, (jfloat)point.x, (jfloat)point.y, (jfloat)normal.x, (jfloat)normal.y, (jfloat)fraction);
//		return env->CallFloatMethod(obj, reportRayFixtureID, (jlong)fixture, (jfloat)point.x, (jfloat)point.y, (jfloat)normal.x, (jfloat)normal.y, (jfloat)fraction );
    }
};

class CustomContactFilter: public b2ContactFilter
{
private:
    jcontext ctx;
    jobject obj;

public:
    CustomContactFilter( jcontext ctx, jobject obj )
    {
        this->ctx = ctx;
        this->obj = obj;
    }

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
    {
//		if( shouldCollideID != 0 )
//			return env->CallBooleanMethod( obj, shouldCollideID, (jlong)fixtureA, (jlong)fixtureB );
//		else
//			return true;
        return M_com_badlogic_gdx_physics_box2d_World_contactFilter_long_long_R_boolean(ctx, obj, (jlong)fixtureA, (jlong)fixtureB);
    }
};

class CustomContactListener: public b2ContactListener
{
private:
    jcontext ctx;
    jobject obj;

public:
    CustomContactListener( jcontext ctx, jobject obj )
    {
        this->ctx = ctx;
        this->obj = obj;
    }

    /// Called when two fixtures begin to touch.
    virtual void BeginContact(b2Contact* contact)
    {
        M_com_badlogic_gdx_physics_box2d_World_beginContact_long(ctx, obj, (jlong)contact);
//			if( beginContactID != 0 )
//				env->CallVoidMethod(obj, beginContactID, (jlong)contact );
    }

    /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact)
    {
//			if( endContactID != 0 )
//				env->CallVoidMethod(obj, endContactID, (jlong)contact);
        M_com_badlogic_gdx_physics_box2d_World_endContact_long(ctx, obj, (jlong)contact);
    }

    /// This is called after a contact is updated.
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
//			if( preSolveID != 0 )
//				env->CallVoidMethod(obj, preSolveID, (jlong)contact, (jlong)oldManifold);
        M_com_badlogic_gdx_physics_box2d_World_preSolve_long_long(ctx, obj, (jlong)contact, (jlong)oldManifold);
    }

    /// This lets you inspect a contact after the solver is finished.
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
//			if( postSolveID != 0 )
//				env->CallVoidMethod(obj, postSolveID, (jlong)contact, (jlong)impulse);
        M_com_badlogic_gdx_physics_box2d_World_postSolve_long_long(ctx, obj, (jlong)contact, (jlong)impulse);
    }
};

class CustomQueryCallback: public b2QueryCallback
{
private:
    jcontext ctx;
    jobject obj;

public:
    CustomQueryCallback( jcontext ctx, jobject obj )
    {
        this->ctx = ctx;
        this->obj = obj;
    }

    virtual bool ReportFixture( b2Fixture* fixture )
    {
//		return env->CallBooleanMethod(obj, reportFixtureID, (jlong)fixture );
        M_com_badlogic_gdx_physics_box2d_World_reportFixture_long_R_boolean(ctx, obj, (jlong)fixture);
    }
};

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

b2ContactFilter defaultFilter;

jlong M_com_badlogic_gdx_physics_box2d_World_newWorld_float_float_boolean_R_long(jcontext ctx, jobject self, jfloat gravityX, jfloat gravityY, jbool doSleep) {

    // we leak one global ref.
//		if(!worldClass) {
//			worldClass = (jclass)env->NewGlobalRef(env->GetObjectClass(object));
//			beginContactID = env->GetMethodID(worldClass, "beginContact", "(J)V" );
//			endContactID = env->GetMethodID( worldClass, "endContact", "(J)V" );
//			preSolveID = env->GetMethodID( worldClass, "preSolve", "(JJ)V" );
//			postSolveID = env->GetMethodID( worldClass, "postSolve", "(JJ)V" );
//			reportFixtureID = env->GetMethodID(worldClass, "reportFixture", "(J)Z" );
//			reportRayFixtureID = env->GetMethodID(worldClass, "reportRayFixture", "(JFFFFF)F" );
//			shouldCollideID = env->GetMethodID( worldClass, "contactFilter", "(JJ)Z");
//		}

    b2World* world = new b2World( b2Vec2( gravityX, gravityY ));
    world->SetAllowSleeping( doSleep );
    return (jlong)world;
}

void M_com_badlogic_gdx_physics_box2d_World_setUseDefaultContactFilter_boolean(jcontext ctx, jobject self, jbool use) {

    // FIXME
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateBody_long_int_float_float_float_float_float_float_float_float_boolean_boolean_boolean_boolean_boolean_float_R_long(jcontext ctx, jobject self, jlong addr, jint type, jfloat positionX, jfloat positionY, jfloat angle, jfloat linearVelocityX, jfloat linearVelocityY, jfloat angularVelocity, jfloat linearDamping, jfloat angularDamping, jbool allowSleep, jbool awake, jbool fixedRotation, jbool bullet, jbool active, jfloat inertiaScale) {

    b2BodyDef bodyDef;
    bodyDef.type = getBodyType(type);
    bodyDef.position.Set( positionX, positionY );
    bodyDef.angle = angle;
    bodyDef.linearVelocity.Set( linearVelocityX, linearVelocityY );
    bodyDef.angularVelocity = angularVelocity;
    bodyDef.linearDamping = linearDamping;
    bodyDef.angularDamping = angularDamping;
    bodyDef.allowSleep = allowSleep;
    bodyDef.awake = awake;
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.bullet = bullet;
    bodyDef.active = active;
    bodyDef.gravityScale = inertiaScale;

    b2World* world = (b2World*)addr;
    b2Body* body = world->CreateBody( &bodyDef );
    return (jlong)body;
}

void M_com_badlogic_gdx_physics_box2d_World_jniDestroyBody_long_long(jcontext ctx, jobject self, jlong addr, jlong bodyAddr) {

    b2World* world = (b2World*)addr;
    b2Body* body = (b2Body*)bodyAddr;
    CustomContactFilter contactFilter(ctx, self);
    CustomContactListener contactListener(ctx,self);
    world->SetContactFilter(&contactFilter);
    world->SetContactListener(&contactListener);
    world->DestroyBody(body);
    world->SetContactFilter(&defaultFilter);
    world->SetContactListener(0);
}

void M_com_badlogic_gdx_physics_box2d_World_jniDestroyFixture_long_long_long(jcontext ctx, jobject self, jlong addr, jlong bodyAddr, jlong fixtureAddr) {

    b2World* world = (b2World*)(addr);
    b2Body* body = (b2Body*)(bodyAddr);
    b2Fixture* fixture = (b2Fixture*)(fixtureAddr);
    CustomContactFilter contactFilter(ctx, self);
    CustomContactListener contactListener(ctx, self);
    world->SetContactFilter(&contactFilter);
    world->SetContactListener(&contactListener);
    body->DestroyFixture(fixture);
    world->SetContactFilter(&defaultFilter);
    world->SetContactListener(0);
}

void M_com_badlogic_gdx_physics_box2d_World_jniDeactivateBody_long_long(jcontext ctx, jobject self, jlong addr, jlong bodyAddr) {

    b2World* world = (b2World*)(addr);
    b2Body* body = (b2Body*)(bodyAddr);
    CustomContactFilter contactFilter(ctx, self);
    CustomContactListener contactListener(ctx, self);
    world->SetContactFilter(&contactFilter);
    world->SetContactListener(&contactListener);
    body->SetActive(false);
    world->SetContactFilter(&defaultFilter);
    world->SetContactListener(0);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateWheelJoint_long_long_long_boolean_float_float_float_float_float_float_boolean_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat localAxisAX, jfloat localAxisAY, jbool enableMotor, jfloat maxMotorTorque, jfloat motorSpeed, jfloat frequencyHz, jfloat dampingRatio) {

    b2World* world = (b2World*)addr;
    b2WheelJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.localAxisA = b2Vec2(localAxisAX, localAxisAY);
    def.enableMotor = enableMotor;
    def.maxMotorTorque = maxMotorTorque;
    def.motorSpeed = motorSpeed;
    def.frequencyHz = frequencyHz;
    def.dampingRatio = dampingRatio;

    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateRopeJoint_long_long_long_boolean_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat maxLength) {

    b2World* world = (b2World*)addr;
    b2RopeJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.maxLength = maxLength;

    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateDistanceJoint_long_long_long_boolean_float_float_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat length, jfloat frequencyHz, jfloat dampingRatio) {

    b2World* world = (b2World*)addr;
    b2DistanceJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.length = length;
    def.frequencyHz = frequencyHz;
    def.dampingRatio = dampingRatio;

    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateFrictionJoint_long_long_long_boolean_float_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat maxForce, jfloat maxTorque) {

    b2World* world = (b2World*)addr;
    b2FrictionJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.maxForce = maxForce;
    def.maxTorque = maxTorque;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateGearJoint_long_long_long_boolean_long_long_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jlong joint1, jlong joint2, jfloat ratio) {

    b2World* world = (b2World*)addr;
    b2GearJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.joint1 = (b2Joint*)joint1;
    def.joint2 = (b2Joint*)joint2;
    def.ratio = ratio;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateMotorJoint_long_long_long_boolean_float_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat linearOffsetX, jfloat linearOffsetY, jfloat angularOffset, jfloat maxForce, jfloat maxTorque, jfloat correctionFactor) {

    b2World* world = (b2World*)addr;
    b2MotorJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.linearOffset = b2Vec2( linearOffsetX, linearOffsetY );
    def.angularOffset = angularOffset;
    def.maxForce = maxForce;
    def.maxTorque = maxTorque;
    def.correctionFactor = correctionFactor;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateMouseJoint_long_long_long_boolean_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat targetX, jfloat targetY, jfloat maxForce, jfloat frequencyHz, jfloat dampingRatio) {

    b2World* world = (b2World*)addr;
    b2MouseJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.target = b2Vec2( targetX, targetY );
    def.maxForce = maxForce;
    def.frequencyHz = frequencyHz;
    def.dampingRatio = dampingRatio;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreatePrismaticJoint_long_long_long_boolean_float_float_float_float_float_float_float_boolean_float_float_boolean_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat localAxisAX, jfloat localAxisAY, jfloat referenceAngle, jbool enableLimit, jfloat lowerTranslation, jfloat upperTranslation, jbool enableMotor, jfloat maxMotorForce, jfloat motorSpeed) {

    b2World* world = (b2World*)addr;
    b2PrismaticJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.localAxisA = b2Vec2( localAxisAX, localAxisAY );
    def.referenceAngle = referenceAngle;
    def.enableLimit = enableLimit;
    def.lowerTranslation = lowerTranslation;
    def.upperTranslation = upperTranslation;
    def.enableMotor = enableMotor;
    def.maxMotorForce = maxMotorForce;
    def.motorSpeed = motorSpeed;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreatePulleyJoint_long_long_long_boolean_float_float_float_float_float_float_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat groundAnchorAX, jfloat groundAnchorAY, jfloat groundAnchorBX, jfloat groundAnchorBY, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat lengthA, jfloat lengthB, jfloat ratio) {

    b2World* world = (b2World*)addr;
    b2PulleyJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.groundAnchorA = b2Vec2( groundAnchorAX, groundAnchorAY );
    def.groundAnchorB = b2Vec2( groundAnchorBX, groundAnchorBY );
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.lengthA = lengthA;
    def.lengthB = lengthB;
    def.ratio = ratio;

    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateRevoluteJoint_long_long_long_boolean_float_float_float_float_float_boolean_float_float_boolean_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat referenceAngle, jbool enableLimit, jfloat lowerAngle, jfloat upperAngle, jbool enableMotor, jfloat motorSpeed, jfloat maxMotorTorque) {

    b2World* world = (b2World*)addr;
    b2RevoluteJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.referenceAngle = referenceAngle;
    def.enableLimit = enableLimit;
    def.lowerAngle = lowerAngle;
    def.upperAngle = upperAngle;
    def.enableMotor = enableMotor;
    def.motorSpeed = motorSpeed;
    def.maxMotorTorque = maxMotorTorque;
    return (jlong)world->CreateJoint(&def);
}

jlong M_com_badlogic_gdx_physics_box2d_World_jniCreateWeldJoint_long_long_long_boolean_float_float_float_float_float_float_float_R_long(jcontext ctx, jobject self, jlong addr, jlong bodyA, jlong bodyB, jbool collideConnected, jfloat localAnchorAX, jfloat localAnchorAY, jfloat localAnchorBX, jfloat localAnchorBY, jfloat referenceAngle, jfloat frequencyHz, jfloat dampingRatio) {

    b2World* world = (b2World*)addr;
    b2WeldJointDef def;
    def.bodyA = (b2Body*)bodyA;
    def.bodyB = (b2Body*)bodyB;
    def.collideConnected = collideConnected;
    def.localAnchorA = b2Vec2(localAnchorAX, localAnchorAY);
    def.localAnchorB = b2Vec2(localAnchorBX, localAnchorBY);
    def.referenceAngle = referenceAngle;
    def.frequencyHz = frequencyHz;
    def.dampingRatio = dampingRatio;

    return (jlong)world->CreateJoint(&def);
}

void M_com_badlogic_gdx_physics_box2d_World_jniDestroyJoint_long_long(jcontext ctx, jobject self, jlong addr, jlong jointAddr) {

    b2World* world = (b2World*)addr;
    b2Joint* joint = (b2Joint*)jointAddr;
    CustomContactFilter contactFilter(ctx, self);
    CustomContactListener contactListener(ctx, self);
    world->SetContactFilter(&contactFilter);
    world->SetContactListener(&contactListener);
    world->DestroyJoint( joint );
    world->SetContactFilter(&defaultFilter);
    world->SetContactListener(0);
}

void M_com_badlogic_gdx_physics_box2d_World_jniStep_long_float_int_int(jcontext ctx, jobject self, jlong addr, jfloat timeStep, jint velocityIterations, jint positionIterations) {

    b2World* world = (b2World*)addr;
    CustomContactFilter contactFilter(ctx, self);
    CustomContactListener contactListener(ctx, self);
    world->SetContactFilter(&contactFilter);
    world->SetContactListener(&contactListener);
    world->Step( timeStep, velocityIterations, positionIterations );
    world->SetContactFilter(&defaultFilter);
    world->SetContactListener(0);
}

void M_com_badlogic_gdx_physics_box2d_World_jniClearForces_long(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    world->ClearForces();
}

void M_com_badlogic_gdx_physics_box2d_World_jniSetWarmStarting_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

    b2World* world = (b2World*)addr;
    world->SetWarmStarting(flag);
}

void M_com_badlogic_gdx_physics_box2d_World_jniSetContiousPhysics_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

    b2World* world = (b2World*)addr;
    world->SetContinuousPhysics(flag);
}

jint M_com_badlogic_gdx_physics_box2d_World_jniGetProxyCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->GetProxyCount();
}

jint M_com_badlogic_gdx_physics_box2d_World_jniGetBodyCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->GetBodyCount();
}

jint M_com_badlogic_gdx_physics_box2d_World_jniGetJointcount_long_R_int(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->GetJointCount();
}

jint M_com_badlogic_gdx_physics_box2d_World_jniGetContactCount_long_R_int(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->GetContactCount();
}

void M_com_badlogic_gdx_physics_box2d_World_jniSetGravity_long_float_float(jcontext ctx, jobject self, jlong addr, jfloat gravityX, jfloat gravityY) {

    b2World* world = (b2World*)addr;
    world->SetGravity( b2Vec2( gravityX, gravityY ) );
}

void M_com_badlogic_gdx_physics_box2d_World_jniGetGravity_long_Array1_float(jcontext ctx, jobject self, jlong addr, jobject gravity_object) {
    auto gravity = (jfloat *)((jarray) gravity_object)->data;

    b2World* world = (b2World*)addr;
    b2Vec2 g = world->GetGravity();
    gravity[0] = g.x;
    gravity[1] = g.y;
}

jbool M_com_badlogic_gdx_physics_box2d_World_jniIsLocked_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->IsLocked();
}

void M_com_badlogic_gdx_physics_box2d_World_jniSetAutoClearForces_long_boolean(jcontext ctx, jobject self, jlong addr, jbool flag) {

    b2World* world = (b2World*)addr;
    world->SetAutoClearForces(flag);
}

jbool M_com_badlogic_gdx_physics_box2d_World_jniGetAutoClearForces_long_R_boolean(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)addr;
    return world->GetAutoClearForces();
}

void M_com_badlogic_gdx_physics_box2d_World_jniQueryAABB_long_float_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat lowX, jfloat lowY, jfloat upX, jfloat upY) {

    b2World* world = (b2World*)addr;
    b2AABB aabb;
    aabb.lowerBound = b2Vec2( lowX, lowY );
    aabb.upperBound = b2Vec2( upX, upY );

    CustomQueryCallback callback( ctx, self );
    world->QueryAABB( &callback, aabb );
}

void M_com_badlogic_gdx_physics_box2d_World_jniGetContactList_long_Array1_long(jcontext ctx, jobject self, jlong addr, jobject contacts_object) {
    auto contacts = (jlong *)((Array *)contacts_object)->data;

    b2World* world = (b2World*)addr;

    b2Contact* contact = world->GetContactList();
    int i = 0;
    while( contact != 0 )
    {
        contacts[i++] = (long long)contact;
        contact = contact->GetNext();
    }
}

void M_com_badlogic_gdx_physics_box2d_World_jniDispose_long(jcontext ctx, jobject self, jlong addr) {

    b2World* world = (b2World*)(addr);
    delete world;
}

void SM_com_badlogic_gdx_physics_box2d_World_setVelocityThreshold_float(jcontext ctx, jfloat threshold) {

    b2_velocityThreshold = threshold;
}

jfloat SM_com_badlogic_gdx_physics_box2d_World_getVelocityThreshold_R_float(jcontext ctx) {

    return b2_velocityThreshold;
}

void M_com_badlogic_gdx_physics_box2d_World_jniRayCast_long_float_float_float_float(jcontext ctx, jobject self, jlong addr, jfloat aX, jfloat aY, jfloat bX, jfloat bY) {

    b2World *world = (b2World*)addr;
    CustomRayCastCallback callback( ctx, self );
    world->RayCast( &callback, b2Vec2(aX,aY), b2Vec2(bX,bY) );
}

