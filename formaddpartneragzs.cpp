#include "formaddpartneragzs.h"
#include "ui_formaddpartneragzs.h"

#define Constants {
const int c_cityMobileIndex = 10;
const int c_yandexIndex = 11;

const int c_columnsColumnAgzs       = 0;
const int c_columnsColumnVcode      = 1;
const int c_columnsColumnSideAdress = 2;
const int c_columnsColumnPartnerNum = 3;
const int c_columnsColumnVisible    = 4;
const int c_columnsColumnSide       = 5;
const int c_columnsColumnFuel1      = 6;
const int c_columnsColumnFuel2      = 7;
const int c_columnsColumnFuel3      = 8;
const int c_columnsColumnFuel4      = 9;
const int c_columnsColumnFuel5      = 10;
#define ConstantsEnd }

FormAddPartnerAgzs::FormAddPartnerAgzs(QWidget *parent) :
QWidget(parent),
ui(new Ui::FormAddPartnerAgzs) {
    ui->setupUi(this);
    initColumnsTables();
    initAgzs();
    ui->ToolBoxAgzs->setCurrentIndex(0);

    ui->DateTimeEditPriceUpdateTime->setDateTime(QDateTime::currentDateTime());

    setEnableSettings(false);
}

FormAddPartnerAgzs::~FormAddPartnerAgzs() {
    delete ui;
}

#define InitStart {
void FormAddPartnerAgzs::initAgzs() {
    auto agzss = DataBase::getAgzs(QDateTime(QDate(QDate::currentDate().year(), 1, 1)), QDateTime::currentDateTime());
    std::sort(agzss.begin(), agzss.end());
    agzss.erase(std::unique(agzss.begin(), agzss.end()), agzss.end());

    ui->ComboBoxAgzs->clear();
    ui->ComboBoxAgzs->addItem(tr("АГЗС не выбрана"));

    for (auto &agzs: agzss) {
        ui->ComboBoxAgzs->addItem(agzs.engName);
    }
}

void FormAddPartnerAgzs::initColumnsTables() {
    QStringList headers;
    headers << tr("АГЗС") << tr("VCode") << tr("SideAdress") << tr("В приложении") << tr("Видимость")
            << tr("Side") << tr("Топливо1") << tr("Топливо2") << tr("Топливо3") << tr("Топливо4") << tr("Топливо5");

    QStandardItemModel *modelDb = new QStandardItemModel();
    modelDb->setHorizontalHeaderLabels(headers);
    ui->TableViewColumnsDb->setModel(modelDb);
    ui->TableViewColumnsDb->setColumnHidden(1, true);
    ui->TableViewColumnsDb->resizeColumnsToContents();

    QStandardItemModel *modelAgzs = new QStandardItemModel();
    modelAgzs->setHorizontalHeaderLabels(headers);
    ui->TableViewColumnsAgzs->setModel(modelAgzs);
    ui->TableViewColumnsAgzs->setColumnHidden(1, true);
    ui->TableViewColumnsAgzs->resizeColumnsToContents();
}
#define InitEnd }

void FormAddPartnerAgzs::setEnableSettings(bool isEnabled) {
    ui->ToolBoxAgzs->setEnabled(isEnabled);
}

#define MainLoad {
void FormAddPartnerAgzs::on_ComboBoxAgzs_currentIndexChanged(int aIndex) {
    setEnableSettings(aIndex != 0);
    if (aIndex == 0) {
        dataToUi(AgzsData());
        priceToUi(AgzsPrice(), c_yandexIndex);
        priceToUi(AgzsPrice(), c_cityMobileIndex);
        columnsToUi(QList<AgzsColumn>(), QList<AgzsColumn>());
        return;
    }

    const QString agzs = ui->ComboBoxAgzs->currentText();

    AgzsData data = DataBase::getData(agzs);
    if (data.AgzsL == "") {
        data.AgzsL = agzs;
        data.cDate = QDateTime::currentDateTime();
        data.vCode = DataBase::getLxKeyCode("PR_AGZSData", true);
        data.id = data.vCode;
        DataBase::insertAgzsData(data);
    }
    dataToUi(data);
    priceToUi(DataBase::getPrice(agzs, c_yandexIndex), c_yandexIndex);
    priceToUi(DataBase::getPrice(agzs, c_cityMobileIndex), c_cityMobileIndex);
//    columnsToUi(DataBase::getColumnsDb(agzs), DataBase::getColumnsAdast(agzs,
//                                                    ui->LineEditDataP->text(),
//                                                    ui->LineEditDataNumber->text().toInt()));
}

void FormAddPartnerAgzs::on_PushButtonCancel_clicked() {
    on_ComboBoxAgzs_currentIndexChanged(ui->ComboBoxAgzs->currentIndex());
}
#define MainLoadEnd }

#define UiStart {
void FormAddPartnerAgzs::priceToUi(AgzsPrice aPrice, int aIPartner) {
    switch (aIPartner) {
    case c_yandexIndex: {
        ui->DoubleSpinBoxYandexDiesel       ->setValue(aPrice.diesel);
        ui->DoubleSpinBoxYandexDieselPremium->setValue(aPrice.diesel_premium);
        ui->DoubleSpinBoxYandexA80          ->setValue(aPrice.a80);
        ui->DoubleSpinBoxYandexA92          ->setValue(aPrice.a92);
        ui->DoubleSpinBoxYandexA92Premium   ->setValue(aPrice.a92_premium);
        ui->DoubleSpinBoxYandexA95          ->setValue(aPrice.a95);
        ui->DoubleSpinBoxYandexA95Premium   ->setValue(aPrice.a95_premium);
        ui->DoubleSpinBoxYandexA98          ->setValue(aPrice.a98);
        ui->DoubleSpinBoxYandexA98Premium   ->setValue(aPrice.a98_premium);
        ui->DoubleSpinBoxYandexA100         ->setValue(aPrice.a100);
        ui->DoubleSpinBoxYandexA100Premium  ->setValue(aPrice.a100_premium);
        ui->DoubleSpinBoxYandexPropane      ->setValue(aPrice.propane);
        ui->DoubleSpinBoxYandexMetan        ->setValue(aPrice.metan);
        break;
    }
    case c_cityMobileIndex: {
        ui->DoubleSpinBoxCityMobileDiesel       ->setValue(aPrice.diesel);
        ui->DoubleSpinBoxCityMobileDieselPremium->setValue(aPrice.diesel_premium);
        ui->DoubleSpinBoxCityMobileA80          ->setValue(aPrice.a80);
        ui->DoubleSpinBoxCityMobileA92          ->setValue(aPrice.a92);
        ui->DoubleSpinBoxCityMobileA92Premium   ->setValue(aPrice.a92_premium);
        ui->DoubleSpinBoxCityMobileA95          ->setValue(aPrice.a95);
        ui->DoubleSpinBoxCityMobileA95Premium   ->setValue(aPrice.a95_premium);
        ui->DoubleSpinBoxCityMobileA98          ->setValue(aPrice.a98);
        ui->DoubleSpinBoxCityMobileA98Premium   ->setValue(aPrice.a98_premium);
        ui->DoubleSpinBoxCityMobileA100         ->setValue(aPrice.a100);
        ui->DoubleSpinBoxCityMobileA100Premium  ->setValue(aPrice.a100_premium);
        ui->DoubleSpinBoxCityMobilePropane      ->setValue(aPrice.propane);
        ui->DoubleSpinBoxCityMobileMetan        ->setValue(aPrice.metan);
        break;
    }
    }
}

