#ifndef VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP
#define VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP

#include <QAction>
#include <QListWidget>
#include <QToolBar>
#include "helpers/settings.hpp"
#include "helpers/text.hpp"
#include "dockable.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Tasks : public Dockable
{
    Q_OBJECT
private:
    QListWidget *list;
    static Tasks *self;
private:
    static QString translate(const char *key) {
        return Helpers::Text::translate("tasks", key);
    }
private slots:
    void onChange();
    void onClear();
    void onStop();
public:
    explicit Tasks(QWidget *parent = 0);
    void add(const QString &, Resources::Runnable, Resources::Callback);
    static Tasks *instance();
signals:
    void enableClear(bool);
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP
