#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "physicsworld.h"
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QTransform>
#include <iostream>

using namespace std;



MainWindow::MainWindow(PhysicsWorld& world, Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&model, &Model::playerLost,
            this, &MainWindow::displayPlayerLost);
    connect(&model, &Model::playerWon,
            this, &MainWindow::displayPlayerWon);
    connect(&model, &Model::updateProgress,
            this, &MainWindow::updateProgressBar);
    connect(this, &MainWindow::checkStep,
            &model,&Model::checkStep);
    connect(&model, &Model::checkStepResponse,
           this, &MainWindow::displayStepResponse);
    connect(this, &MainWindow::setRecipe,
            &model,&Model::setRecipe);

    // cup
    ui->cupLabel->setPixmap(QPixmap(":/images/cup.png"));
    ui->statusLabel->hide();

    // Items
    ui->milkButton->setIconSize(QSize(121,111));
    ui->milkButton->setIcon(QIcon(":/images/milk.png"));
    ui->milkButton->setToolTip("Milk");
    ui->espressoButton->setIconSize(QSize(121,111));
    ui->espressoButton->setIcon(QIcon(":/images/espresso.png"));
    ui->espressoButton->setToolTip("Single espresso Shot");
    ui->vanillaButton->setIconSize(QSize(121,111));
    ui->vanillaButton->setIcon(QIcon(":/images/bottle.png"));
    ui->vanillaButton->setToolTip("Vanilla");
    ui->iceButton->setIconSize(QSize(121,111));
    ui->iceButton->setIcon(QIcon(":/images/ice.png"));
    ui->iceButton->setToolTip("Ice");
    ui->hotWaterButton->setIconSize(QSize(121,111));
    ui->hotWaterButton->setIcon(QIcon(":/images/coffee.png"));
    ui->hotWaterButton->setToolTip("Regular Coffee");
    //coffees

    //LiquidFun Render
    physicsUpdate = new QTimer(this);

    connect(physicsUpdate, &QTimer::timeout,
            &world, &PhysicsWorld::update);
    connect(&world, &PhysicsWorld::sendParticleData,
            ui->renderLabel, &PhysicsRender::receiveParticleData);
    connect(&world, &PhysicsWorld::sendB2Data,
            ui->renderLabel, &PhysicsRender::receiveB2Data);
    connect(&world, &PhysicsWorld::redrawRender,
            ui->renderLabel, &PhysicsRender::redraw);
    connect(this, &MainWindow::sendColorChange,
            &world, &PhysicsWorld::addLiquid);
    connect(this, &MainWindow::removeLiquid,
            &world, &PhysicsWorld::removeLiquid);
    connect(this, &MainWindow::addIceCubes,
            &world, &PhysicsWorld::addIceCubes);

    delay = ((1 * 1000) / 60); //240 updates per second
    physicsUpdate->start(delay);

    // Preview
    ui->previewStep1->hide();
    ui->previewStep2->hide();
    ui->previewStep3->hide();
    ui->previewStep4->hide();
    ui->previewStep5->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief MainWindow::on_milkButton_clicked Qt auto-generated slot for milk item.
 */
void MainWindow::on_milkButton_clicked()
{
//    // 0: milk    1: espresso    2: vanilla    3: ice
    ui->progressBar->setValue(0);
    movesTaken++;
    emit checkStep(0);
    counter = 0;
    rotates(":/images/milk.png", -110.0);
    currImagePath = ":/images/milk.png";
    ingredientsEnabled(true);

    emit sendColorChange("milk");
    physicsUpdate->start(delay);
}

/**
 * @brief MainWindow::on_espressoButton_clicked Qt auto-generated slot for espresso item.
 */
void MainWindow::on_espressoButton_clicked()
{
    movesTaken++;
    emit checkStep(1);
    counter = 0;
    currImagePath = ":/images/espresso.png";
    rotates(":/images/espresso.png", -85.0);
    expresso = true;
    expressoCounter = 0;

    ingredientsEnabled(true);
    emit sendColorChange("coffee");
    physicsUpdate->start(delay);
}

/**
 * @brief MainWindow::on_vanillaButton_clicked Qt auto-generated slot for vanilla item.
 */
void MainWindow::on_vanillaButton_clicked()
{
    movesTaken++;
    emit checkStep(2);
    counter = 0;
    rotates(":/images/bottle.png", -140.0);
    currImagePath = ":/images/bottle.png";
    ingredientsEnabled(true);

    emit sendColorChange("vanilla");
    physicsUpdate->start(delay);
}

/**
 * @brief MainWindow::on_iceButton_clicked Qt auto-generated slot for ice item.
 */
