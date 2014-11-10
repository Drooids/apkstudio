#include "tasks.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Components {

Tasks *Tasks::self = nullptr;

Tasks::Tasks(QWidget *parent) :
    Dockable(translate("title_dock"), parent)
{
    self = this;
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    list = new ListWidget(widget);
    layout->addWidget(list);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    list->setMinimumSize(160, 160);
    threads.setMaxThreadCount(Settings::maxConcurrentTasks());
    widget->setLayout(layout);
    setObjectName("tasks");
    setWidget(widget);
}

void Tasks::add(const QString &title, Async::Task *task)
{
    Task *item = new Task(title, task, list);
    connections.append(connect(item, &Task::finished, [ item, this ] () {
        delete item;
#ifdef Q_OS_WIN
        if (this->list->count() <= 0)
            progress->setVisible(false);
#endif
    }));
    list->addItem(item);
    list->setItemWidget(item, item->widget);
    threads.start(task);
#ifdef Q_OS_WIN
    progress->setVisible(true);
#endif
}

#ifdef Q_OS_WIN
void Tasks::setProgress(QWinTaskbarProgress *progress)
{
    this->progress = progress;
}
#endif

Tasks *Tasks::instance()
{
    return self;
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