AgzsPrice FormAddPartnerAgzs::uiToPrice(int aIPartner) {
    AgzsPrice price;
    switch (aIPartner) {
    case c_yandexIndex: {
        price.diesel            = ui->DoubleSpinBoxYandexDiesel       ->value();
        price.diesel_premium    = ui->DoubleSpinBoxYandexDieselPremium->value();
        price.a80               = ui->DoubleSpinBoxYandexA80          ->value();
        price.a92               = ui->DoubleSpinBoxYandexA92          ->value();
        price.a92_premium       = ui->DoubleSpinBoxYandexA92Premium   ->value();
        price.a95               = ui->DoubleSpinBoxYandexA95          ->value();
        price.a95_premium       = ui->DoubleSpinBoxYandexA95Premium   ->value();
        price.a98               = ui->DoubleSpinBoxYandexA98          ->value();
        price.a98_premium       = ui->DoubleSpinBoxYandexA98Premium   ->value();
        price.a100              = ui->DoubleSpinBoxYandexA100         ->value();
        price.a100_premium      = ui->DoubleSpinBoxYandexA100Premium  ->value();
        price.propane           = ui->DoubleSpinBoxYandexPropane      ->value();
        price.metan             = ui->DoubleSpinBoxYandexMetan        ->value();
        price.iPartner = c_yandexIndex;
        price.partner = "ЯНДЕКС";
        break;
    }
    case c_cityMobileIndex: {
        price.diesel            = ui->DoubleSpinBoxCityMobileDiesel       ->value();
        price.diesel_premium    = ui->DoubleSpinBoxCityMobileDieselPremium->value();
        price.a80               = ui->DoubleSpinBoxCityMobileA80          ->value();
        price.a92               = ui->DoubleSpinBoxCityMobileA92          ->value();
        price.a92_premium       = ui->DoubleSpinBoxCityMobileA92Premium   ->value();
        price.a95               = ui->DoubleSpinBoxCityMobileA95          ->value();
        price.a95_premium       = ui->DoubleSpinBoxCityMobileA95Premium   ->value();
        price.a98               = ui->DoubleSpinBoxCityMobileA98          ->value();
        price.a98_premium       = ui->DoubleSpinBoxCityMobileA98Premium   ->value();
        price.a100              = ui->DoubleSpinBoxCityMobileA100         ->value();
        price.a100_premium      = ui->DoubleSpinBoxCityMobileA100Premium  ->value();
        price.propane           = ui->DoubleSpinBoxCityMobilePropane      ->value();
        price.metan             = ui->DoubleSpinBoxCityMobileMetan        ->value();
        price.iPartner = c_cityMobileIndex;
        price.partner = "СИТИМОБИЛ";
        break;
    }
    }
    return price;
}

void FormAddPartnerAgzs::dataToUi(AgzsData aData) {
    ui->LineEditDataName            ->setText(aData.agzsName);
    ui->LineEditDataNumber          ->setText(QString::number(aData.agzs));
    ui->LineEditDataAdress          ->setText(aData.adress);
    ui->DoubleSpinBoxDataLatitude   ->setValue(aData.latitude);
    ui->DoubleSpinBoxDataLongitude  ->setValue(aData.longitude);
    ui->SpinBoxDataColumns          ->setValue(aData.columnsCount);
    ui->LineEditDataL               ->setText(aData.AgzsL);
    ui->LineEditDataP               ->setText(aData.AgzsP);
    ui->LineEditDataCityMobileToken ->setText(aData.cityMobileToken);
    ui->LineEditDataYandexToken     ->setText(aData.yandexToken);
    ui->LineEditDataVcode           ->setText(QString::number(aData.vCode));
}

AgzsData FormAddPartnerAgzs::uiToData() {
    AgzsData data;
    data.agzsName           = ui->LineEditDataName              ->text();
    data.agzs               = ui->LineEditDataNumber            ->text().toInt();
    data.adress             = ui->LineEditDataAdress            ->text();
    data.latitude           = ui->DoubleSpinBoxDataLatitude     ->value();
    data.longitude          = ui->DoubleSpinBoxDataLongitude    ->value();
    data.columnsCount       = ui->SpinBoxDataColumns            ->value();
    data.AgzsL              = ui->LineEditDataL                 ->text();
    data.AgzsP              = ui->LineEditDataP                 ->text();
    data.cityMobileToken    = ui->LineEditDataCityMobileToken   ->text();
    data.yandexToken        = ui->LineEditDataYandexToken       ->text();
    data.vCode              = ui->LineEditDataVcode             ->text().toInt();
    return data;
}

