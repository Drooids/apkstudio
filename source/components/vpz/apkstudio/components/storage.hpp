#ifndef VPZ_APKSTUDIO_COMPONENTS_STORAGE_HPP
#define VPZ_APKSTUDIO_COMPONENTS_STORAGE_HPP

#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "async/copy.hpp"
#include "async/move.hpp"
#include "async/pull.hpp"
#include "async/push.hpp"
#include "async/remove.hpp"
#include "async/rename.hpp"
#include "helpers/adb.hpp"
#include "helpers/format.hpp"
#include "helpers/text.hpp"
#include "resources/variant.hpp"
#include "clearable.hpp"
#include "tasks.hpp"
#include "treewidget.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Storage : public QWidget
{
    Q_OBJECT
private:
    QList<QMetaObject::Connection> connections;
    QString device;
    Clearable *path;
    TreeWidget *tree;
private:
    void onCHMOD();
    void onCopy(const QVector<Resources::File> &, const QString &);
    void onMove(const QStringList &, const QString &);
    void onPull();
    void onPush();
    QVector<Resources::File> selected();
    static QString translate(const char *key) {
        return Helpers::Text::translate("storage", key);
    }
private slots:
    void onCreate();
    void onCopy();
    void onCopyFinished(const QVariant &);
    void onDetails();
    void onDoubleClicked(const QModelIndex &);
    void onEdit();
    void onFilesDropped(const QStringList &, const QModelIndex &);
    void onItemsDropped(const QModelIndexList &, const QModelIndex &, Qt::DropAction);
    void onMove();
    void onMoveFinished(const QVariant &, const QStringList &);
    void onPullFinished(const QVariant &);
    void onPushFinished(const QVariant &);
    void onRefresh();
    void onRemove();
    void onRemoveFinished(const QVariant &, const QStringList &);
    void onRename();
    void onRenameFinished(const QVariant &, const QStringList &);
    void onReturn();
    void onUp();
public:
    enum {
        ACTION_CHMOD = 1,
        ACTION_COPY,
        ACTION_CREATE,
        ACTION_DETAILS,
        ACTION_MOVE,
        ACTION_PULL,
        ACTION_PUSH,
        ACTION_REMOVE,
        ACTION_RENAME
    };
    explicit Storage(const QString &, QWidget * = 0);
    ~Storage();
public slots:
    void onAction(QAction *);
signals:
    void showCHMOD(Resources::File);
    void showFile(QString);
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_STORAGE_HPP
