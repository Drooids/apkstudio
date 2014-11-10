#include "devices.hpp"

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Components {

Devices::Devices(QWidget *parent) :
    Dockable(translate("title_dock"), parent)
{
    QWidget *widget = new QWidget(this);
    QToolButton *adb = new QToolButton(widget);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    QToolButton *reboot = new QToolButton(widget);
    QToolBar *tool_bar = new QToolBar(widget);
    QAction *bootloader = new QAction(translate("item_bootloader"), tool_bar);
    QAction *download = new QAction(translate("item_download"), tool_bar);
    QAction *explore = new QAction(icon("folder_stand"), translate("item_explore"), tool_bar);
    QAction *information = new QAction(icon("processor"), translate("item_information"), tool_bar);
    QAction *kill = new QAction(translate("item_kill"), tool_bar);
    QAction *logcat = new QAction(icon("resource_monitor"), translate("item_logcat"), tool_bar);
    QAction *normal = new QAction(translate("item_normal"), tool_bar);
    QAction *recovery = new QAction(translate("item_recovery"), tool_bar);
    QAction *refresh = new QAction(icon("arrow_circle_double"), translate("item_refresh"), tool_bar);
    QAction *restart = new QAction(translate("item_restart"), tool_bar);
    QAction *safemode = new QAction(translate("item_safemode"), tool_bar);
    QAction *screenshot = new QAction(icon("camera"), translate("item_screenshot"), tool_bar);
    QAction *shell = new QAction(icon("terminal"), translate("item_shell"), tool_bar);
    QAction *shutdown = new QAction(translate("item_shutdown"), tool_bar);
    QAction *soft = new QAction(translate("item_soft"), tool_bar);
    QAction *start = new QAction(translate("item_start"), tool_bar);
    QAction *zygote = new QAction(translate("item_zygote"), tool_bar);
    QMenu *menu_adb = new QMenu(adb);
    QMenu *menu_reboot = new QMenu(reboot);
    tree = new QTreeWidget(widget);
    adb->setDefaultAction(restart);
    adb->setIcon(icon("control_power"));
    adb->setText(translate("item_adb"));
    adb->setToolTip(translate("tooltip_adb"));
    adb->setMenu(menu_adb);
    adb->setPopupMode(QToolButton::MenuButtonPopup);
    explore->setEnabled(false);
    information->setEnabled(false);
    layout->addWidget(tool_bar);
    layout->addWidget(tree);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setSpacing(0);
    logcat->setEnabled(false);
    menu_adb->addAction(start);
    menu_adb->addAction(kill);
    menu_adb->addSeparator();
    menu_adb->addAction(restart);
    menu_reboot->addAction(normal);
    menu_reboot->addAction(bootloader);
    menu_reboot->addAction(download);
    menu_reboot->addAction(recovery);
    menu_reboot->addAction(safemode);
    menu_reboot->addAction(shutdown);
    menu_reboot->addAction(soft);
    menu_reboot->addAction(zygote);
    reboot->setDefaultAction(normal);
    reboot->setIcon(icon("mobile_phone_restart"));
    reboot->setText(translate("item_reboot"));
    reboot->setToolTip(translate("tooltip_reboot"));
    reboot->setMenu(menu_reboot);
    reboot->setPopupMode(QToolButton::MenuButtonPopup);
    reboot->setEnabled(false);
    screenshot->setEnabled(false);
    shell->setEnabled(false);
    tool_bar->addWidget(adb);
    tool_bar->addAction(refresh);
    tool_bar->addSeparator();
    tool_bar->addWidget(reboot);
    tool_bar->addAction(information);
    tool_bar->addSeparator();
    tool_bar->addAction(explore);
    tool_bar->addAction(screenshot);
    tool_bar->addAction(logcat);
    tool_bar->addAction(shell);
    QStringList labels;
    labels << "";
    labels << translate("header_serial");
    labels << translate("header_type");
    tree->setHeaderLabels(labels);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setRootIsDecorated(false);
    tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setColumnWidth(0, 32);
    tree->setSortingEnabled(true);
    tree->sortByColumn(1, Qt::AscendingOrder);
    connections.append(connect(bootloader, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_BOOTLOADER);
    }));
    connections.append(connect(download, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_DOWNLOAD);
    }));
    connections.append(connect(explore, SIGNAL(triggered()), this, SLOT(onExplore())));
    connections.append(connect(information, SIGNAL(triggered()), this, SLOT(onInformation())));
    connections.append(connect(kill, &QAction::triggered, [ ] () {
        ADB::instance()->kill();
    }));
    connections.append(connect(logcat, SIGNAL(triggered()), this, SLOT(onLogcat())));
    connections.append(connect(normal, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_NORMAL);
    }));
    connections.append(connect(recovery, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_RECOVERY);
    }));
    connections.append(connect(refresh, SIGNAL(triggered()), this, SLOT(onRefresh())));
    connections.append(connect(restart, &QAction::triggered, [ ] () {
        ADB::instance()->kill();
        ADB::instance()->start();
    }));
    connections.append(connect(safemode, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_SAFEMODE);
    }));
    connections.append(connect(screenshot, SIGNAL(triggered()), this, SLOT(onScreenshot())));
    connections.append(connect(shell, SIGNAL(triggered()), this, SLOT(onShell())));
    connections.append(connect(shutdown, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_SHUTDOWN);
    }));
    connections.append(connect(soft, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_SOFT);
    }));
    connections.append(connect(zygote, &QAction::triggered, [ this ] () {
        Device device = this->selected();
        if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
            return;
        ADB::instance()->reboot(device.serial, ADB::REBOOT_ZYGOTE);
    }));
    connections.append(connect(start, &QAction::triggered, [ ] () {
        ADB::instance()->start();
    }));
    connections.append(connect(this, SIGNAL(showExplorer(QString)), this->parent(), SLOT(onShowExplorer(QString))));
    connections.append(connect(this, SIGNAL(showInformation(QString)), this->parent(), SLOT(onShowInformation(QString))));
    connections.append(connect(this, SIGNAL(showLogcat(QString)), this->parent(), SLOT(onShowLogcat(QString))));
    connections.append(connect(tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick(QModelIndex))));
    connections.append(connect(tree->selectionModel(), &QItemSelectionModel::selectionChanged, [ explore, information, logcat, reboot, screenshot, shell, this ] (const QItemSelection &current, const QItemSelection &previous) {
        Q_UNUSED(current)
        Q_UNUSED(previous)
        bool enable = false;
        Device device = this->selected();
        if (!device.serial.isEmpty())
            enable = (device.status == Device::ONLINE);
        explore->setEnabled(enable);
        information->setEnabled(enable);
        logcat->setEnabled(enable);
        reboot->setEnabled(enable);
        screenshot->setEnabled(enable);
        shell->setEnabled(enable);
    }));
    widget->setLayout(layout);
    widget->setMinimumSize(64, 64);
    setObjectName("devices");
    setWidget(widget);
}