void FormAddPartnerAgzs::columnsToUi(QList<AgzsColumn> aColumnsDb, QList<AgzsColumn> aColumnsAgzs) {
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsDb->model());
    int row = 0;
    if (model != nullptr) {
        model->setRowCount(0);
        for(auto &column: aColumnsDb) {
            QStandardItem *itemAgzs = new QStandardItem(QString::number(column.agzs));
            QStandardItem *itemVCode = new QStandardItem(QString::number(column.vCode));
            QStandardItem *itemSideAdress = new QStandardItem(QString::number(column.sideAdress));
            QStandardItem *itemPartnerNum = new QStandardItem(QString::number(column.partnerTrkNum));
            QStandardItem *itemVisibility = new QStandardItem(QString::number(column.visible));
            QStandardItem *itemSide = new QStandardItem(column.side);
            QStandardItem *itemFuel1 = new QStandardItem(QString::number(column.nozzle1Fuel));
            QStandardItem *itemFuel2 = new QStandardItem(QString::number(column.nozzle2Fuel));
            QStandardItem *itemFuel3 = new QStandardItem(QString::number(column.nozzle3Fuel));
            QStandardItem *itemFuel4 = new QStandardItem(QString::number(column.nozzle4Fuel));
            QStandardItem *itemFuel5 = new QStandardItem(QString::number(column.nozzle5Fuel));

            model->setItem(row, c_columnsColumnAgzs, itemAgzs);
            model->setItem(row, c_columnsColumnVcode, itemVCode);
            model->setItem(row, c_columnsColumnSideAdress, itemSideAdress);
            model->setItem(row, c_columnsColumnPartnerNum, itemPartnerNum);
            model->setItem(row, c_columnsColumnVisible, itemVisibility);
            model->setItem(row, c_columnsColumnSide, itemSide);
            model->setItem(row, c_columnsColumnFuel1, itemFuel1);
            model->setItem(row, c_columnsColumnFuel2, itemFuel2);
            model->setItem(row, c_columnsColumnFuel3, itemFuel3);
            model->setItem(row, c_columnsColumnFuel4, itemFuel4);
            model->setItem(row, c_columnsColumnFuel5, itemFuel5);
            ++row;
        }
        ui->TableViewColumnsDb->resizeRowsToContents();
    }
    model = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsAgzs->model());
    row = 0;
    if (model != nullptr) {
        model->setRowCount(0);
        for(auto &column: aColumnsAgzs) {
            QStandardItem *itemAgzs = new QStandardItem(QString::number(column.agzs));
            QStandardItem *itemVCode = new QStandardItem(QString::number(column.vCode));
            QStandardItem *itemSideAdress = new QStandardItem(QString::number(column.sideAdress));
            QStandardItem *itemPartnerNum = new QStandardItem(QString::number(column.partnerTrkNum));
            QStandardItem *itemVisibility = new QStandardItem(QString::number(column.visible));
            QStandardItem *itemSide = new QStandardItem(column.side);
            QStandardItem *itemFuel1 = new QStandardItem(QString::number(column.nozzle1Fuel));
            QStandardItem *itemFuel2 = new QStandardItem(QString::number(column.nozzle2Fuel));
            QStandardItem *itemFuel3 = new QStandardItem(QString::number(column.nozzle3Fuel));
            QStandardItem *itemFuel4 = new QStandardItem(QString::number(column.nozzle4Fuel));
            QStandardItem *itemFuel5 = new QStandardItem(QString::number(column.nozzle5Fuel));

            model->setItem(row, c_columnsColumnAgzs, itemAgzs);
            model->setItem(row, c_columnsColumnVcode, itemVCode);
            model->setItem(row, c_columnsColumnSideAdress, itemSideAdress);
            model->setItem(row, c_columnsColumnPartnerNum, itemPartnerNum);
            model->setItem(row, c_columnsColumnVisible, itemVisibility);
            model->setItem(row, c_columnsColumnSide, itemSide);
            model->setItem(row, c_columnsColumnFuel1, itemFuel1);
            model->setItem(row, c_columnsColumnFuel2, itemFuel2);
            model->setItem(row, c_columnsColumnFuel3, itemFuel3);
            model->setItem(row, c_columnsColumnFuel4, itemFuel4);
            model->setItem(row, c_columnsColumnFuel5, itemFuel5);
            ++row;
        }
        ui->TableViewColumnsAgzs->resizeRowsToContents();
    }
}

