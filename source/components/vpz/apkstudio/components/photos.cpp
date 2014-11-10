#include "photos.hpp"

using namespace VPZ::APKStudio::Async;
using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Components {

Photos::Photos(const QString &device, QWidget *parent) :
    QTreeWidget(parent), device(device)
{
    QStringList labels;
    labels << translate("header_name");
    labels << translate("header_resolution");
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

void Photos::onAction(QAction *action)
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

void Photos::onCopy()
{
    QVector<Photo> files = selected();
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
    foreach (const Photo &file, files)
        paths[file.path] = false;
    Copy *copy = new Copy(device, paths, destination);
    connections.append(connect(copy, SIGNAL(finished(QVariant)), this, SLOT(onCopyFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_copy").arg(QString::number(paths.count())), copy);
}

void Photos::onCopyFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_copy_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Photos::onDetails()
{
    QVector<Photo> photos = selected();
    if (photos.isEmpty())
        return;
    emit showFile(photos.first().path);
}

void Photos::onMove()
{
    QVector<Photo> files = selected();
    if (files.isEmpty())
        return;
    QStringList sources;
    foreach (const Photo &file, files)
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

void Photos::onMoveFinished(const QVariant &result, const QStringList &a)
{
    Q_UNUSED(a)
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_move_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (pair.first >= 1)
        onRefresh();
}

void Photos::onPull()
{
    QVector<Photo> files = selected();
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
    foreach(const Photo &file, files)
        paths[file.path] = false;
    Pull *pull = new Pull(device, paths, directory);
    connections.append(connect(pull, SIGNAL(finished(QVariant)), this, SLOT(onPullFinished(QVariant)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_pull").arg(QString::number(paths.count())), pull);
}

void Photos::onPullFinished(const QVariant &result)
{
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_pull_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
}

void Photos::onRefresh()
{
    if (model()->hasChildren())
        model()->removeRows(0, model()->rowCount());
    QVector<Photo> photos = ADB::instance()->photos(device);
    foreach (const Photo &photo, photos) {
        QTreeWidgetItem *row = new QTreeWidgetItem();
        for (int i = 0; i < 4; ++i)
            row->setData(i, ROLE_STRUCT, QVariant::fromValue(photo));
        row->setIcon(0, ::icon("image"));
        row->setText(0, photo.name);
        row->setText(1, QString::number(photo.width).append('x').append(QString::number(photo.height)));
        row->setText(2, Format::size(photo.size));
        row->setText(3, Format::timestamp(QDateTime::fromTime_t(photo.time)));
        row->setToolTip(0, photo.path);
        addTopLevelItem(row);
    }
    scrollToTop();
}

void Photos::onRemove()
{
    QVector<Photo> files = selected();
    if (files.isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_remove"), translate("message_remove").arg(QString::number(files.count())), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    QMap<QString, bool> removeable;
    foreach (const Photo &file, files)
        removeable[file.path] = false;
    Remove *remove = new Remove(device, removeable);
    connections.append(connect(remove, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onRemoveFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_remove").arg(QString::number(removeable.count())), remove);
}

void Photos::onRemoveFinished(const QVariant &result, const QStringList &removed)
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

void Photos::onRename()
{
    QVector<Photo> files = selected();
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
        Photo file = files.at(i);
        QString newpath(file.path.section('/', 0, -2));
        newpath.append('/');
        newpath.append(newname);
        renameable[file.path] = newpath;
    }
    Rename *rename = new Rename(device, renameable);
    connections.append(connect(rename, SIGNAL(finished(QVariant, QStringList)), this, SLOT(onRenameFinished(QVariant, QStringList)), Qt::QueuedConnection));
    Tasks::instance()->add(translate("task_rename").arg(QString::number(renameable.count())), rename);
}

void Photos::onRenameFinished(const QVariant &result, const QStringList &a)
{
    Q_UNUSED(a)
    QPair<int, int> pair = result.value<QPair<int, int>>();
    if (pair.second >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_rename_failed").arg(QString::number(pair.first), QString::number(pair.second)), QMessageBox::Close);
    if (pair.first >= 1)
        onRefresh();
}

QVector<Photo> Photos::selected()
{
    QVector<Photo> files;
    foreach (QTreeWidgetItem *item, selectedItems())
        files.append(item->data(0, ROLE_STRUCT).value<Photo>());
    return files;
}

Photos::~Photos()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
