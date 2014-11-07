#ifndef VPZ_APKSTUDIO_COMPONENTS_PHOTOS_HPP
#define VPZ_APKSTUDIO_COMPONENTS_PHOTOS_HPP

#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QTreeWidget>
#include "helpers/adb.hpp"
#include "helpers/format.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "resources/variant.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Photos : public QTreeWidget
{
    Q_OBJECT
private:
    QList<QMetaObject::Connection> connections;
    QString device;
private:
    QVector<Resources::Photo> selected();
    static QString translate(const char *key) {
        return Helpers::Text::translate("photos", key);
    }
private slots:
    void onCopy();
    void onDetails();
    void onMove();
    void onPull();
    void onRemove();
    void onRename();
public:
    enum {
        ACTION_COPY = 1,
        ACTION_DETAILS,
        ACTION_MOVE,
        ACTION_PULL,
        ACTION_REMOVE,
        ACTION_RENAME
    };
    explicit Photos(const QString &, QWidget * = 0);
    ~Photos();
public slots:
    void onAction(QAction *);
    void onRefresh();
signals:
    void showFile(QString);
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_PHOTOS_HPP