QList<AgzsColumn> FormAddPartnerAgzs::UiToColumns() {
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsDb->model());
    QList<AgzsColumn> columns;
    if (model != nullptr) {
        for (int row = 0; row < model->rowCount(); ++row) {
            AgzsColumn column;
            column.agzs = model->item(row, c_columnsColumnAgzs)->text().toInt();
            column.vCode = model->item(row, c_columnsColumnVcode)->text().toInt();
            column.sideAdress = model->item(row, c_columnsColumnSideAdress)->text().toInt();
            column.partnerTrkNum = model->item(row, c_columnsColumnPartnerNum)->text().toInt();
            column.visible = model->item(row, c_columnsColumnVisible)->text().toInt();
            column.side = model->item(row, c_columnsColumnSide)->text();
            column.nozzle1Fuel = model->item(row, c_columnsColumnFuel1)->text().toInt();
            column.nozzle2Fuel = model->item(row, c_columnsColumnFuel2)->text().toInt();
            column.nozzle3Fuel = model->item(row, c_columnsColumnFuel3)->text().toInt();
            column.nozzle4Fuel = model->item(row, c_columnsColumnFuel4)->text().toInt();
            column.nozzle5Fuel = model->item(row, c_columnsColumnFuel5)->text().toInt();
            columns.append(column);
        }
    }
    return columns;
}
#define UiEnd }

#define AgzsDataStart {
void FormAddPartnerAgzs::on_PushButtonAcceptAgzsData_clicked() {
    AgzsData data = uiToData();
    if (ui->LineEditDataVcode->text() == ""
        || ui->LineEditDataVcode->text().toInt() == 0) {
        data.vCode = DataBase::getLxKeyCode("PR_AGZSData", true);
        data.id = QString::number(data.vCode);
        data.cDate = QDateTime::currentDateTime();
        if (DataBase::insertAgzsData(data)) {
            QMessageBox::information(this, tr("Успешно!"), tr("Данные были добавлены!"));
        }
    } else {
        data.id = QString::number(data.vCode);
        if (DataBase::updateAgzsData(data)) {
            QMessageBox::information(this, tr("Успешно!"), tr("Данные были обновлены!"));
        }
    }
}

void FormAddPartnerAgzs::on_ButtonRegisterCityMobile_clicked() {
    RequestData *request = new RequestData();
    qDebug() << "https://terminal.api.fuelup.ru/api/auth?login=" + ui->lineEditUUID->text();
    request->get(QNetworkRequest(QUrl("https://terminal.api.fuelup.ru/api/auth?login=" + ui->lineEditUUID->text())), false);
    int statusCode = request->getCode();
    if (statusCode == 200) {
        request->post(QNetworkRequest(QUrl("https://terminal.api.fuelup.ru/api/auth?login=" + ui->lineEditUUID->text() + "&code=" + ui->lineEditAPI_KEY->text())), QString(), false);
        statusCode = request->getCode();
        qDebug() << request->getAuthorization();
        if (statusCode == 200) {
            ui->lineEditResult->setText(QString::fromUtf8(request->getAuthorization()));
        } else {
            ui->lineEditResult->setText("Ошибка пароля " + QString::number(statusCode));
            QMessageBox::warning(this, "Ошибка!", "Не правильно указан api_key (код: " + QString::number(statusCode) + ")");
        }
    } else {
        ui->lineEditResult->setText("Ошибка логина " + QString::number(statusCode));
        QMessageBox::warning(this, "Ошибка!", "Не найден uuid (код: " + QString::number(statusCode) + ")");
    }
    delete request;
}
#define AgzsDataEnd }