void MainWindow::on_iceButton_clicked()
{
    movesTaken++;
    emit checkStep(3);
    counter = 0;
    rotates(":/images/ice.png", -50.0);
    currImagePath = ":/images/ice.png";
    ingredientsEnabled(true);

    emit addIceCubes(0.0f);
    emit addIceCubes(30.0f);
    physicsUpdate->start(delay);
}

/**
 * @brief MainWindow::on_hotWaterButton_clicked Qt auto-generated slot for hot-water item.
 */
void MainWindow::on_hotWaterButton_clicked()
{
    movesTaken++;
    emit checkStep(4);
    counter = 0;
    rotates(":/images/coffee.png", -80.0);
    currImagePath = ":/images/coffee.png";
    ingredientsEnabled(true);

    emit sendColorChange("water");
    physicsUpdate->start(delay);
}

/**
 * @brief MainWindow::rotates Called on coffee item, places the item over the main cup and rotates counter-clock wise until it is appropriate to render the particles.
 * @param imagePath The image to be rotated
 * @param rotationAngle Angle for which the specified item should rotate to/until.
 */
void MainWindow::rotates(QString imagePath, double rotationAngle)
{
    while(counter > rotationAngle){
        QApplication::processEvents();
        QPixmap ship(imagePath);
        QPixmap rotate(ship.size());
        rotate.fill(Qt::transparent);

        QPainter p(&rotate);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.translate(rotate.size().height() / 2, rotate.size().width() / 2);
        p.rotate(counter);
        p.translate(-rotate.size().height() / 2, -rotate.size().width() / 2);
        p.drawPixmap(0, 20, ship);
        p.end();
        ui->itemLabel->setPixmap(rotate);
        counter = counter - 0.08;

        ingredientsEnabled(false);
    }
    expressoCounter++;
    QTimer::singleShot(1000, this, SLOT(rotatesBack()));
}

/**
 * @brief MainWindow::rotatesBack To be called after rotates and sets the specified (rotated) item back to its original orientation.
 */
void MainWindow::rotatesBack()
{
    while(counter < 0){
        QApplication::processEvents();
        QPixmap ship(currImagePath);
        QPixmap rotate(ship.size());
        rotate.fill(Qt::transparent);

        QPainter p(&rotate);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.translate(rotate.size().height() / 2, rotate.size().width() / 2);
        p.rotate(counter);
        p.translate(-rotate.size().height() / 2, -rotate.size().width() / 2);

        p.drawPixmap(0, 20, ship);
        p.end();

        ui->itemLabel->setPixmap(rotate);
        counter = counter + 0.08;
    }

    if(expresso && expressoCounter < 1)
    {
       rotates(":/images/espresso.png", -85.0);
       ingredientsEnabled(true);
       emit sendColorChange("coffee");
       physicsUpdate->start(delay);
    }
}


/**
 * @brief MainWindow::on_latteButton_clicked Qt auto-generated slot for Latte coffee recipe.
 */
void MainWindow::on_latteButton_clicked()
{
    emit setRecipe("latte");

    ui->statusLabel->hide();
    ui->progressBar->setValue(0);
    totalMoves = 2;
    movesTaken = 0;
    resetStepLabels();
    ui->stepLabel->setText("1. Add 2 espresso shots");
    ui->stepLabel2->setText("2. Add milk");

    emit removeLiquid();

}

/**
 * @brief MainWindow::on_macchiatoButton_clicked Qt auto-generated slot for Macchiato coffee recipe.
 */
void MainWindow::on_macchiatoButton_clicked()
{
    emit setRecipe("macchiato");

    ui->statusLabel->hide();
    ui->progressBar->setValue(0);
    totalMoves = 4;
    movesTaken = 0;
    resetStepLabels();
    ui->stepLabel->setText("1. Add 1 scoop of ice");
    ui->stepLabel2->setText("2. Add 2 espresso shots");
    ui->stepLabel3->setText("3. Add milk");
    ui->stepLabel4->setText("4. Add vanilla flavoring");

    emit removeLiquid();
}

/**
 * @brief MainWindow::on_redEye_clicked Qt auto-generated slot for red-eye coffee recipe.
 */
void MainWindow::on_redEye_clicked()
{
    emit setRecipe("red eye");

    ui->statusLabel->hide();
    ui->progressBar->setValue(0);
    totalMoves = 2;
    movesTaken = 0;
    resetStepLabels();
    ui->stepLabel->setText("1. Add 2 espresso shots");
    ui->stepLabel2->setText("2. Add hot water");
    emit removeLiquid();
}

/**
 * @brief MainWindow::on_vanillaCoffee_clicked Qt auto-generated slot for vanilla coffee recipe.
 */
