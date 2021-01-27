#ifndef FORMCANCELPARTNERTRANSACTION_H
#define FORMCANCELPARTNERTRANSACTION_H

#include <QWidget>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "partners/yandexapi.h"

namespace Ui {
class FormCancelPartnerTransaction;
}

class FormCancelPartnerTransaction : public QWidget
{
    Q_OBJECT

public:
    explicit FormCancelPartnerTransaction(QWidget *parent = nullptr);
    ~FormCancelPartnerTransaction();

private slots:
    void initAgzsList();
    bool initDB();
    bool openDB();
    void on_ButtonCancelCity_clicked();
    void on_ButtonCancelYandex_clicked();
    bool getTokens(int aAgzs, QString &aCityMobileToken, QString &aYandexToken);
    QStringList getAgzsList();

private:
    Ui::FormCancelPartnerTransaction *ui;
    YandexAPI yandex_;
    //CityMobileAPI cityMobile_;
    QSqlDatabase db_;
};

#endif // FORMCANCELPARTNERTRANSACTION_H
