#include "tasks.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Components {

Tasks::Tasks(QWidget *parent) :
    Dockable(translate("title_dock"), parent)
{
    list = new QListWidget(this);
    list->setMinimumSize(64, 64);
    add("Check version", "java", QStringList("--version"));
    setObjectName("tasks");
    setWidget(list);
}

void Tasks::add(const QString &title, const QString &binary, const QStringList &arguments)
{
    QListWidgetItem *item = new QListWidgetItem("", list);
    Task *task = new Task(title, this);
    item->setSizeHint(task->sizeHint());
    list->addItem(item);
    list->setItemWidget(item, task);
    task->run(binary, arguments);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
