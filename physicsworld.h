#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "Box2D/Box2D.h"
#include <QObject>

class PhysicsWorld : public QObject
{
    Q_OBJECT

public:
    PhysicsWorld(QObject *parent = nullptr);

signals:
    void sendParticleData(b2Vec2 *positions, b2ParticleColor *colors, int size);
    void sendB2Data(b2Body *positions);
    void redrawRender();

public slots:
    void addLiquid(std::string);
    void removeLiquid();
    void addIceCubes(float x);
    void update();

private:
    b2World world;
    const float TIMESTEP = 1.0f / 20.0f;
    const int VELOCITY_ITERATIONS = 6;
    const int POSITION_ITERATIONS = 2;
    const int PARTICLE_ITERATIONS = 3;
    b2ParticleSystem *particleWorld;
    b2ParticleSystem *particleSystem;
    bool destroyIce = false;
    int initBodyCount;

    void createWorldBorders();
    void createWorldCup();
    void createWorldParticleSystem();
};

#endif // PHYSICSWORLD_H
