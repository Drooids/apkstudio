#include "videos.hpp"

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Components {

Videos::Videos(const QString &device, QWidget *parent) :
    QTreeWidget(parent), device(device)
{
    QStringList labels;
    labels << translate("header_name");
    labels << translate("header_resolution");
    labels << translate("header_duration");
    labels << translate("header_size");
    labels << translate("header_time");
    setHeaderLabels(labels);
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

void Videos::onAction(QAction *action)
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

void Videos::onCopy()
{
    QVector<Video> files = selected();
    if (files.isEmpty())
        return;
    bool ok = false;
    QString destination = QInputDialog::getText(this, translate("title_copy"), translate("label_copy"), QLineEdit::Normal, "/", &ok);
    if (!ok || destination.trimmed().isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_copy"), translate("message_copy").arg(QString::number(files.count()), destination), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    int failed = 0;
    int successful = 0;
    foreach (const Video &file, files) {
        if (ADB::instance()->copy(device, file.path, destination, false))
            successful++;
        else
            failed++;
    }
    if (failed >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_copy_failed").arg(QString::number(successful), QString::number(failed)), QMessageBox::Close);
}

void Videos::onDetails()
{
    QVector<Video> videos = selected();
    if (videos.isEmpty())
        return;
    emit showFile(videos.first().path);
}

void Videos::onMove()
{
    QVector<Video> files = selected();
    if (files.isEmpty())
        return;
    QStringList sources;
    foreach (const Video &file, files)
        sources << file.path;
    bool ok = false;
    QString destination = QInputDialog::getText(this, translate("title_move"), translate("label_move"), QLineEdit::Normal, "/", &ok);
    if (!ok || destination.trimmed().isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_move"), translate("message_move").arg(QString::number(files.count()), destination), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    if (ADB::instance()->move(device, sources, destination))
        onRefresh();
    else
        QMessageBox::critical(this, translate("title_failure"), translate("message_move_failed").arg(destination), QMessageBox::Close);
}

void Videos::onPull()
{
    QVector<Video> files = selected();
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
    int failed = 0;
    int successful = 0;
    foreach (const Video &file, files) {
        if (ADB::instance()->pull(device, file.path, directory.absolutePath()))
            successful++;
        else
            failed++;
    }
    if (failed >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_pull_failed").arg(QString::number(successful), QString::number(failed)), QMessageBox::Close);
}

void Videos::onRefresh()
{
    if (model()->hasChildren())
        model()->removeRows(0, model()->rowCount());
    QVector<Video> videos = ADB::instance()->videos(device);
    foreach (const Video &video, videos) {
        QTreeWidgetItem *row = new QTreeWidgetItem();
        for (int i = 0; i < 4; ++i)
            row->setData(i, ROLE_STRUCT, QVariant::fromValue(video));
        row->setIcon(0, ::icon("film"));
        row->setText(0, video.name);
        row->setText(1, QString::number(video.width).append('x').append(QString::number(video.height)));
        row->setText(2, Format::timestamp(QDateTime::fromTime_t(video.duration / 1000).toUTC(), "hh:mm:ss"));
        row->setText(3, Format::size(video.size));
        row->setText(4, Format::timestamp(QDateTime::fromTime_t(video.time)));
        row->setToolTip(0, video.path);
        addTopLevelItem(row);
    }
    scrollToTop();
}

void Videos::onRemove()
{
    QVector<Video> files = selected();
    if (files.isEmpty())
        return;
    int result =  QMessageBox::question(this, translate("title_remove"), translate("message_remove").arg(QString::number(files.count())), QMessageBox::No | QMessageBox::Yes);
    if (result != QMessageBox::Yes)
        return;
    int failed = 0;
    int successful = 0;
    foreach (const Video &file, files) {
        if (ADB::instance()->remove(device, file.path, false)) {
            successful++;
            QList<QTreeWidgetItem *> rows = findItems(file.name, Qt::MatchExactly, 0);
            if (rows.count() != 1)
                continue;
            delete rows.first();
        } else
            failed++;
    }
    if (failed >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_remove_failed").arg(QString::number(successful), QString::number(failed)), QMessageBox::Close);
}

void Videos::onRename()
{
    QVector<Video> files = selected();
    if (files.isEmpty())
        return;
    bool ok = false;
    QString name = QInputDialog::getText(this, translate("title_rename"), translate("label_rename"), QLineEdit::Normal, files.first().name, &ok);
    if (!ok || name.trimmed().isEmpty())
        return;
    bool multiple = (files.count() > 1);
    int failed = 0;
    int successful = 0;
    for (int i = 0; i < files.count(); ++i) {
        QString newname(name);
        if (multiple)
            newname.prepend(QString("(%1) ").arg(QString::number(i + 1)));
        Video file = files.at(i);
        QString newpath(file.path.section('/', 0, -2));
        newpath.append('/');
        newpath.append(newname);
        if (ADB::instance()->move(device, file.path, newpath)) {
            successful++;
            QList<QTreeWidgetItem *> rows = findItems(file.name, Qt::MatchExactly, 0);
            if (rows.count() != 1)
                continue;
            file.name = newname;
            file.path = newpath;
            QTreeWidgetItem *row = rows.first();
            for (int i = 0; i < 6; ++i)
                row->setData(i, ROLE_STRUCT, QVariant::fromValue(file));
            row->setText(0, newname);
            row->setToolTip(0, newpath);
        } else
            failed++;
    }
    if (failed >= 1)
        QMessageBox::critical(this, translate("title_failure"), translate("message_rename_failed").arg(QString::number(successful), QString::number(failed)), QMessageBox::Close);
}

QVector<Video> Videos::selected()
{
    QVector<Video> files;
    foreach (QTreeWidgetItem *item, selectedItems())
        files.append(item->data(0, ROLE_STRUCT).value<Video>());
    return files;
}

Videos::~Videos()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