#define AgzsColumnsStart {
void FormAddPartnerAgzs::on_PushButtonAcceptAgzsColumns_clicked() {
    QList<AgzsColumn> columnsToDb = UiToColumns();
    QList<AgzsColumn> columnsDb = DataBase::getColumnsDb(ui->ComboBoxAgzs->currentText());
    int in = 0, up = 0, de = 0;
    int ine = 0, upe = 0, dee = 0;
    for (auto &columnDb: columnsDb) {
        auto iterator = std::find_if(columnsToDb.begin(),
                                     columnsToDb.end(),
                                     [=](AgzsColumn columnToDb) {
                                         return columnToDb.vCode == columnDb.vCode;
                                     });
        if (iterator != columnsToDb.end()) {
            if (DataBase::updateAgzsColumn(*iterator)) {
                ++up;
            } else {
                ++upe;
            }
        } else {
            if (DataBase::deleteAgzsColumn(columnDb.vCode)) {
                ++de;
            } else {
                ++dee;
            }
        }
    }
    for (auto &column: columnsToDb) {
        if (column.vCode == 0) {
            column.vCode = DataBase::getLxKeyCode("PR_Columns", true);
            column.cDate = QDateTime::currentDateTime();
            if (DataBase::insertAgzsColumn(column)) {
                ++in;
            } else {
                ++ine;
            }
        }
    }
    if (in <= 0 && up <= 0 && de <= 0 && ine <= 0 && upe <= 0 && dee <= 0) {
        return;
    }
    QStringList message;
    if (in > 0) {
        message.append(tr("Добавлено %1 колонок").arg(in));
    }
    if (up > 0) {
        message.append(tr("Обновлено %1 колонок").arg(up));
    }
    if (de > 0) {
        message.append(tr("Удалено %1 колонок").arg(de));
    }
    if (ine > 0) {
        message.append(tr("Не удалось добавить %1 колонок").arg(ine));
    }
    if (upe > 0) {
        message.append(tr("Не удалось обновить %1 колонок").arg(upe));
    }
    if (dee > 0) {
        message.append(tr("Не удалось удалить %1 колонок").arg(dee));
    }
    QMessageBox::information(this, tr("Внимание!"), message.join("\n"));
}

void FormAddPartnerAgzs::on_PushButtonAddColumn_clicked() {
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsDb->model());
    if (model) {
        int rowsCount = model->rowCount();
        model->insertRow(rowsCount);
        model->setItem(rowsCount, c_columnsColumnVcode, new QStandardItem("0"));
        ui->TableViewColumnsDb->resizeRowToContents(rowsCount);
    }
}

void FormAddPartnerAgzs::on_PushButtonDeleteRow_clicked() {
    int rowNumber = ui->SpinBoxRowNumber->value();
    if (ui->TableViewColumnsDb->model()->rowCount() > rowNumber) {
        ui->TableViewColumnsDb->model()->removeRow(rowNumber);
    }
}

void FormAddPartnerAgzs::on_PushButtonReplaceColumns_clicked() {
    QStandardItemModel *modelDb = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsDb->model());
    QStandardItemModel *modelAgzs = dynamic_cast<QStandardItemModel*>(ui->TableViewColumnsAgzs->model());
    if (modelDb != nullptr
        && modelAgzs != nullptr) {
        modelDb->setRowCount(0);
        for (int row = 0; row < modelAgzs->rowCount(); ++row) {
            for (int col = 0; col < modelAgzs->columnCount(); ++col) {
                modelDb->setItem(row, col, new QStandardItem(modelAgzs->item(row, col)->text()));
            }
        }
    }
}
#define AgzsColumnsEnd }

