#include "application.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Windows {

Application::Application(const QString &device, const QString &package, QWidget *parent) :
    Dialog(parent), device(device), package(package)
{
    QLayout *horizontal = new QHBoxLayout;
    QVBoxLayout *vertical = new QVBoxLayout;
    QGroupBox *fs = new QGroupBox(translate("group_fs"), this);
    QGroupBox *meta = new QGroupBox(translate("group_meta"), this);
    QGroupBox *status = new QGroupBox(translate("group_status"), this);
    QFormLayout *layoutfs = new QFormLayout(fs);
    QFormLayout *layoutmeta = new QFormLayout(meta);
    QFormLayout *layoutstatus = new QFormLayout(status);
    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QLineEdit *eapk = new QLineEdit(this);
    QLineEdit *edata = new QLineEdit(this);
    QLineEdit *eenabled = new QLineEdit(this);
    QLineEdit *einstaller = new QLineEdit(this);
    QLineEdit *ename = new QLineEdit(this);
    QLineEdit *epackage = new QLineEdit(package, this);
    QLineEdit *erunning = new QLineEdit(this);
    QLineEdit *esize = new QLineEdit(this);
    QLineEdit *esystem = new QLineEdit(this);
    QLineEdit *eupdate = new QLineEdit(this);
    QLineEdit *evcode = new QLineEdit(this);
    QLineEdit *evname = new QLineEdit(this);
    buttons->addButton(translate("button_close"), QDialogButtonBox::AcceptRole);
    connections.append(connect(buttons, SIGNAL(accepted()), this, SLOT(accept())));
    connections.append(connect(this, &Application::detailsReceived, [ eapk, edata, eenabled, einstaller, ename, epackage, erunning, esize, esystem, eupdate, evcode, evname ] (QMap<QString, QString> map) {
        eapk->setText(map["codePath"]);
        edata->setText(map["dataDir"]);
        eenabled->setText((QString::compare(map["enabled"], "2") == 0) ? "No" : "Yes");
        einstaller->setText(map["installerPackageName"]);
        ename->setText(map["name"]);
        erunning->setText((QString::compare(map["stopped"], "false") == 0) ? "Yes" : "No");
        esize->setText(map["size"]);
        esystem->setText(map["flags"].contains(" SYSTEM ") ? "Yes" : "No");
        eupdate->setText(map["lastUpdateTime"]);
        evcode->setText(map["versionCode"]);
        evname->setText(map["versionName"]);
    }));
    eapk->setReadOnly(true);
    edata->setReadOnly(true);
    eenabled->setReadOnly(true);
    einstaller->setReadOnly(true);
    ename->setReadOnly(true);
    epackage->setReadOnly(true);
    erunning->setReadOnly(true);
    esize->setReadOnly(true);
    esystem->setReadOnly(true);
    eupdate->setReadOnly(true);
    evcode->setReadOnly(true);
    evname->setReadOnly(true);
    fs->setLayout(layoutfs);
    horizontal->addWidget(meta);
    horizontal->addWidget(status);
    layoutfs->addRow(translate("label_name"), ename);
    layoutfs->addRow(translate("label_apk"), eapk);
    layoutfs->addRow(translate("label_data"), edata);
    layoutfs->addRow(translate("label_size"), esize);
    layoutmeta->addRow(translate("label_package"), epackage);
    layoutmeta->addRow(translate("label_installer"), einstaller);
    layoutmeta->addRow(translate("label_vcode"), evcode);
    layoutmeta->addRow(translate("label_vname"), evname);
    layoutstatus->addRow(translate("label_running"), erunning);
    layoutstatus->addRow(translate("label_enabled"), eenabled);
    layoutstatus->addRow(translate("label_system"), esystem);
    layoutstatus->addRow(translate("label_update"), eupdate);
    meta->setLayout(layoutmeta);
    status->setLayout(layoutstatus);
    vertical->addLayout(horizontal);
    vertical->addWidget(fs);
    vertical->addWidget(buttons);
    setLayout(vertical);
    setWindowIcon(::icon("apk"));
    setWindowTitle(translate("title_window").arg(package));
}

void Application::onInitComplete()
{
    emit detailsReceived(ADB::instance()->details(device, ADB::DETAILS_APPLICATION, package));
}

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ
