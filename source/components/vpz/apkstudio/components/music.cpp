#include "music.hpp"

using namespace VPZ::APKStudio::Async;
using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Components {

Music::Music(const QString &device, QWidget *parent) :
    QTreeWidget(parent), device(device)
{
    QStringList labels;
    labels << translate("header_name");
    labels << translate("header_duration");
    labels << translate("header_size");
    labels << translate("header_time");
    setHeaderLabels(labels);
    setColumnWidth(0, 160);
    setColumnWidth(1, 64);
    setColumnWidth(2, 64);
    setColumnWidth(3, 96);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSortingEnabled(true);
    sortByColumn(3, Qt::DescendingOrder);
    // -- //
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this, SLOT(onCopy()));
    new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(onDetails()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), this, SLOT(onMove()));
    new QShortcut(QKeySequence(Qt::Key_F5), this, SLOT(onRefresh()));
    new QShortcut(QKeySequence(Qt::Key_F2), this, SLOT(onRename()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(onRemove()));
}

void Music::onAction(QAction *action)
{
    switch (action->data().toInt())
    {
    case ACTION_COPY:
        onCopy();
        break;
    case ACTION_DETAILS:
        onDetails();
        break;
    case ACTION_MOVE:
        onMove();
        break;
    case ACTION_PULL:
        onPull();
        break;
    case ACTION_REMOVE:
        onRemove();
        break;
    case ACTION_RENAME:
        onRename();
        break;
    }
}

void Music::onCopy()
{
    QVector<Resources::Music> files = selected();
    if (files.isEmpty())
        return;
    bool ok = false;
    QString destination = QInputDialog::getText(this, translate("title_copy"), translate("label_copy"), QLineEdit::Normal, "/", &ok);
    if (!ok || destination.trimmed().isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_copy"), translate("message_copy").arg(QString::number(files.count()), destination), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    QMap<QString, bool> paths;
    foreach (const Resources::Music &file, files)
        paths[file.path] = false;
    Copy *copy = new Copy(device, paths, destination);
    connections.append(connect(copy, SIGNAL(finished(QVariant)), this, SLOT(onCopyFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_copy").arg(QString::number(paths.count())), copy);
}

void Music::onCopyFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_copy_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Music::onDetails()
{
    QVector<Resources::Music> music = selected();
    if (music.isEmpty())
        return;
    emit showFile(music.first().path);
}

void Music::onMove()
{
    QVector<Resources::Music> files = selected();
    if (files.isEmpty())
        return;
    QStringList sources;
    foreach (const Resources::Music &file, files)
        sources << file.path;
    bool ok = false;
    QString destination = QInputDialog::getText(this, translate("title_move"), translate("label_move"), QLineEdit::Normal, "/", &ok);
    if (!ok || destination.trimmed().isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_move"), translate("message_move").arg(QString::number(files.count()), destination), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    Move *move = new Move(device, sources, destination);
    connections.append(connect(move, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onMoveFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_move").arg(QString::number(files.count())), move);
}

void Music::onMoveFinished(const QVariant &result, const QStringList &a)
{
    Q_UNUSED(a)
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_move_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (pair.first >= 1)
        onRefresh();
}

void Music::onPull()
{
    QVector<Resources::Music> files = selected();
    if (files.isEmpty())
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
    foreach(const Resources::Music &file, files)
        paths[file.path] = false;
    Pull *pull = new Pull(device, paths, directory);
    connections.append(connect(pull, SIGNAL(finished(QVariant)), this, SLOT(onPullFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_pull").arg(QString::number(paths.count())), pull);
}

void Music::onPullFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_pull_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Music::onRefresh()
{
    if (model()->hasChildren())
        model()->removeRows(0, model()->rowCount());
    QVector<Resources::Music> musics = ADB::instance()->music(device);
    foreach (const Resources::Music &music, musics) {
        QTreeWidgetItem *row = new QTreeWidgetItem();
        for (int i = 0; i < 4; ++i)
            row->setData(i, ROLE_STRUCT, QVariant::fromValue(music));
        row->setIcon(0, ::icon("music"));
        row->setText(0, music.name);
        row->setText(1, Format::timestamp(QDateTime::fromTime_t(music.duration / 1000).toUTC(), "hh:mm:ss"));
        row->setText(2, Format::size(music.size));
        row->setText(3, Format::timestamp(QDateTime::fromTime_t(music.time)));
        row->setToolTip(0, music.path);
        addTopLevelItem(row);
    }
    scrollToTop();
}

void Music::onRemove()
{
    QVector<Resources::Music> files = selected();
    if (files.isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_remove"), translate("message_remove").arg(QString::number(files.count())), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    QMap<QString, bool> removeable;
    foreach (const Resources::Music &file, files)
        removeable[file.path] = false;
    Remove *remove = new Remove(device, removeable);
    connections.append(connect(remove, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onRemoveFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_remove").arg(QString::number(removeable.count())), remove);
}

void Music::onRemoveFinished(const QVariant &result, const QStringList &removed)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_remove_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (removed.isEmpty())
        return;
    foreach (const QString &path, removed) {
        QList<QTreeWidgetItem *> rows = findItems(path.section('/', -1, -1), Qt::MatchExactly, 0);
        if (rows.count() != 1)
            continue;
        delete rows.first();
    }
}

void Music::onRename()
{
    QVector<Resources::Music> files = selected();
    if (files.isEmpty())
        return;
    bool ok = false;
    QString name = QInputDialog::getText(this, translate("title_rename"), translate("label_rename"), QLineEdit::Normal, files.first().name, &ok);
    if (!ok || name.trimmed().isEmpty())
        return;
    bool multiple = (files.count() > 1);
    QMap<QString, QString> renameable;
    for (int i = 0; i < files.count(); ++i) {
        QString newname(name);
        if (multiple)
            newname.prepend(QString("(%1) ").arg(QString::number(i + 1)));
        Resources::Music file = files.at(i);
        QString newpath(file.path.section('/', 0, -2));
        newpath.append('/');
        newpath.append(newname);
        renameable[file.path] = newpath;
    }
    Rename *rename = new Rename(device, renameable);
    connections.append(connect(rename, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onRenameFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_rename").arg(QString::number(renameable.count())), rename);
}

void Music::onRenameFinished(const QVariant &result, const QStringList &a)
{
    Q_UNUSED(a)
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_rename_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (pair.first >= 1)
        onRefresh();
}

QVector<Resources::Music> Music::selected()
{
    QVector<Resources::Music> files;
    foreach (QTreeWidgetItem *item, selectedItems())
        files.append(item->data(0, ROLE_STRUCT).value<Resources::Music>());
    return files;
}

Music::~Music()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
