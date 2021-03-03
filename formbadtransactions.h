#ifndef FORMBADTRANSACTIONS_H
#define FORMBADTRANSACTIONS_H

#include <QWidget>
#include <QDateTime>
#include <QCheckBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QStandardItemModel>
#include <QMenu>
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
    void init();
    void on_PushButtonConnect_clicked();
    void findAgzs(QDateTime dateStart, QDateTime dateEnd);
    QMenu *createMenu();

    void on_CheckBoxCheckAll_stateChanged(int arg1);
    void checkBoxAgzs_stateChanged(int arg1);

    void on_PushButtonFind_clicked();

    void goToTransaction();
private:
    Ui::FormBadTransactions *ui;
    QString login_ = "";
    QString password_ = "";
};

#endif // FORMBADTRANSACTIONS_H
