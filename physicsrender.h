#ifndef PHYSICSRENDER_H
#define PHYSICSRENDER_H

#include <QLabel>
#include "Box2D/Box2D.h"

class PhysicsRender : public QLabel
{
    Q_OBJECT
public:
    PhysicsRender(QWidget *parent = nullptr);
public slots:
    void receiveParticleData(b2Vec2 *positions, b2ParticleColor *colors, int size);
    void receiveB2Data(b2Body *positions);
    void redraw();

protected:
    void paintEvent(QPaintEvent *event);

private:
    b2Vec2 *particlePositions;
    b2ParticleColor *particleColors;
    int particleCount;

    b2Body *bodyPositions;

    void paintParticles(QPainter *painter, b2Vec2 *positions, b2ParticleColor *colors, int size);
    void paintBox2D(QPainter *painter, b2Body *positions);
    double distanceBetweenVec(b2Vec2 vec1, b2Vec2 vec2);
};

#endif // PHYSICSRENDER_H
