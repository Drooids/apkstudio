#ifndef VPZ_APKSTUDIO_WINDOWS_EXPLORER_HPP
#define VPZ_APKSTUDIO_WINDOWS_EXPLORER_HPP

#include <QAction>
#include <QActionGroup>
#include <QHeaderView>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include "components/applications.hpp"
#include "components/information.hpp"
#include "components/music.hpp"
#include "components/partitions.hpp"
#include "components/photos.hpp"
#include "components/storage.hpp"
#include "components/treewidget.hpp"
#include "components/videos.hpp"
#include "helpers/format.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "resources/variant.hpp"
#include "application.hpp"
#include "chmod.hpp"
#include "dialog.hpp"
#include "file.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

class Explorer : public Dialog
{
    Q_OBJECT
private:
    enum {
        NAVIGATION_APPLICATIONS = 1,
        NAVIGATION_STORAGE,
        NAVIGATION_INFORMATION,
        NAVIGATION_MUSIC,
        NAVIGATION_PARTITIONS,
        NAVIGATION_PHOTOS,
        NAVIGATION_VIDEOS
    };
    QList<QMetaObject::Connection> contextual;
    QString device;
    QSplitter *splitter;
    QTabWidget *tabs;
    Components::TreeWidget *tree;
private:
    void createTabs();
    void createToolbar();
    void createTree();
    static QString translate(const char *key) {
        return Helpers::Text::translate("explorer", key);
    }
private slots:
    void onNodeClicked(const QModelIndex &);
    void onShowApplication(const QString &);
    void onShowCHMOD(const Resources::File &);
    void onShowFile(const QString &);
public:
    explicit Explorer(const QString &, QWidget * = 0);
    ~Explorer();
public slots:
    void onInitComplete();
signals:
    void refreshTriggered();
};

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_WINDOWS_EXPLORER_HPP
