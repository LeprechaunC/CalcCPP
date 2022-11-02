#include "calculator.h"
#include "./ui_calculator.h"
#include <QRegularExpression>


double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{

    ui->setupUi(this);
    ui->Display->setText(QString::number(calcVal));

    // Will hold references to all the number buttons
    QPushButton *numButtons[10];

    // Cycles through all 10 buttons, assigns them to an array numButtons
    for(int i = 0; i < 10; ++i){
        QString butName = "Button" + QString::number(i);

        // Get the buttons by name and add to array
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);

        // When the button is released call num_pressed()
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

    // connect signals and slots for math buttons
    connect(ui->Add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));

    // connect equals button
    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));

    // connect change sign
    connect(ui->ChangeSign, SIGNAL(released()), this,
            SLOT(ChangeNumberSign()));
    // connect the delete button
    connect(ui->Clear, SIGNAL(released()), this,
            SLOT(DeleteAll()));
//dot button
    connect(ui->Dot, SIGNAL(released()), this,
            SLOT(DotAdd()));

    connect(ui->SquareRoot, SIGNAL(released()), this, SLOT(SqrtRoot()));

}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){

    QPushButton *button = (QPushButton *)sender();

    QString butVal = button->text();

    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){

        // calcVal = butVal.toDouble();
        ui->Display->setText(butVal);

    } else {
        // Put the new number to the right of whats there
        QString newVal = displayVal + butVal;

        // Double version of number
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed(){

    // Cancel out previous math button clicks
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    // Store current value in Display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get math symbol on the button
    QString butVal = button->text();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0)
    {
        divTrigger = true;
    }
    else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    }
    else
    {
        subTrigger = true;
    }

    ui->Display->setText("");

}

void Calculator::EqualButtonPressed(){

    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    // Make sure a math button was pressed
    if(addTrigger || subTrigger || multTrigger || divTrigger ){
        if(addTrigger){
            solution = calcVal + dblDisplayVal;
        }
        else if(subTrigger){
            solution = calcVal - dblDisplayVal;
        }
        else if(multTrigger){
            solution = calcVal * dblDisplayVal;
        }
        else
        {

            solution = calcVal / dblDisplayVal;
        }
    }

    // solution, the result
    ui->Display->setText(QString::number(solution));

}

void Calculator::ChangeNumberSign(){

    QString displayVal = ui->Display->text();
     QRegularExpression reg("[-]?[0-9.]*");
     QRegularExpressionMatch match = reg.match(displayVal);
     if(match.hasMatch())
     {

         double dblDisplayVal = displayVal.toDouble();
         double dblDisplayValSign = -1 * dblDisplayVal;
         ui->Display->setText(QString::number(dblDisplayValSign));
     }
}

void Calculator::DeleteAll()
{
   ui->Display->setText("");
}

void Calculator::DotAdd()
{
    QString previous_text = ui->Display->text();
    ui->Display->setText(previous_text + ".");
}
void Calculator::SqrtRoot()
{
    QString previous_text = ui->Display->text();
    ui->Display->setText("√" + previous_text);
}

