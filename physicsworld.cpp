#include "physicsworld.h"
#include <iostream>
#include <QColor>

PhysicsWorld::PhysicsWorld(QObject *parent)
    : QObject(parent)
    , world(b2Vec2(0.0f, -9.8f))
{
    createWorldBorders();
    createWorldCup();
    createWorldParticleSystem();

    PhysicsWorld::initBodyCount = world.GetBodyCount();
}

/**
 * @brief PhysicsWorld::addIceCubes
 * @param x
 */
void PhysicsWorld::addIceCubes(float x){
    b2BodyDef testBodyDef;
    testBodyDef.position.Set(x,30.0f);
    testBodyDef.angle = 1.0f;
    testBodyDef.type = b2_dynamicBody;
    testBodyDef.userData = new QColor(155,226,242,255);
    b2Body *testBody = world.CreateBody(&testBodyDef);
    b2PolygonShape testShape;
    testShape.SetAsBox(20.0f,20.0f);
    testBody->CreateFixture(&testShape, 0.5f);
}

void PhysicsWorld::removeLiquid(){
    int size = particleSystem->GetParticleCount();
    for (int i = 0; i < size; i++){
        particleSystem->DestroyParticle(i);
    }

    PhysicsWorld::destroyIce=true;
}

void PhysicsWorld::addLiquid(std::string color) {
    b2ParticleGroupDef particleDef;
    b2PolygonShape shape;
    if (color == "coffee") {
        shape.SetAsBox(20.0f, 50.0f);
        particleDef.color.Set(100,70,50,255);
    } else if (color == "milk") {
        shape.SetAsBox(30.0f, 70.0f);
        particleDef.color.Set(232,218,188,255);
    } else if (color == "vanilla") {
        shape.SetAsBox(16.0f, 24.0f);
        particleDef.color.Set(124,76,32,255);
    } else if (color == "water") {
        shape.SetAsBox(30.0f, 70.0f);
        particleDef.color.Set(111,78,55,20);
    }

    particleDef.shape = &shape;
    particleDef.flags = b2_colorMixingParticle;
    particleDef.position.Set(0.0f, 0.0f);
    particleSystem->CreateParticleGroup(particleDef);
}

/**
 * @brief PhysicsWorld::update
 * Slot that updates world by stepping forward one time
 */
void PhysicsWorld::update() {
    if (PhysicsWorld::destroyIce){
        while (world.GetBodyCount()!=PhysicsWorld::initBodyCount){
            world.DestroyBody(world.GetBodyList());
        }
        PhysicsWorld::destroyIce=false;
    }
    world.Step(TIMESTEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS, PARTICLE_ITERATIONS);

    b2Vec2 *particlePositions = particleSystem->GetPositionBuffer();
    b2ParticleColor *particleColors = particleSystem->GetColorBuffer();
    int particleSize = particleSystem->GetParticleCount();

    b2Body *bodyPositions = world.GetBodyList();

    emit sendParticleData(particlePositions, particleColors, particleSize);
    emit sendB2Data(bodyPositions);
    emit redrawRender();
}

/**
 * @brief PhysicsWorld::createWorldBorders
 *
 * Helper method that creates the borders along the bottom and sides of the label so that particles don't fall out.
 */
