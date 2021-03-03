#include "formbadtransactions.h"
#include "ui_formbadtransactions.h"

FormBadTransactions::FormBadTransactions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBadTransactions)
{
    ui->setupUi(this);
    init();
}

FormBadTransactions::~FormBadTransactions() {
    delete ui;
}

void FormBadTransactions::init() {
    QDate currentDate = QDate::currentDate();
    QDate monthStart(currentDate.year(), currentDate.month(), 1);
    QDate monthEnd = monthStart.addMonths(1);
    ui->DateTimeEditStart->setDateTime(QDateTime(monthStart));
    ui->DateTimeEditEnd->setDateTime(QDateTime(monthEnd));

    ui->TableViewTransactions->setModel(new QStandardItemModel());
    connect(ui->TableViewTransactions, &QTableView::customContextMenuRequested, this, [=](QPoint pos) {
        QMenu *menu = createMenu();
        menu->popup(ui->TableViewTransactions->viewport()->mapToGlobal(pos));
    });
}

QMenu *FormBadTransactions::createMenu() {
    QMenu *menu = new QMenu(this);

    QAction *goToTransaction = new QAction(tr("Посмотреть транзакции"), this);
    //actionGoToProfile->setIcon(QIcon(Images::goTo()));

    //QAction *actionFavorites = new QAction(this);
    //updateActionFavoriteData(actionFavorites, isProfileFavorite(aProfile));

    connect (goToTransaction, &QAction::triggered, this, &FormBadTransactions::goToTransaction);
    //connect (actionFavorites,   &QAction::triggered, this, &FormBadTransactions::friendToFavorite);

    menu -> addAction (goToTransaction);
    //menu -> addAction (actionFavorites);
    return menu;
}

void FormBadTransactions::goToTransaction() {
    auto index = ui->TableViewTransactions->currentIndex();
    index.row();
}

void FormBadTransactions::on_PushButtonConnect_clicked() {
    login_ = ui->LineEditLogin->text();
    password_ = ui->LineEditPassword->text();
    if (DataBase::connectDB(QSqlDatabase(), login_, password_)) {
        ui->LabelStatus->setText(tr("Подключено к базе %1").arg(ui->LineEditLogin->text()));
        ui->LabelStatus->setStyleSheet("color: rgb(10,200,10);");
        findAgzs(QDateTime(QDate(QDate::currentDate().year(), 1, 1)), QDateTime::currentDateTime());
    } else {
        ui->LabelStatus->setText(tr("Не удалось подключиться к базе %1").arg(ui->LineEditLogin->text()));
        ui->LabelStatus->setStyleSheet("color: rgb(200,10,10);");
    }
}

void FormBadTransactions::findAgzs(QDateTime dateStart, QDateTime dateEnd) {
    auto agzss = DataBase::getAgzs(dateStart, dateEnd);
    std::sort( agzss.begin(), agzss.end() );
    agzss.erase( std::unique(agzss.begin(), agzss.end() ), agzss.end() );

    if (ui->ScrollAreaAgzss->widget() == nullptr) {
        ui->ScrollAreaAgzss->setWidget(new QWidget());
    }
    if (ui->ScrollAreaAgzss->widget()->layout() == nullptr) {
        ui->ScrollAreaAgzss->widget()->setLayout(new QVBoxLayout());
    }
    QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>(ui->ScrollAreaAgzss->widget()->layout());
    if (layout != nullptr) {
        for(auto agzs: agzss) {
            QCheckBox *chb = new QCheckBox(agzs.engName);
            //qDebug() << agzs.number;
            chb->setObjectName("CheckBox_" + QString::number(agzs.number));
            connect(chb, &QCheckBox::stateChanged, this, &FormBadTransactions::checkBoxAgzs_stateChanged);
            layout->addWidget(chb);
        }
    }
}

