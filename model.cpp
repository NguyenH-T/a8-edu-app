#include "model.h"
#include <iostream>
#include <string>

Model::Model(QObject* parent) : QObject{parent}
{
    redEyeList.push_back(1);
    redEyeList.push_back((4));
    //add regCoffee

    //add regCoffee
    vanillaCoffeeList.push_back(2);
    vanillaCoffeeList.push_back(4);


    macchiatoList.push_back(3);
    macchiatoList.push_back(1);
    macchiatoList.push_back(0);
    macchiatoList.push_back(2);

    latteList.push_back(1);
    latteList.push_back(0);

    icedCoffeeList.push_back(3);
    icedCoffeeList.push_back(4);
}

/**
 * @brief Model::checkStep Checks whether or not the user made a valid move in crafting the coffee recipe
 * emits playerLost if incorrect and markLabelIncorrect.  If list is empty after a valid move emit playerWon, increase progress bar and markLabel as correct.
 * @param x The integer corresponding the the coffee item used (0=>milk, 1=>espresso, 2=>vanilla, 3=> ice)
 */
void Model::checkStep(int x)
{
    if (!currentList.isEmpty() && currentList.first() != x){
        emit playerLost();
        emit checkStepResponse(false);
    }
    else if (!currentList.isEmpty() && currentList.first() == x){
        currentList.pop_front();
        if (currentList.isEmpty()){
            emit playerWon();
        }
        emit updateProgress();
        emit checkStepResponse(true);
    }
}

/**
 * @brief Model::setRecipe Populates a vector with the steps needed to make a given coffee recipe
 * @param recipe The recipe for which to populate the currList Vector.
 */
void Model::setRecipe(QString recipe)
{
    if (recipe == "latte")
        currentList = latteList;
    else if (recipe == "red eye")
        currentList = redEyeList;
    else if (recipe == "vanilla coffee")
        currentList = vanillaCoffeeList;
    else if (recipe == "macchiato")
        currentList = macchiatoList;
    else if (recipe == "ice coffee")
        currentList = icedCoffeeList;
}
