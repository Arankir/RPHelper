#ifndef CITYMOBILEAPI_H
#define CITYMOBILEAPI_H

#include <QObject>
#include "networking/requestdata.h"
#include <QSettings>

class CityMobileAPI : public QObject {
    Q_OBJECT
public:
    explicit CityMobileAPI(QObject *parent = nullptr);
    ~CityMobileAPI();
    void updateConfigurationAGZS(QJsonDocument post);
    void updatePriceList(QString post);
    void checkOrders();
    void setStatusAccept(QString orderId, int vCode);
    void setStatusFueling(QString orderId, int vCode);
    void setStatusCanceled(QString orderId, QString reason, QString extendedOrderId, QDateTime extendedDate);
    void setStatusCompleted(QString orderId, double litre, QString extendedOrderId, QDateTime extendedDate);
    void setStatusFuelNow(QString orderId, double litre);
    void setStatusMessage(QString orderId, QString msg);

signals:
    void s_finished(RequestData *request);
    void s_gotOrders(QJsonDocument orders);
    void s_error(QString status, QString order, int code);
    void s_networkRequestInfo(QString api, QString type, QString request, QString post, int code);

private slots:
    QNetworkRequest createRequest(QString url, QString contentType, bool auth);

private:
    RequestData *_request;
    QSettings _reestr;
    QString _apiKey;
    const QString c_baseTest = "https://terminal.api.test.fuelup.ru/";
    const QString c_baseRelease = "https://terminal.api.fuelup.ru/";
    const QString c_baseUrl;
};

#endif // CITYMOBILEAPI_H
