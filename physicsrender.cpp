#include "physicsrender.h"
#include <QPainter>
#include <cmath>

PhysicsRender::PhysicsRender(QWidget *parent)
    :QLabel(parent)
{
}

/**
 * @brief PhysicsRender::receiveParticleData
 * Slot that saves the pointers to the instance variables of this class
 *
 * @param positions - pointer to the array that holds the positions of the particles
 * @param colors - pointer to the array that holds the colors of the particles
 * @param size - the total number of particles
 */
void PhysicsRender::receiveParticleData(b2Vec2 *positions, b2ParticleColor *colors, int size) {
    particlePositions = positions;
    particleColors = colors;
    particleCount = size;
}

/**
 * @brief PhysicsRender::receiveB2Data
 * Slot that saves the pointers to the instance variables of this class
 *
 * @param positions - pointer to the array that holds the positions of the b2Bodies
 */
void PhysicsRender::receiveB2Data(b2Body* positions) {
    bodyPositions = positions;
}

/**
 * @brief PhysicsRender::redraw
 * Slot that calls a update to the label.
 */
void PhysicsRender::redraw() {
    update();
}

/**
 * @brief PhysicsRender::paintEvent
 * Draws the particles on the label
 * @param event
 */
void PhysicsRender::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintParticles(&painter, particlePositions, particleColors, particleCount);
    paintBox2D(&painter, bodyPositions);
    painter.end();
}

/**
 * @brief PhysicsRender::paintParticles
 * Helper method that draws the particles according to their specific position and color
 *
 * @param painter - copy of the painter
 * @param positions - positions of all the particles
 * @param colors - colors of all the particles
 * @param size - total number of particles
 */
void PhysicsRender::paintParticles(QPainter *painter, b2Vec2 *positions, b2ParticleColor *colors, int size) {
    double halfCanvasX = (width() / 2.0);
    double halfCanvasY = (height() / 2.0);

    QPen pen;
    pen.setWidthF(0);
    pen.setColor(QColor(0,0,0,0));
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    painter->setPen(pen);

    for(int i = 0; i < size; i++) {
        QColor currentColor(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        brush.setColor(currentColor);
        painter->setBrush(brush);
        QPointF currentParticle(halfCanvasX + positions[i].x, halfCanvasY - positions[i].y);
        painter->drawEllipse(currentParticle, 4.0f, 4.0f);
    }
}

/**
 * @brief PhysicsRender::paintBox2D
 * Helper method that draws the particles according to their specific position and color
 *
 * @param painter - copy of the painter
 * @param positions - positions of all the particles
 * @param size
 */
void PhysicsRender::paintBox2D(QPainter *painter, b2Body *positions) {
    double halfCanvasX = (width() / 2.0);
    double halfCanvasY = (height() / 2.0);

    QPen pen;
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidthF(0);
    pen.setColor(QColor(0,0,0,0));
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter->setPen(pen);

    while(positions != NULL) {
        QColor *color = (QColor*)positions->GetUserData();
        if(color->alpha() > 0) {
            brush.setColor(*color);
            painter->setBrush(brush);

            b2PolygonShape *shape = (b2PolygonShape*)positions->GetFixtureList()[0].GetShape();
            double shapeWidth = distanceBetweenVec(shape->GetVertex(0), shape->GetVertex(1));
            double shapeHeight = distanceBetweenVec(shape->GetVertex(0), shape->GetVertex(3));
            double shapeX = halfCanvasX + positions->GetPosition().x;
            double shapeY = halfCanvasY - positions->GetPosition().y;

            QTransform adjustmentMatrix;
            adjustmentMatrix.translate(shapeX, shapeY);
            adjustmentMatrix.rotateRadians(-positions->GetAngle());
            painter->setTransform(adjustmentMatrix);
            QRectF rectToDraw(-(shapeWidth / 2), -(shapeHeight / 2), shapeWidth, shapeHeight);
            painter->drawRect(rectToDraw);
        }
        positions = positions->GetNext();
    }
    painter->setTransform(QTransform());
}

/**
 * @brief PhysicsRender::distanceBetweenVec
 * Helper method to calculate the distance between 2 vectors where their
 * two roots are at the origin.
 *
 * @param vec1
 * @param vec2
 * @return a double that is the distance beteen the two vectors
 */
double PhysicsRender::distanceBetweenVec(b2Vec2 vec1, b2Vec2 vec2)
{
    double xLength = abs(vec1.x - vec2.x);
    double yLength = abs(vec1.y - vec2.y);
    return sqrt((pow(xLength, 2) + pow(yLength, 2)));
}

