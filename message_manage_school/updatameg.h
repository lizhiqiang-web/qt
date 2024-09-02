#ifndef UPDATAMEG_H
#define UPDATAMEG_H

#include <QDialog>

namespace Ui {
class UpDataMeg;
}

class UpDataMeg : public QDialog
{
    Q_OBJECT

public:
    explicit UpDataMeg(QWidget *parent = nullptr);
    ~UpDataMeg();

private:
    Ui::UpDataMeg *ui;
};

#endif // UPDATAMEG_H