void Devices::onDoubleClick(const QModelIndex &)
{
    onShell();
}

void Devices::onExplore()
{
    Device device = selected();
    if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
        return;
    emit showExplorer(device.serial);
}

void Devices::onInformation()
{
    Device device = selected();
    if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
        return;
    emit showInformation(device.serial);
}

void Devices::onLogcat()
{
    Device device = selected();
    if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
        return;
    emit showLogcat(device.serial);
}

void Devices::onRefresh()
{
    if (tree->model()->hasChildren())
        tree->model()->removeRows(0, tree->model()->rowCount());
    QVector<Device> devices = ADB::instance()->devices();
    if (devices.count() <= 0)
        return;
    foreach (const Device &device, devices) {
        QTreeWidgetItem *row = new QTreeWidgetItem();
        switch (device.status) {
        case Device::BOOTLOADER:
            row->setIcon(0, ::icon("mobile_phone_warning"));
            row->setToolTip(0, translate("status_bootloader"));
            break;
        case Device::ONLINE:
            row->setIcon(0, ::icon("mobile_phone"));
            row->setToolTip(0, translate("status_online"));
            break;
        case Device::UNAUTHORIZED:
            row->setIcon(0, ::icon("mobile_phone_minus"));
            row->setToolTip(0, translate("status_unauthorized"));
            break;
        default:
            row->setIcon(0, ::icon("mobile_phone_off"));
            row->setToolTip(0, translate("status_offline"));
            break;
        }
        row->setText(1, device.serial);
        switch (device.type) {
        case Device::DEVICE:
            row->setText(2, translate("type_phone"));
            break;
        default:
            row->setText(2, translate("type_emulator"));
            break;
        }
        row->setData(0, ROLE_STRUCT, QVariant().fromValue(device));
        tree->addTopLevelItem(row);
    }
    tree->header()->resizeSections(QHeaderView::ResizeToContents);
}

void Devices::onScreenshot()
{
    Device device = selected();
    if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
        return;
    QString file = QString(Settings::previousDirectory());
    file.append('/');
    file.append(device.serial);
    file.append('_');
    file.append(Format::timestamp(QDateTime::currentDateTime(), FORMAT_TIMESTAMP_FILE));
    QString path = QFileDialog::getSaveFileName(this, translate("title_save"), file, "PNG Image Files (*.png)", 0, 0);
    if (path.isNull() || path.isEmpty())
        return;
    if (QFile::exists(path))
        QFile::remove(path);
    Settings::previousDirectory(QFileInfo(path).dir().absolutePath());
    ADB::instance()->screenshot(device.serial, path);
    if (QFile::exists(path))
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Devices::onShell()
{
    Device device = selected();
    if ((device.serial.isEmpty()) || (device.status != Device::ONLINE))
        return;
    ADB::instance()->shell(device.serial);
}

Device Devices::selected() const
{
    Device device;
    QModelIndexList selection = this->tree->selectionModel()->selectedRows(0);
    if (selection.count() == 1)
        device = selection.first().data(ROLE_STRUCT).value<Device>();
    return device;
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
