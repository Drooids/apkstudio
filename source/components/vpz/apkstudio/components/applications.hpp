#ifndef VPZ_APKSTUDIO_COMPONENTS_APPLICATIONS_HPP
#define VPZ_APKSTUDIO_COMPONENTS_APPLICATIONS_HPP

#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include "async/install.hpp"
#include "async/pull.hpp"
#include "async/uninstall.hpp"
#include "helpers/adb.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "resources/variant.hpp"
#include "tasks.hpp"
#include "treewidget.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Applications : public TreeWidget
{
    Q_OBJECT
    QString device;
private:
    void onDisable();
    void onEnable();
    void onPull();
    QVector<Resources::Application> selected();
    static QString translate(const char *key) {
        return Helpers::Text::translate("applications", key);
    }
private slots:
    void onDetails();
    void onFilesDropped(const QStringList &, const QModelIndex &);
    void onInstall();
    void onInstallFinished(const QVariant &);
    void onPullFinished(const QVariant &);
    void onUninstall();
    void onUninstallFinished(const QVariant &, const QStringList &);
public:
    enum {
        ACTION_DETAILS = 1,
        ACTION_DISABLE,
        ACTION_ENABLE,
        ACTION_INSTALL,
        ACTION_PULL,
        ACTION_UNINSTALL
    };
    explicit Applications(const QString &, QWidget * = 0);
public slots:
    void onAction(QAction *);
    void onRefresh();
signals:
    void showApplication(QString);
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_APPLICATIONS_HPP
