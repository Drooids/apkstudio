#include "tasks.hpp"
#include <QDebug>

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

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
    list = new QListWidget(widget);
    QToolBar *tool_bar = new QToolBar(widget);
    QAction *clear = new QAction(::icon("bin_empty"), translate("item_clear"), tool_bar);
    QAction *stop = new QAction(::icon("control_stop"), translate("item_stop"), tool_bar);
    connections.append(connect(clear, SIGNAL(triggered()), this, SLOT(onClear())));
    connections.append(connect(stop, SIGNAL(triggered()), this, SLOT(onStop())));
    layout->addWidget(tool_bar);
    connections.append(connect(list->selectionModel(), &QItemSelectionModel::selectionChanged, [ clear, stop, this ] (const QItemSelection &current, const QItemSelection &previous) {
        Q_UNUSED(current)
        Q_UNUSED(previous)
        stop->setEnabled(this->list->selectionModel()->selectedRows(0).count() == 1);
    }));
    connections.append(connect(this, &Tasks::enableClear, [ clear ] (const bool enable) {
        clear->setEnabled(enable);
    }));
    clear->setEnabled(false);
    layout->addWidget(list);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setSpacing(0);
    list->setMinimumSize(160, 160);
    stop->setEnabled(false);
    tool_bar->addAction(stop);
    tool_bar->addAction(clear);
    widget->setLayout(layout);
    setObjectName("tasks");
    setWidget(widget);
}

void Tasks::add(const QString &title, Resources::Runnable runnable, Resources::Callback callback)
{
    QListWidgetItem *item = new QListWidgetItem("", list);
    Task *task = new Task(title, runnable, callback, this);
    item->setSizeHint(task->sizeHint());
    list->addItem(item);
    list->setItemWidget(item, task);
    onChange();
}

Tasks *Tasks::instance()
{
    return self;
}

void Tasks::onChange()
{
    int rows = list->model()->rowCount(list->rootIndex());
    emit enableClear(rows >= 1);
    if (rows <= 0)
        return;
    QListWidgetItem *item = list->item(0);
    Task *task = qobject_cast<Task *>(list->itemWidget(item));
    if (task->isRunning())
        return;
    if (task->isFinished()) {
        delete item;
        delete task;
        onChange();
    } else
        task->start();
}

void Tasks::onClear()
{
    foreach(QListWidgetItem *item, list->findItems("*", Qt::MatchWildcard)) {
        Task *task = qobject_cast<Task *>(list->itemWidget(item));
        if (!task)
            continue;
        if (task->isRunning())
            task->stop();
        delete item;
        delete task;
    }
    onChange();
}

void Tasks::onStop()
{
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