void MainWindow::on_vanillaCoffee_clicked()
{
    emit setRecipe("vanilla coffee");

    ui->statusLabel->hide();
    ui->progressBar->setValue(0);
    totalMoves = 2;
    movesTaken = 0;
    resetStepLabels();
    ui->stepLabel->setText("1. Add regular coffee");
    ui->stepLabel2->setText("2. Add vanilla flavor");
    emit removeLiquid();
}

/**
 * @brief MainWindow::on_icedCoffee_clicked Qt auto-generated slot for Iced coffee recipe.
 */
void MainWindow::on_icedCoffee_clicked()
{

    emit setRecipe("ice coffee");

    ui->statusLabel->hide();
    ui->progressBar->setValue(0);
    totalMoves = 2;
    movesTaken = 0;
    resetStepLabels();
    ui->stepLabel->setText("1. Add ice");
    ui->stepLabel2->setText("2. Add regular coffee");
    emit removeLiquid();
}

/**
 * @brief MainWindow::ingredientsEnabled Sets all ingredient buttons to be enabled so the user may interact.
 * @param enable Sets ingredient QPushButtons to be enabled if true, otherwise does nothing.
 */
void MainWindow::ingredientsEnabled(bool enable)
{
    ui->espressoButton->setEnabled(enable);
    ui->iceButton->setEnabled(enable);
    ui->milkButton->setEnabled(enable);
    ui->vanillaButton->setEnabled(enable);
}

/**
 * @brief MainWindow::displayPlayerLost To be called when player incorrectly adds ingredients relative to the current coffee recipe.
 * displays text on a game status label
 */
void MainWindow::displayPlayerLost()
{
    ui->progressBar->setValue(0);
    ui->statusLabel->setText("OOPS!:(\n Select a Coffee to try again!");
    ui->statusLabel->show();
}

/**
 * @brief MainWindow::displayPlayerWon To be called when player correctly adds ingredients relative to the current coffee recipe.
 * Displays text on a game status label
 */
void MainWindow::displayPlayerWon()
{
    ui->statusLabel->setStyleSheet("QLabel { color: rgb(161, 187, 219);}");
    ui->statusLabel->setText("CONGRATS!");
    ui->statusLabel->show();
    emit steam();
}

/**
 * @brief MainWindow::updateProgressBar Increases the value of progress bar when a player matches an ingredient for the currrent
 * coffee recipe
 */
void MainWindow::updateProgressBar()
{
    double percentage = (movesTaken/totalMoves)*100;
    ui->progressBar->setValue(percentage);
}

/**
 * @brief MainWindow::markLabel Marks a given label by setting its background white and the text green.
 * @param x The label to be marked
 */
void MainWindow::markLabel(int x)
{
    switch(x){
    case(1):
        ui->stepLabel->setStyleSheet("QLabel { background-color : white; color : green; }");
        ui->stepLabel->show();
        break;
    case(2):
        ui->stepLabel2->setStyleSheet("QLabel { background-color : white; color : green; }");
        ui->stepLabel2->show();
        break;
    case(3):
        ui->stepLabel3->setStyleSheet("QLabel { background-color : white; color : green; }");
        ui->stepLabel3->show();
        break;
    case(4):
        ui->stepLabel4->setStyleSheet("QLabel { background-color : white; color : green; }");
        ui->stepLabel4->show();
        break;
    case(5):
        ui->stepLabel5->setStyleSheet("QLabel { background-color : white; color : green; }");
        ui->stepLabel5->show();
        break;
    }
}

/**
 * @brief MainWindow::markLabelIncorrect Marks a given label by setting its background white and its text red.
 * @param x The label to be marked
 */
void MainWindow::markLabelIncorrect(int x){
    switch(x){
    case(1):
        ui->stepLabel->setStyleSheet("QLabel { background-color : white; color : red; }");
        ui->stepLabel->show();
        break;
    case(2):
        ui->stepLabel2->setStyleSheet("QLabel { background-color : white; color : red; }");
        ui->stepLabel2->show();
        break;
    case(3):
        ui->stepLabel3->setStyleSheet("QLabel { background-color : white; color : red; }");
        ui->stepLabel3->show();
        break;
    case(4):
        ui->stepLabel4->setStyleSheet("QLabel { background-color : white; color : red; }");
        ui->stepLabel4->show();
        break;
    case(5):
        ui->stepLabel5->setStyleSheet("QLabel { background-color : white; color : red; }");
        ui->stepLabel5->show();
        break;
    }
}

/**
 * @brief MainWindow::on_closeOpeningButton_clicked Qt auto-generated slot that hides the starting screen to inform the player they may begin
 * to interact
 */
void MainWindow::on_closeOpeningButton_clicked()
{
    ui->openingMessage->hide();
}

/**
 * @brief MainWindow::on_previewButton_clicked Qt auto-generated slot that begins the tutorial sequence.
 */
