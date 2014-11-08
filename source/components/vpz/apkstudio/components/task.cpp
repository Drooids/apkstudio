#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

Task::Task(const QString &title, Async::Task *task, QListWidget *parent)
    : QListWidgetItem("", parent), task(task)
{
    widget = new QWidget(parent);
    QLayout *layout = new QVBoxLayout(widget);
    progress = new QProgressBar(widget);
    connections.append(connect(task, SIGNAL(started()), this, SLOT(onStart())));
    connections.append(connect(task, SIGNAL(stopped()), this, SLOT(onStop())));
    layout->addWidget(new QLabel(title, widget));
    layout->addWidget(progress);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(4);
    progress->setEnabled(false);
    progress->setMaximumHeight(12);
    progress->setMaximum(0);
    progress->setMinimum(0);
    progress->setTextVisible(false);
    widget->setLayout(layout);
    setSizeHint(widget->sizeHint());
}

void Task::onStart()
{
    progress->setEnabled(true);
}

void Task::onStop()
{
    progress->setEnabled(false);
    emit finished();
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
