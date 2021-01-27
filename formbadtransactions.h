#ifndef FORMBADTRANSACTIONS_H
#define FORMBADTRANSACTIONS_H

#include <QWidget>
#include <QDateTime>
#include <QCheckBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "database.h"

namespace Ui {
class FormBadTransactions;
}

class FormBadTransactions : public QWidget
{
    Q_OBJECT

public:
    explicit FormBadTransactions(QWidget *parent = nullptr);
    ~FormBadTransactions();

private slots:
    void on_PushButtonConnect_clicked();
    void on_PushButtonFindAgzs_clicked();

    void on_CheckBoxCheckAll_stateChanged(int arg1);
    void checkBoxAgzs_stateChanged(int arg1);

private:
    Ui::FormBadTransactions *ui;
    QString login_ = "";
    QString password_ = "";
};

#endif // FORMBADTRANSACTIONS_H
