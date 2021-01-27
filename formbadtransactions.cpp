#include "formbadtransactions.h"
#include "ui_formbadtransactions.h"

FormBadTransactions::FormBadTransactions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBadTransactions)
{
    ui->setupUi(this);
}

FormBadTransactions::~FormBadTransactions() {
    delete ui;
}

void FormBadTransactions::on_PushButtonConnect_clicked() {
    login_ = ui->LineEditLogin->text();
    password_ = ui->LineEditPassword->text();
    if (DataBase::connectDB(QSqlDatabase(), login_, password_)) {
        ui->LabelStatus->setText(tr("Подключено к базе %1").arg(ui->LineEditLogin->text()));
        ui->LabelStatus->setStyleSheet("color: rgb(10,200,10);");
    } else {
        ui->LabelStatus->setText(tr("Не удалось подключиться к базе %1").arg(ui->LineEditLogin->text()));
        ui->LabelStatus->setStyleSheet("color: rgb(200,10,10);");
    }
}

void FormBadTransactions::on_PushButtonFindAgzs_clicked() {
    auto agzss = DataBase::getAgzs(ui->DateTimeEditStart->dateTime(), ui->DateTimeEditEnd->dateTime());
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
            chb->setObjectName("CheckBox_" + agzs.engName);
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