void PhysicsWorld::createWorldBorders() {
    b2BodyDef groundbodyDef;
    groundbodyDef.userData = new QColor(0,0,0,0);
    groundbodyDef.position.Set(0.0f, -245.0f);

    b2Body* groundBody = world.CreateBody(&groundbodyDef);

    //Defining the walls
    b2BodyDef sideBodyDef;
    sideBodyDef.userData = new QColor(0,0,0,0);
    sideBodyDef.position.Set(271.0f, 0.0f);
    b2Body* sideBodyR = world.CreateBody(&sideBodyDef);

    sideBodyDef.position.Set(-271.0f, 0.0f);
    b2Body* sideBodyL = world.CreateBody(&sideBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(261.0f, 10.0f);

    b2PolygonShape sideBox;
    sideBox.SetAsBox(10.0f, 236.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    sideBodyR->CreateFixture(&sideBox, 0.0f);
    sideBodyL->CreateFixture(&sideBox, 0.0f);
}

/**
 * @brief PhysicsWorld::createWorldCup
 *
 * Helper method that creates the borders of the cup
 *
 * Alpha is set to 0 so that these borders will not render
 */
void PhysicsWorld::createWorldCup() {
    //Create cup body
    b2BodyDef cupBody;
    cupBody.userData = new QColor(0,0,0,0);
    cupBody.position.Set(6.0f, -176.0f);
    b2Body* cupBase = world.CreateBody(&cupBody);
    b2PolygonShape cupBox;
    cupBox.SetAsBox(36.0f, 1.0f);
    cupBase->CreateFixture(&cupBox, 0.0f);

    //Sides of cup:

    //Lower sides
    b2BodyDef cupLowSideLDef;
    cupLowSideLDef.userData = new QColor(0,0,0,0);
    cupLowSideLDef.position.Set(-46.0f, -168.0f);
    cupLowSideLDef.angle = 1.05f;
    b2Body* cupLowSideL = world.CreateBody(&cupLowSideLDef);
    b2PolygonShape lowLShape;
    lowLShape.SetAsBox(1.0f, 14.0f);
    cupLowSideL->CreateFixture(&lowLShape, 0.0f);

    b2BodyDef cupLowSideRDef;
    cupLowSideRDef.userData = new QColor(0,0,0,0);
    cupLowSideRDef.position.Set(60.0f, -168.0f);
    cupLowSideRDef.angle = -1.05f;
    b2Body* cupLowSideR = world.CreateBody(&cupLowSideRDef);
    b2PolygonShape lowRShape;
    lowRShape.SetAsBox(1.0f, 14.0f);
    cupLowSideR->CreateFixture(&lowRShape, 0.0f);

    //Middle sides
    b2BodyDef cupMidSideLDef;
    cupMidSideLDef.userData = new QColor(0,0,0,0);
    cupMidSideLDef.position.Set(-62.0f, -142.0f);
    cupMidSideLDef.angle = 0.4f;
    b2Body* cupMidSideL = world.CreateBody(&cupMidSideLDef);
    b2PolygonShape midLShape;
    midLShape.SetAsBox(1.0f, 16.0f);
    cupMidSideL->CreateFixture(&midLShape, 0.0f);

    b2BodyDef cupMidSideRDef;
    cupMidSideRDef.userData = new QColor(0,0,0,0);
    cupMidSideRDef.position.Set(74.0f, -142.0f);
    cupMidSideRDef.angle = -0.4;
    b2Body* cupMidSideR = world.CreateBody(&cupMidSideRDef);
    b2PolygonShape midRShape;
    midRShape.SetAsBox(1.0f, 20.0f);
    cupMidSideR->CreateFixture(&midRShape, 0.0f);

    //Upper sides
    b2BodyDef cupHighSideLDef;
    cupHighSideLDef.userData = new QColor(0,0,0,0);
    cupHighSideLDef.position.Set(-72.0f, -88.0f);
    cupHighSideLDef.angle = 0.1f;
    b2Body* cupHighSideL = world.CreateBody(&cupHighSideLDef);
    b2PolygonShape highLShape;
    highLShape.SetAsBox(1.0f, 36.0f);
    cupHighSideL->CreateFixture(&highLShape, 0.0f);

    b2BodyDef cupHighSideRDef;
    cupHighSideRDef.userData = new QColor(0,0,0,0);
    cupHighSideRDef.position.Set(84.0f, -88.0f);
    cupHighSideRDef.angle = -0.1f;
    b2Body* cupHighSideR = world.CreateBody(&cupHighSideRDef);
    b2PolygonShape highRShape;
    highRShape.SetAsBox(1.0f, 36.0f);
    cupHighSideR->CreateFixture(&highRShape, 0.0f);
}

/**
 * @brief PhysicsWorld::createWorldParticleSystem
 *
 * Helper method that creates the particle system for the world.
 */
void PhysicsWorld::createWorldParticleSystem() {
    //Creating basic particle system
    b2ParticleSystemDef particleSysDef;
    particleSysDef.radius = 3.0f;
    particleSystem = world.CreateParticleSystem(&particleSysDef);
}
