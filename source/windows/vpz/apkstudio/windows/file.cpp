#include "file.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Windows {

File::File(const QString &device, const QString &path, QWidget *parent) :
    Dialog(parent), device(device), path(path)
{
    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QLayout *horizontal = new QHBoxLayout;
    QVBoxLayout *vertical = new QVBoxLayout;
    QGroupBox *meta = new QGroupBox(translate("group_meta"), this);
    QGroupBox *permissions = new QGroupBox(translate("group_permissions"), this);
    QFormLayout *layoutfs = new QFormLayout;
    QFormLayout *layoutpermissions = new QFormLayout(permissions);
    QFormLayout *layoutmeta = new QFormLayout(meta);
    QLineEdit *egroup = new QLineEdit(this);
    QLineEdit *ename = new QLineEdit(this);
    QLineEdit *eowner = new QLineEdit(this);
    QLineEdit *epath = new QLineEdit(path, this);
    QLineEdit *epermissions = new QLineEdit(this);
    QLineEdit *esize = new QLineEdit(this);
    QLineEdit *esymlink = new QLineEdit(this);
    QLineEdit *etimestamp = new QLineEdit(this);
    buttons->addButton(translate("button_close"), QDialogButtonBox::AcceptRole);
    connections.append(connect(buttons, SIGNAL(accepted()), this, SLOT(accept())));
    connections.append(connect(this, &File::detailsReceived, [ egroup, ename, eowner, epath, epermissions, esize, esymlink, etimestamp ] (QMap<QString, QString> map) {
        egroup->setText(map["group"]);
        ename->setText(map["name"]);
        eowner->setText(map["owner"]);
        epermissions->setText(map["permissions"]);
        esize->setText(map["size"]);
        esymlink->setText(map["link"]);
        etimestamp->setText(map["timestamp"]);
    }));
    egroup->setReadOnly(true);
    ename->setReadOnly(true);
    eowner->setReadOnly(true);
    epath->setReadOnly(true);
    epermissions->setReadOnly(true);
    esize->setReadOnly(true);
    esymlink->setReadOnly(true);
    etimestamp->setReadOnly(true);
    horizontal->addWidget(meta);
    horizontal->addWidget(permissions);
    layoutfs->addRow(translate("label_name"), ename);
    layoutfs->addRow(translate("label_path"), epath);
    layoutmeta->addRow(translate("label_symlink"), esymlink);
    layoutmeta->addRow(translate("label_size"), esize);
    layoutmeta->addRow(translate("label_timestamp"), etimestamp);
    layoutpermissions->addRow(translate("label_permissions"), epermissions);
    layoutpermissions->addRow(translate("label_owner"), eowner);
    layoutpermissions->addRow(translate("label_group"), egroup);
    meta->setLayout(layoutmeta);
    permissions->setLayout(layoutpermissions);
    vertical->addLayout(layoutfs);
    vertical->addLayout(horizontal);
    vertical->addWidget(buttons);
    setLayout(vertical);
    setMinimumSize(320, 160);
    setWindowIcon(::icon("file"));
    setWindowTitle(translate("title_window").arg(path));
}

void File::onInitComplete()
{
    emit detailsReceived(ADB::instance()->details(device, ADB::DETAILS_FILE, path));
}

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ
