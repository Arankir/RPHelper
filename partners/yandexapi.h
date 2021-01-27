#ifndef YANDEXAPI_H
#define YANDEXAPI_H

#include <QObject>
#include "networking/requestdata.h"
#include <QSettings>
#include <QTimer>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>
#include <QDateTime>

class YandexAPI : public QObject {
    Q_OBJECT
public:
    YandexAPI(QObject *parent = nullptr);
    YandexAPI(QString token, QObject *parent = nullptr);
    void setToken(QString token) {_token = token;}
    void getPassword(QString login);
    void authorization(QString login, QString password);
    void updatePriceList(QString pricelist);
    void updateConfigurationAGZS(QJsonObject configuration);
    void updateConfigurationAGZS(QString configuration);
    int checkOrders();
    void receiveVolumeReport(QDateTime sdate, QDateTime edate, int page = 0);
    void getStateAGZS(QString apikey);
    void setStatusAccept(QString orderId, int vCode);
    void setStatusFueling(QString orderId, int vCode);
    void setStatusCanceled(QString orderId, QString reason, QString extendedOrderId, QDateTime extendedDate);
    void setStatusCompleted(QString orderId, double litre, QString extendedOrderId, QDateTime extendedDate);
    void setFuelNow(QString orderId, double litre);

signals:
    void s_setTimer(int mSec);
    void s_needAuth(bool);
    void s_authComplete(QString token);
    void s_updatePrice();
    void s_gotOrders(QJsonDocument orders);
    void s_error(QString status, QString order, int code);
    void s_networkRequestInfo(QString api, QString type, QString request, QString post, int code);

private slots:
    QNetworkRequest createRequest(QString url, QString contentType, bool auth);
    void checkAuth(RequestData*);
    void saveToken(RequestData*);

private:
    RequestData *_request;
    QString _token;
    QTimer _timer;
    const QString c_baseTest = "http://cabinet.tst.tanker.yandex.ru";
    const QString c_baseRelease = "https://cabinet.tanker.yandex.net";
    const QString c_baseUrl;

};

#endif // YANDEXAPI_H
