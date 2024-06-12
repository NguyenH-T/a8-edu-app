#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "physicsworld.h"
#include "model.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PhysicsWorld& world, Model& model, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
     void updateProgressBar();
     void rotatesBack();


private slots:
    void on_milkButton_clicked();
    void rotates(QString s, double rotationAngle);
    void on_espressoButton_clicked();
    void on_vanillaButton_clicked();
    void on_iceButton_clicked();
    void on_latteButton_clicked();
    void on_macchiatoButton_clicked();
    void on_redEye_clicked();
    void on_vanillaCoffee_clicked();
    void on_icedCoffee_clicked();
    void displayPlayerWon();
    void displayPlayerLost();

    void on_closeOpeningButton_clicked();
    void on_previewButton_clicked();

    void previewStep1();
    void previewStep2();
    void previewStep3();
    void previewAddEspresso();
    void previewEspressoUnclick();
    void previewStep4();
    void previewAddMilk();
    void previewMilkUnclick();
    void previewStep5();
    void displayStepResponse(bool);

    void on_closePreviewButton_clicked();
    void on_previewAgainButton_clicked();
    void on_hotWaterButton_clicked();

signals:
    void sendColorChange(std::string);
    void removeLiquid();
    void addIceCubes(float x);
    void checkStep(int);
    void initStepsAmericano();
    void initStepsAmericano2();
    void initStepsLatteButton();
    void initStepsMacchiato();
    void setRecipe(QString);
    void steam();

private:
    Ui::MainWindow *ui;
    double counter = 0;
    QString currImagePath;
    bool expresso = false;
    int expressoCounter = 0;
    void ingredientsEnabled(bool);
    void hideCheckBoxes();
    void markLabel(int);
    void markLabelIncorrect(int);
    void clearLabels();
    void resetStepLabels();
    double movesTaken;
    double totalMoves;
    QTimer *physicsUpdate;
    double delay;

};
#endif // MAINWINDOW_H
