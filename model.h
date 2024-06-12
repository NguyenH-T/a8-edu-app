#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>

class Model: public QObject
{
    Q_OBJECT

public:
   explicit Model(QObject* parent = nullptr);

private:
    QVector<int> redEyeList;
    QVector<int> vanillaCoffeeList;
    QVector<int> latteList;
    QVector<int> macchiatoList;
    QVector<int> currentList;
    QVector<int> icedCoffeeList;

public slots:
    void checkStep(int);
    void setRecipe(QString);

signals:
    void playerLost();
    void playerWon();
    void checkStepResponse(bool isCorrect);
    void updateProgress();
};

#endif // MODEL_H