#define AgzsPriceStart {
void FormAddPartnerAgzs::on_PushButtonCopyYandexToCityMobile_clicked() {
    ui->DoubleSpinBoxCityMobileDiesel       ->setValue(ui->DoubleSpinBoxYandexDiesel        ->value());
    ui->DoubleSpinBoxCityMobileDieselPremium->setValue(ui->DoubleSpinBoxYandexDieselPremium ->value());
    ui->DoubleSpinBoxCityMobileA80          ->setValue(ui->DoubleSpinBoxYandexA80           ->value());
    ui->DoubleSpinBoxCityMobileA92          ->setValue(ui->DoubleSpinBoxYandexA92           ->value());
    ui->DoubleSpinBoxCityMobileA92Premium   ->setValue(ui->DoubleSpinBoxYandexA92Premium    ->value());
    ui->DoubleSpinBoxCityMobileA95          ->setValue(ui->DoubleSpinBoxYandexA95           ->value());
    ui->DoubleSpinBoxCityMobileA95Premium   ->setValue(ui->DoubleSpinBoxYandexA95Premium    ->value());
    ui->DoubleSpinBoxCityMobileA98          ->setValue(ui->DoubleSpinBoxYandexA98           ->value());
    ui->DoubleSpinBoxCityMobileA98Premium   ->setValue(ui->DoubleSpinBoxYandexA98Premium    ->value());
    ui->DoubleSpinBoxCityMobileA100         ->setValue(ui->DoubleSpinBoxYandexA100          ->value());
    ui->DoubleSpinBoxCityMobileA100Premium  ->setValue(ui->DoubleSpinBoxYandexA100Premium   ->value());
    ui->DoubleSpinBoxCityMobilePropane      ->setValue(ui->DoubleSpinBoxYandexPropane       ->value());
    ui->DoubleSpinBoxCityMobileMetan        ->setValue(ui->DoubleSpinBoxYandexMetan         ->value());
}

void FormAddPartnerAgzs::on_PushButtonCopyCityMobileToYandex_clicked() {
    ui->DoubleSpinBoxYandexDiesel       ->setValue(ui->DoubleSpinBoxCityMobileDiesel        ->value());
    ui->DoubleSpinBoxYandexDieselPremium->setValue(ui->DoubleSpinBoxCityMobileDieselPremium ->value());
    ui->DoubleSpinBoxYandexA80          ->setValue(ui->DoubleSpinBoxCityMobileA80           ->value());
    ui->DoubleSpinBoxYandexA92          ->setValue(ui->DoubleSpinBoxCityMobileA92           ->value());
    ui->DoubleSpinBoxYandexA92Premium   ->setValue(ui->DoubleSpinBoxCityMobileA92Premium    ->value());
    ui->DoubleSpinBoxYandexA95          ->setValue(ui->DoubleSpinBoxCityMobileA95           ->value());
    ui->DoubleSpinBoxYandexA95Premium   ->setValue(ui->DoubleSpinBoxCityMobileA95Premium    ->value());
    ui->DoubleSpinBoxYandexA98          ->setValue(ui->DoubleSpinBoxCityMobileA98           ->value());
    ui->DoubleSpinBoxYandexA98Premium   ->setValue(ui->DoubleSpinBoxCityMobileA98Premium    ->value());
    ui->DoubleSpinBoxYandexA100         ->setValue(ui->DoubleSpinBoxCityMobileA100          ->value());
    ui->DoubleSpinBoxYandexA100Premium  ->setValue(ui->DoubleSpinBoxCityMobileA100Premium   ->value());
    ui->DoubleSpinBoxYandexPropane      ->setValue(ui->DoubleSpinBoxCityMobilePropane       ->value());
    ui->DoubleSpinBoxYandexMetan        ->setValue(ui->DoubleSpinBoxCityMobileMetan         ->value());
}

void FormAddPartnerAgzs::on_PushButtonAcceptPriceYandex_clicked() {
    AgzsPrice priceYandex = uiToPrice(c_yandexIndex);
    priceYandex.agzsName = ui->LineEditDataName->text();
    priceYandex.agzs = ui->LineEditDataNumber->text().toInt();
    priceYandex.link = ui->LineEditDataVcode->text().toInt();
    if (DataBase::updateLastAgzsPrice(priceYandex)) {
        QMessageBox::information(this, tr("Успешно!"), tr("Данные цен по Яндексу успешно обновлены!"));
    } else {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Данные цен по Яндексу не обновлены!"));
    }
}

void FormAddPartnerAgzs::on_PushButtonAcceptPriceCityMobile_clicked() {
    AgzsPrice priceCityMobile = uiToPrice(c_cityMobileIndex);
    priceCityMobile.agzsName = ui->LineEditDataName->text();
    priceCityMobile.agzs = ui->LineEditDataNumber->text().toInt();
    priceCityMobile.link = ui->LineEditDataVcode->text().toInt();
    if (DataBase::updateLastAgzsPrice(priceCityMobile)) {
        QMessageBox::information(this, tr("Успешно!"), tr("Данные цен по Ситимобилу успешно обновлены!"));
    } else {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Данные цен по Ситимобилу не обновлены!"));
    }
}

void FormAddPartnerAgzs::on_PushButtonAcceptPrice_clicked() {
    on_PushButtonAcceptPriceYandex_clicked();
    on_PushButtonAcceptPriceCityMobile_clicked();
}

void FormAddPartnerAgzs::on_PushButtonAcceptPriceFromDateYandex_clicked() {
    AgzsPrice priceYandex = uiToPrice(c_yandexIndex);
    priceYandex.agzsName = ui->LineEditDataName->text();
    priceYandex.agzs = ui->LineEditDataNumber->text().toInt();
    priceYandex.link = ui->LineEditDataVcode->text().toInt();
    priceYandex.cDate = QDateTime::currentDateTime();
    priceYandex.dateStart = ui->DateTimeEditPriceUpdateTime->dateTime();
    priceYandex.vCode = DataBase::getLxKeyCode("PR_AGZSPrice", true);
    if (DataBase::insertAgzsPrice(priceYandex)) {
        QMessageBox::information(this, tr("Успешно!"), tr("Данные цен по Яндексу успешно добавлены!"));
    } else {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Данные цен по Яндексу не добавлены!"));
    }
}

void FormAddPartnerAgzs::on_PushButtonAcceptPriceFromDateCityMobile_clicked() {
    AgzsPrice priceCityMobile = uiToPrice(c_cityMobileIndex);
    priceCityMobile.agzsName = ui->LineEditDataName->text();
    priceCityMobile.agzs = ui->LineEditDataNumber->text().toInt();
    priceCityMobile.link = ui->LineEditDataVcode->text().toInt();
    priceCityMobile.cDate = QDateTime::currentDateTime();
    priceCityMobile.dateStart = ui->DateTimeEditPriceUpdateTime->dateTime();
    priceCityMobile.vCode = DataBase::getLxKeyCode("PR_AGZSPrice", true);
    if (DataBase::insertAgzsPrice(priceCityMobile)) {
        QMessageBox::information(this, tr("Успешно!"), tr("Данные цен по Ситимобилу успешно добавлены!"));
    } else {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Данные цен по Ситимобилу не добавлены!"));
    }
}

void FormAddPartnerAgzs::on_PushButtonAcceptPriceFromDate_clicked() {
    on_PushButtonAcceptPriceFromDateYandex_clicked();
    on_PushButtonAcceptPriceFromDateCityMobile_clicked();
}
#define AgzsPriceEnd }

void FormAddPartnerAgzs::on_PushButtonFindColumn_clicked() {
    const QString agzs = ui->ComboBoxAgzs->currentText();
    columnsToUi(DataBase::getColumnsDb(agzs), DataBase::getColumnsAdast(agzs,
                                                    ui->LineEditDataP->text(),
                                                    ui->LineEditDataNumber->text().toInt()));
}
