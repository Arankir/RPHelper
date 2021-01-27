#include "formcancelpartnertransaction.h"
#include "ui_formcancelpartnertransaction.h"

const int c_maxRestartCount = 7;

FormCancelPartnerTransaction::FormCancelPartnerTransaction(QWidget *parent) :
QWidget(parent),
ui(new Ui::FormCancelPartnerTransaction) {
    ui->setupUi(this);
    initDB();
    initAgzsList();
}

FormCancelPartnerTransaction::~FormCancelPartnerTransaction() {
    delete ui;
}

void FormCancelPartnerTransaction::initAgzsList() {
    ui->ComboBoxAgzs->clear();
    ui->ComboBoxAgzs->addItems(getAgzsList());
}

bool FormCancelPartnerTransaction::initDB() {
    db_ = QSqlDatabase::addDatabase("QODBC3");
    db_.setDatabaseName("DRIVER={SQL Server};"
                        "SERVER=grimnir;"
                        "DATABASE=agzs;"
                        "Persist Security Info=true;"
                        "uid=sa;"
                        "pwd=Gfhf743Djpbr");
    return openDB();
}

bool FormCancelPartnerTransaction::openDB() {
    if (db_.open()) {
        qInfo () << "DB is open";
        return true;
    } else {
        qInfo () << "DB isn't open";
        return false;
    }
}

void FormCancelPartnerTransaction::on_ButtonCancelCity_clicked() {
    QString agzs    = ui->ComboBoxAgzs          ->currentText();
    QString id      = ui->LineEditCancelId      ->text();
    QString reason  = ui->LineEditCancelReason  ->text();
    if (agzs.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"АГЗС\" пустое!"));
        return;
    }
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"ID\" пустое!"));
        return;
    }
    if (reason.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"Причина отмены\" пустое!"));
        return;
    }

    QString cityMobileToken, yandexToken;
    getTokens(agzs.toInt(), cityMobileToken, yandexToken);
    if (cityMobileToken.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Отсутствует токен Ситимобила!"));
        return;
    }
    //cityMobile_.setToken(yandexToken);
    //cityMobile_.setStatusCanceled(id, reason, id, QDateTime::currentDateTime());

}

void FormCancelPartnerTransaction::on_ButtonCancelYandex_clicked() {
    QString agzs    = ui->ComboBoxAgzs          ->currentText();
    QString id      = ui->LineEditCancelId      ->text();
    QString reason  = ui->LineEditCancelReason  ->text();
    if (agzs.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"АГЗС\" пустое!"));
        return;
    }
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"ID\" пустое!"));
        return;
    }
    if (reason.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поле \"Причина отмены\" пустое!"));
        return;
    }

    QString cityMobileToken, yandexToken;
    getTokens(agzs.toInt(), cityMobileToken, yandexToken);
    if (yandexToken.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Отсутствует токен Яндекса!"));
        return;
    }
    yandex_.setToken(yandexToken);
    yandex_.setStatusCanceled(id, reason, id, QDateTime::currentDateTime());
}

bool FormCancelPartnerTransaction::getTokens(int aAgzs, QString &aCityMobileToken, QString &aYandexToken) {
    int cycles = 0;
    while (cycles < c_maxRestartCount) {
        QSqlQuery query(db_);
        query.prepare("SELECT TOP 1 "
                        "AGZSName "
                        ", AGZS "
                        ", VCode "
                        ", Adress "
                        ", CityMobileToken "
                        ", YandexToken "
                      "FROM [agzs].[dbo].[PR_AGZSData]"
                      "WHERE AGZS = :agzs");
        query.bindValue(":agzs", aAgzs);
        if (query.exec()) {
            if (query.next()) {
                aCityMobileToken    = query.value(4).toString();
                aYandexToken        = query.value(5).toString();
                return true;
            } else {
                qWarning() << "getTokens empty" << cycles;
                return false;
            }
        } else {
            qWarning() << "getTokens error" << cycles;
            if (!db_.isOpen()) {
                openDB();
            }
        }
        cycles++;
    }
    return false;
}

QStringList FormCancelPartnerTransaction::getAgzsList() {
    int cycles = 0;
    while (cycles < c_maxRestartCount) {
        QSqlQuery query(db_);
        query.exec("SELECT TOP 1000 [AGZS] "
                      "FROM [agzs].[dbo].[PR_AGZSData]");
        if (query.next()) {
            QStringList list;
            do {
                list.append(query.value(0).toString());
            } while(query.next());
            return list;
        } else {
            qWarning() << "getAgzsList error" << cycles;
            if (!db_.isOpen()) {
                openDB();
            }
        }
        cycles++;
    }
    return QStringList();
}
