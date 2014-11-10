#ifndef VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP
#define VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP

#include <QThreadPool>
#ifdef Q_OS_WIN
#   include <QWinTaskbarProgress>
#endif
#include "async/task.hpp"
#include "helpers/settings.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "dockable.hpp"
#include "listwidget.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Tasks : public Dockable
{
    Q_OBJECT
private:
#ifdef Q_OS_WIN
    QWinTaskbarProgress *progress;
#endif
    ListWidget *list;
    QThreadPool threads;
    static Tasks *self;
private:
    static QString translate(const char *key) {
        return Helpers::Text::translate("tasks", key);
    }
public:
    explicit Tasks(QWidget * = 0);
    void add(const QString &, Async::Task *);
#ifdef Q_OS_WIN
    void setProgress(QWinTaskbarProgress *);
#endif
    static Tasks *instance();
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_TASKS_HPP