void FormBadTransactions::checkBoxAgzs_stateChanged(int arg1) {
    bool isAnyChecked = arg1 == 2;
    bool isAnyUnchecked = arg1 == 0;
    for (int i = 0; i < ui->ScrollAreaAgzss->widget()->layout()->count(); ++i) {
        QCheckBox *chb = dynamic_cast<QCheckBox*>(ui->ScrollAreaAgzss->widget()->layout()->itemAt(i)->widget());
        if (chb) {
            if (!isAnyChecked) {
                isAnyChecked = chb->isChecked();
            }
            if (!isAnyUnchecked) {
                isAnyUnchecked = !chb->isChecked();
            }
            if (isAnyChecked && isAnyUnchecked) {
                break;
            }
        }
    }
    ui->CheckBoxCheckAll->blockSignals(true);
    if (isAnyChecked) {
        if (isAnyUnchecked) {
            ui->CheckBoxCheckAll->setCheckState(Qt::CheckState::PartiallyChecked);
        } else {
            ui->CheckBoxCheckAll->setCheckState(Qt::CheckState::Checked);
        }
    } else {
        if (isAnyUnchecked) {
            ui->CheckBoxCheckAll->setCheckState(Qt::CheckState::Unchecked);
        } else {
            ui->CheckBoxCheckAll->setCheckState(Qt::CheckState::PartiallyChecked);
        }
    }
    ui->CheckBoxCheckAll->blockSignals(false);
}

void FormBadTransactions::on_CheckBoxCheckAll_stateChanged(int arg1) {
    bool checked;
    switch (arg1) {
    case 0: {
        checked = false;
        break;
    }
    case 1: {
        ui->CheckBoxCheckAll->setCheckState(Qt::CheckState::Checked);
        return;
    }
    case 2: {
        checked = true;
        break;
    }
    }
    if (ui->ScrollAreaAgzss->widget() == nullptr) {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Не найден виджет!"));
        return;
    }
    if (ui->ScrollAreaAgzss->widget()->layout() == nullptr) {
        QMessageBox::warning(this, tr("Ошибка!"), tr("Не найден лейаут!"));
        return;
    }
    for (int i = 0; i < ui->ScrollAreaAgzss->widget()->layout()->count(); ++i) {
        QCheckBox *chb = dynamic_cast<QCheckBox*>(ui->ScrollAreaAgzss->widget()->layout()->itemAt(i)->widget());
        if (chb) {
            chb->blockSignals(true);
            chb->setChecked(checked);
            chb->blockSignals(false);
        }
    }
}

void FormBadTransactions::on_PushButtonFind_clicked() {
    QDateTime datestart = ui->DateTimeEditStart->dateTime();
    QDateTime dateend = ui->DateTimeEditEnd->dateTime();
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->TableViewTransactions->model());
    if (model == nullptr) {
        qWarning() << "on_PushButtonFind_clicked error tableModel";
        QMessageBox::warning(this, tr("Ошибка!"), tr("Не удалось найти модель таблицы."));
        return;
    }
    model->clear();
    for (int i = 0; i < ui->ScrollAreaAgzss->widget()->layout()->count(); ++i) {
        QCheckBox *chb = dynamic_cast<QCheckBox*>(ui->ScrollAreaAgzss->widget()->layout()->itemAt(i)->widget());
        if (chb) {
            int agzs = chb->objectName().remove("CheckBox_").toInt();
            auto transactions = DataBase::getBadTransaction(agzs, datestart, dateend, 14);
            qDebug() << transactions;
            if (transactions.count() > 0) {
                for (int row = 0; row < transactions.count(); ++row) {
                    QStandardItem *item = new QStandardItem(QString::number(agzs));
                    //something
                    model->setItem(row, 0, item);
                    for (int column = 0; column < transactions[row].count(); ++column) {
                        QStandardItem *item = new QStandardItem(transactions[row][column].toString());
                        //something
                        model->setItem(column, row + 1, item);
                    }
                }
            }
        }
    }
}
