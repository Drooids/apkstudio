#include "applications.hpp"

using namespace VPZ::APKStudio::Async;
using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Components {

Applications::Applications(const QString &device, QWidget *parent) :
    TreeWidget(false, true, parent), device(device)
{
    QStringList labels;
    labels << translate("header_file");
    labels << translate("header_package");
    labels << translate("header_system");
    labels << translate("header_status");
    connections.append(connect(this, &Applications::filesDropped, this, &Applications::onFilesDropped));
    setHeaderLabels(labels);
    setColumnWidth(0, 160);
    setColumnWidth(1, 160);
    setColumnWidth(2, 64);
    setColumnWidth(3, 64);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    // -- //
    new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(onDetails()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_I), this, SLOT(onInstall()));
    new QShortcut(QKeySequence(Qt::Key_F5), this, SLOT(onRefresh()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(onUninstall()));
}

void Applications::onAction(QAction *action)
{
    switch (action->data().toInt())
    {
    case ACTION_DETAILS:
        onDetails();
        break;
    case ACTION_DISABLE:
        onDisable();
        break;
    case ACTION_ENABLE:
        onEnable();
        break;
    case ACTION_INSTALL:
        onInstall();
        break;
    case ACTION_PULL:
        onPull();
        break;
    case ACTION_UNINSTALL:
        onUninstall();
        break;
    }
}

void Applications::onDetails()
{
    QVector<Application> applications = selected();
    if (applications.isEmpty())
        return;
    emit showApplication(applications.first().package);
}

void Applications::onDisable()
{
    QVector<Application> applications = selected();
    if (applications.isEmpty())
        return;
    Enable *disable = new Enable(device, applications, false);
    connections.append(connect(disable, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onDisableFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_disable").arg(QString::number(applications.count())), disable);
}

void Applications::onDisableFinished(const QVariant &result, const QStringList &disabled)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_disable_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (disabled.isEmpty())
        return;
    foreach (const QString &package, disabled) {
        QList<QTreeWidgetItem *> rows = findItems(package, Qt::MatchExactly, 1);
        if (rows.count() != 1)
            continue;
        rows.first()->setIcon(0, ::icon("cross"));
    }
}

void Applications::onEnable()
{
    QVector<Application> applications = selected();
    if (applications.isEmpty())
        return;
    Enable *enable = new Enable(device, applications, true);
    connections.append(connect(enable, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onEnableFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_enable").arg(QString::number(applications.count())), enable);
}

void Applications::onEnableFinished(const QVariant &result, const QStringList &enabled)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_enable_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (enabled.isEmpty())
        return;
    foreach (const QString &package, enabled) {
        QList<QTreeWidgetItem *> rows = findItems(package, Qt::MatchExactly, 1);
        if (rows.count() != 1)
            continue;
        rows.first()->setIcon(0, ::icon("tick"));
    }
}

void Applications::onFilesDropped(const QStringList &files, const QModelIndex &a)
{
    Q_UNUSED(a)
    QStringList apks;
    foreach (const QString &file, files) {
        QFileInfo apk(file);
        if (apk.exists() && (QString::compare(apk.suffix(), "apk", Qt::CaseInsensitive) == 0))
            apks.append(apk.absoluteFilePath());
    }
    if (apks.count() <= 0)
        return;
    int result =  QMessageBox::question(this, translate("title_install"), translate("message_install").arg(QString::number(apks.count())), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    Install *install = new Install(device, apks);
    connections.append(connect(install, SIGNAL(finished(QVariant)), this, SLOT(onInstallFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_install").arg(QString::number(apks.count())), install);
}

void Applications::onInstall()
{
    QFileDialog dialog(this, translate("title_select"), Helpers::Settings::previousDirectory(), "Android APK (*.apk)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec() != QFileDialog::Accepted)
        return;
    Helpers::Settings::previousDirectory(dialog.directory().absolutePath());
    QStringList files = dialog.selectedFiles();
    if (files.isEmpty())
        return;
    onFilesDropped(files, QModelIndex());
}

void Applications::onInstallFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_install_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Applications::onPull()
{
    QVector<Application> applications = selected();
    if (applications.isEmpty())
        return;
    QFileDialog dialog(this, translate("title_browse"), Helpers::Settings::previousDirectory());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec() != QFileDialog::Accepted)
        return;
    QStringList folders = dialog.selectedFiles();
    if (folders.count() != 1)
        return;
    QDir directory(folders.first());
    Helpers::Settings::previousDirectory(directory.absolutePath());
    QMap<QString, bool> paths;
    foreach(const Application &application, applications)
        paths[application.path] = false;
    Pull *pull = new Pull(device, paths, directory);
    connections.append(connect(pull, SIGNAL(finished(QVariant)), this, SLOT(onPullFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_pull").arg(QString::number(paths.count())), pull);
}

void Applications::onPullFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_pull_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Applications::onRefresh()
{
    if (model()->hasChildren())
        model()->removeRows(0, model()->rowCount());
    QVector<Application> applications = ADB::instance()->applications(device);
    foreach (const Application &application, applications) {
        QTreeWidgetItem *row = new QTreeWidgetItem();
        for (int i = 0; i < 4; ++i)
            row->setData(i, ROLE_STRUCT, QVariant::fromValue(application));
        row->setIcon(0, ::icon(application.enabled ? "tick" : "cross"));
        row->setText(0, application.name);
        row->setText(1, application.package);
        row->setText(2, application.system ? "Yes" : "No");
        row->setText(3, application.enabled ? "Enabled" : "Disabled");
        row->setToolTip(0, application.path);
        addTopLevelItem(row);
    }
    scrollToTop();
}

QVector<Application> Applications::selected()
{
    QVector<Application> applications;
    foreach (QTreeWidgetItem *item, selectedItems())
        applications.append(item->data(0, ROLE_STRUCT).value<Application>());
    return applications;
}

void Applications::onUninstall()
{
    QVector<Application> applications = selected();
    if (applications.isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_uninstall"), translate("message_uninstall").arg(QString::number(applications.count())), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    Uninstall *uninstall = new Uninstall(device, applications);
    connections.append(connect(uninstall, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onUninstallFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_uninstall").arg(QString::number(applications.count())), uninstall);
}

void Applications::onUninstallFinished(const QVariant &result, const QStringList &uninstalled)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_uninstall_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (uninstalled.isEmpty())
        return;
    foreach (const QString &package, uninstalled) {
        QList<QTreeWidgetItem *> rows = findItems(package, Qt::MatchExactly, 1);
        if (rows.count() != 1)
            continue;
        delete rows.first();
    }
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
