#include "citymobileapi.h"

CityMobileAPI::CityMobileAPI(QObject *parent) : QObject(parent), _request(new RequestData()), _reestr("RegionPostavka", "Partners"),
    _apiKey(_reestr.value("CityMobile Token", "").toString()), c_baseUrl(_reestr.value("isTest").toBool()? c_baseTest: c_baseRelease) {
    qDebug()<<"CityMobile Token ="<<_apiKey<<"\nisTest ="<<_reestr.value("isTest").toString();
    QObject::connect(_request, &RequestData::s_request, this, [=](QString type, QString request, QString post, int code) {
        emit s_networkRequestInfo("CityMobile", type, request, post, code);
    });
}

CityMobileAPI::~CityMobileAPI() {
    delete _request;
}

void CityMobileAPI::updateConfigurationAGZS(QJsonDocument aPost) {
    QNetworkRequest request = createRequest("api/station", "application/json", true);
    _request->post(request,QString(aPost.toJson(QJsonDocument::Compact)).toUtf8());
}

void CityMobileAPI::updatePriceList(QString aPost) {
    QNetworkRequest request = createRequest("api/price", "application/x-www-form-urlencoded", true);
    _request->post(request,aPost.toUtf8());
}

void CityMobileAPI::checkOrders() {
    QNetworkRequest request = createRequest("api/orders/items", "application/x-www-form-urlencoded", true);
    _request->get(request);
    QJsonDocument jsonRequest = QJsonDocument::fromJson(_request->getAnswer());
    emit s_gotOrders(jsonRequest);
    //qDebug() << "Ситимобил заказы:" << _request->getAnswer();
    //connect(_request, &RequestData::s_finished, this, &CityMobileAPI::checkOrders);
}

void CityMobileAPI::setStatusAccept(QString aOrderId, int aVCode) {
    QNetworkRequest request = createRequest("api/orders/accept?orderId=" + aOrderId, "application/x-www-form-urlencoded", true);
    _request->get(request);
    if ((_request->getCode() != 200) && (_request->getCode() != 401)) {
        emit s_error("setStatusAccept", aOrderId, _request->getCode());
        setStatusCanceled(aOrderId, "Неизвестная ошибка.", QString::number(aVCode), QDateTime::currentDateTime());
    }
}

void CityMobileAPI::setStatusFueling(QString aOrderId, int aVCode) {
    Q_UNUSED(aVCode);
    QNetworkRequest request = createRequest("api/orders/fueling?orderId=" + aOrderId, "application/x-www-form-urlencoded", true);
    _request->get(request);
    if ((_request->getCode() != 200) && (_request->getCode() != 401)) {
        emit s_error("setStatusFueling", aOrderId, _request->getCode());
        //setStatusCanceled(orderId, "Неизвестная ошибка.", QString::number(vCode), QDateTime::currentDateTime());
    }
}

void CityMobileAPI::setStatusCanceled(QString aOrderId, QString aReason, QString aExtendedOrderId, QDateTime aExtendedDate) {
    QNetworkRequest request = createRequest("api/orders/canceled?orderId=" + aOrderId +
                                                    "&reason=\"" + aReason.replace(" ","%20") + "\""
                                                    "&extendedOrderId=" + aExtendedOrderId +
                                                    "&extendedDate=" + aExtendedDate.toString("dd.MM.yyyy HH:mm:ss")
                                            , "application/x-www-form-urlencoded", true);
    _request->get(request);
}

void CityMobileAPI::setStatusCompleted(QString aOrderId, double aLitre, QString aExtendedOrderId, QDateTime aExtendedDate) {
    QNetworkRequest request = createRequest(QString("api/orders/completed?orderId=%1&litre=%2&extendedOrderId=%3&extendedDate=%4").arg(
                                                aOrderId,
                                                QString::number(aLitre).replace(",","."),
                                                aExtendedOrderId,
                                                aExtendedDate.toString("dd.MM.yyyy HH:mm:ss")), "application/x-www-form-urlencoded", true);
    _request->get(request);
}

void CityMobileAPI::setStatusFuelNow(QString aOrderId, double aLitre) {
    QNetworkRequest request = createRequest("api/orders/volume", "application/x-www-form-urlencoded", true);
    _request->post(request,QString("orderId=" + aOrderId + "&litre=" + QString::number(aLitre).replace(",",".")).toUtf8());
}

void CityMobileAPI::setStatusMessage(QString aOrderId, QString aMessage) {
    QNetworkRequest request = createRequest("api/orders/message", "application/x-www-form-urlencoded", true);
    _request->post(request,QString("orderId=" + aOrderId + "&msg=" + aMessage).toUtf8());
}

QNetworkRequest CityMobileAPI::createRequest(QString aUrl, QString aContentType, bool aAuth) {
    QNetworkRequest request;
    request.setUrl(QUrl(c_baseUrl + aUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,aContentType);
    if (aAuth) {
        request.setRawHeader("Authorization",_apiKey.toUtf8());
    }
    return request;
}
