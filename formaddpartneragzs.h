#ifndef FORMADDPARTNERAGZS_H
#define FORMADDPARTNERAGZS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include "partners/yandexapi.h"
#include "database.h"

namespace Ui {
class FormAddPartnerAgzs;
}

class FormAddPartnerAgzs : public QWidget
{
    Q_OBJECT

public:
    explicit FormAddPartnerAgzs(QWidget *parent = nullptr);
    ~FormAddPartnerAgzs();

private slots:
    //init
    void initAgzs();
    void initColumnsTables();
    //Ui
    void priceToUi(AgzsPrice aPrice, int aIPartner);
    AgzsPrice uiToPrice(int aIPartner);
    void dataToUi(AgzsData data);
    AgzsData uiToData();
    void columnsToUi(QList<AgzsColumn> columnsDb, QList<AgzsColumn> columnsAgzs);
    QList<AgzsColumn> UiToColumns();
    //copy prices
    void on_PushButtonCopyYandexToCityMobile_clicked();
    void on_PushButtonCopyCityMobileToYandex_clicked();
    //others
    void setEnableSettings(bool isEnabled);
    void on_ComboBoxAgzs_currentIndexChanged(int index);
    void on_PushButtonCancel_clicked();

    void on_PushButtonAcceptAgzsData_clicked();
    void on_ButtonRegisterCityMobile_clicked();

    void on_PushButtonAcceptAgzsColumns_clicked();
    void on_PushButtonAddColumn_clicked();
    void on_PushButtonDeleteRow_clicked();
    void on_PushButtonReplaceColumns_clicked();

    void on_PushButtonAcceptPriceYandex_clicked();
    void on_PushButtonAcceptPriceCityMobile_clicked();
    void on_PushButtonAcceptPrice_clicked();

    void on_PushButtonAcceptPriceFromDateYandex_clicked();
    void on_PushButtonAcceptPriceFromDateCityMobile_clicked();
    void on_PushButtonAcceptPriceFromDate_clicked();

    void on_PushButtonFindColumn_clicked();

private:
    Ui::FormAddPartnerAgzs *ui;
};

#endif // FORMADDPARTNERAGZS_H
