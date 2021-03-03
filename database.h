#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

const QString c_defaultLogin = "grimnir";
const QString c_defaultPassword = "Gfhf743Djpbr";
const bool c_defaultMessage = true;

struct Agzs {
    int number;
    QString name;
    QString engName;

    bool operator==(const Agzs &agzs) {
        return (engName == agzs.engName);
    }
    bool operator<(const Agzs &agzs) {
        return (engName < agzs.engName);
    }
};

struct AgzsPrice {
    QString agzsName;
    int agzs;
    int link;
    QString partner;
    QDateTime cDate;
    int vCode;
    float diesel;
    float diesel_premium;
    float a80;
    float a92;
    float a92_premium;
    float a95;
    float a95_premium;
    float a98;
    float a98_premium;
    float a100;
    float a100_premium;
    float propane;
    float metan;
    int iPartner;
    QDateTime dateStart;
};

struct AgzsData {
    QString agzsName;
    int agzs = -1;
    QDateTime cDate;
    int vCode;
    QString id;
    QString adress;
    double latitude;
    double longitude;
    int columnsCount;
    QString AgzsL;
    QString AgzsP;
    QString cityMobileToken;
    QString yandexToken;
};

struct AgzsColumn {
    int agzs;
    QDateTime cDate;
    int vCode;
    int sideAdress;
    int partnerTrkNum;
    int visible;
    QString side;
    int nozzle1Fuel;
    int nozzle2Fuel;
    int nozzle3Fuel;
    int nozzle4Fuel;
    int nozzle5Fuel;
};

class DataBase : public QObject
{
    Q_OBJECT
public:
    static bool connectDB(QSqlDatabase aDb, const QString &login = c_defaultLogin,
                          const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);

    static QList<Agzs> getAgzs(QDateTime dateStart, QDateTime dateEnd, const QString &login = c_defaultLogin,
                               const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static AgzsPrice getPrice(const QString &aAgzs, int aPartnerIndex, const QString &login = c_defaultLogin,
                              const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static AgzsData getData(const QString &aAgzs, const QString &login = c_defaultLogin,
                            const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static QList<AgzsColumn> getColumnsDb(const QString &aAgzs, const QString &login = c_defaultLogin,
                                          const QString &password = c_defaultPassword,
                                          bool isShowMessage = c_defaultMessage);
    static QList<AgzsColumn> getColumnsAdast(const QString &aAgzs, const QString &aPass, int aAgzsNum,
                                             bool isShowMessage = c_defaultMessage);
    static int getLxKeyCode(const QString &key, bool autoIncrement, const QString &login = c_defaultLogin,
                            const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool updateAgzsData(const QString &aAgzsName, const int &aAgzs, const QString &aId, const QString &aAdress,
                               const double &aLatitude, const double &aLongitude, const int &aColumnsCount,
                               const QString &aAgzsL, const QString &aAgzsP, const QString &aCityMobileToken,
                               const QString &aYandexToken, const int &aVcode, const QString &login = c_defaultLogin,
                               const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool updateAgzsData(const AgzsData &aData, const QString &login = c_defaultLogin,
                               const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool insertAgzsData(const QString &aAgzsName, const int &aAgzs, const QDateTime &aCdate, const QString &aId,
                               const QString &aAdress, const float &aLatitude, const float &aLongitude,
                               const int &aColumnsCount, const QString &aAgzsL, const QString &aAgzsP,
                               const QString &aCityMobileToken, const QString &aYandexToken, const int &aVcode,
                               const QString &login = c_defaultLogin, const QString &password = c_defaultPassword,
                               bool isShowMessage = c_defaultMessage);
    static bool insertAgzsData(const AgzsData &aData, const QString &login = c_defaultLogin,
                               const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);

    static bool updateAgzsColumn(const int &aAgzs, const int &aVcode, const int &aSideAdress,
                                 const int &aPartnerTrkNum, const int &aVisible, const QString &aSide,
                                 const int &aNozzle1Fuel, const int &aNozzle2Fuel, const int &aNozzle3Fuel,
                                 const int &aNozzle4Fuel, const int &aNozzle5Fuel, const QString &login = c_defaultLogin,
                                 const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool updateAgzsColumn(const AgzsColumn &aColumn, const QString &login = c_defaultLogin,
                                 const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool insertAgzsColumn(const int &aAgzs, const QDateTime &aCdate, const int &aVcode, const int &aSideAdress,
                                 const int &aPartnerTrkNum, const int &aVisible, const QString &aSide, const int &aNozzle1Fuel,
                                 const int &aNozzle2Fuel, const int &aNozzle3Fuel, const int &aNozzle4Fuel,
                                 const int &aNozzle5Fuel, const QString &login = c_defaultLogin,
                                 const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool insertAgzsColumn(const AgzsColumn &aColumn, const QString &login = c_defaultLogin,
                                 const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool deleteAgzsColumn(const int &aVcode, const QString &login = c_defaultLogin,
                                 const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);

    static bool updateLastAgzsPrice(const QString &agzsName, const int &agzs, const int &link, const QString &partner,
                                const double &diesel, const double &diesel_premium, const double &a80,
                                const double &a92, const double &a92_premium, const double &a95,
                                const double &a95_premium, const double &a98, const double &a98_premium,
                                const double &a100, const double &a100_premium, const double &propane,
                                const double &metan, const int &iPartner, const QString &login = c_defaultLogin,
                                const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool updateLastAgzsPrice(const AgzsPrice &price, const QString &login = c_defaultLogin,
                                const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool insertAgzsPrice(const QString &agzsName, const int &agzs, const int &vCode, const int &link,
                                const QDateTime &cDate, const QString &partner, const double &diesel,
                                const double &diesel_premium, const double &a80, const double &a92,
                                const double &a92_premium, const double &a95, const double &a95_premium,
                                const double &a98, const double &a98_premium, const double &a100,
                                const double &a100_premium, const double &propane, const double &metan,
                                const int &iPartner, const QDateTime &dateStart, const QString &login = c_defaultLogin,
                                const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static bool insertAgzsPrice(const AgzsPrice &price, const QString &login = c_defaultLogin,
                                const QString &password = c_defaultPassword, bool isShowMessage = c_defaultMessage);
    static QList<QList<QVariant>> getBadTransaction(const int &aAgzsNumber, const QDateTime &aDateStart,
                                                    const QDateTime &aDateEnd, const int &aFuel,
                                                    const QString &login = c_defaultLogin,
                                                    const QString &password = c_defaultPassword,
                                                    bool isShowMessage = c_defaultMessage);

signals:

};

#endif // DATABASE_H