void MainWindow::on_previewButton_clicked()
{
    ui->openingMessage->hide();
    QTimer::singleShot(1000, Qt::PreciseTimer, this, &MainWindow::previewStep1);
    QTimer::singleShot(4000, Qt::PreciseTimer, this, &MainWindow::previewStep2);
    QTimer::singleShot(8000, Qt::PreciseTimer, this, &MainWindow::previewStep3);
    QTimer::singleShot(16000, Qt::PreciseTimer, this, &MainWindow::previewStep4);
    QTimer::singleShot(23000, Qt::PreciseTimer, this, &MainWindow::previewStep5);
}

/**
 * @brief MainWindow::previewStep1 Displays the first step in the preview.
 */
void MainWindow::previewStep1()
{
    ui->previewStep1->show();
    ui->latteButton->setStyleSheet("background-color: rgb(161, 187, 219)");
    MainWindow::on_latteButton_clicked();
}

/**
 * @brief MainWindow::previewStep2 Displays the 2nd step in the preview
 */
void MainWindow::previewStep2()
{
    ui->latteButton->setStyleSheet("");
    ui->previewStep1->hide();
    ui->previewStep2->show();
}

/**
 * @brief MainWindow::previewStep3 Displays the 3rd step in the preview
 */
void MainWindow::previewStep3()
{
    ui->previewStep2->hide();
    ui->previewStep3->show();
    QTimer::singleShot(2000, Qt::PreciseTimer, this, &MainWindow::previewAddEspresso);
    QTimer::singleShot(2400, Qt::PreciseTimer, this, &MainWindow::previewEspressoUnclick);
}

/**
 * @brief MainWindow::previewAddEspresso Sets espresso as recipe in the tutorial/preview
 */
void MainWindow::previewAddEspresso()
{
    ui->espressoButton->setStyleSheet("background-color: rgb(161, 187, 219)");
    MainWindow::on_espressoButton_clicked();
}

/**
 * @brief MainWindow::previewEspressoUnclick Reverts espresso button back to original unset style.
 */
void MainWindow::previewEspressoUnclick()
{
    ui->espressoButton->setStyleSheet("");
}

/**
 * @brief MainWindow::previewStep4 Displays the 4th step in the preview.
 */
void MainWindow::previewStep4()
{
    ui->previewStep3->hide();
    ui->previewStep4->show();
    QTimer::singleShot(2000, Qt::PreciseTimer, this, &MainWindow::previewAddMilk);
    QTimer::singleShot(2400, Qt::PreciseTimer, this, &MainWindow::previewMilkUnclick);
}

/**
 * @brief MainWindow::previewAddMilk Adds milk to coffee cup in tutorial
 */
void MainWindow::previewAddMilk()
{
    ui->milkButton->setStyleSheet("background-color: rgb(161, 187, 219)");
    MainWindow::on_milkButton_clicked();
}

/**
 * @brief MainWindow::previewMilkUnclick Reverts milk push button back to original unset style.
 */
void MainWindow::previewMilkUnclick()
{
    ui->milkButton->setStyleSheet("");
}

/**
 * @brief MainWindow::previewStep5 Displays the 5th step in the tutorial.
 */
void MainWindow::previewStep5()
{
    ui->previewStep4->hide();
    ui->previewStep5->show();
}

/**
 * @brief MainWindow::on_closePreviewButton_clicked Displays the end of the preview/tutorial.
 */
void MainWindow::on_closePreviewButton_clicked()
{
    resetStepLabels();
    ui->previewStep5->hide();
}

/**
 * @brief MainWindow::on_previewAgainButton_clicked Begins the tutorial sequence once again.
 */
void MainWindow::on_previewAgainButton_clicked()
{
    resetStepLabels();
    ui->progressBar->setValue(0);
    ui->previewStep5->hide();
    MainWindow::on_previewButton_clicked();
}

/**
 * @brief MainWindow::resetStepLabels Blanks out all steps that have been marked.
 */
void MainWindow::resetStepLabels(){
    ui->stepLabel->setStyleSheet("");
    ui->stepLabel->clear();
    ui->stepLabel2->setStyleSheet("");
    ui->stepLabel2->clear();
    ui->stepLabel3->setStyleSheet("");
    ui->stepLabel3->clear();
    ui->stepLabel4->setStyleSheet("");
    ui->stepLabel4->clear();
    ui->stepLabel5->setStyleSheet("");
    ui->stepLabel5->clear();
}

/**
 * @brief MainWindow::displayStepResponse If isCorrect is true marks the label correctly, otherwise marks step as incorrect.
 * @param isCorrect
 */
void MainWindow::displayStepResponse(bool isCorrect){
    if (isCorrect){
          markLabel(movesTaken);
    }
    else{
          markLabelIncorrect(movesTaken);
    }
}

