#include "tasks.hpp"

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
    threads.setMaxThreadCount(2);
    widget->setLayout(layout);
    setObjectName("tasks");
    setWidget(widget);
}

void Tasks::add(const QString &title, Async::Task *task)
{
    Task *item = new Task(title, task, list);
    connections.append(connect(item, &Task::finished, [ item ] () {
        delete item;
    }));
    list->addItem(item);
    list->setItemWidget(item, item->widget);
    threads.start(task);
}

Tasks *Tasks::instance()
{
    return self;
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
