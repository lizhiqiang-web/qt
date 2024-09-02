#include "updatameg.h"
#include "ui_updatameg.h"

UpDataMeg::UpDataMeg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpDataMeg)
{
    ui->setupUi(this);
}

UpDataMeg::~UpDataMeg()
{
    delete ui;
}
