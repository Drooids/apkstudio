#include "task.hpp"

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Components {

Task::Task(const QString &title, Runnable runnable, Callback callback, QWidget *parent) :
    QWidget(parent), callback(callback), runnable(runnable)
{
    QLayout *layout = new QVBoxLayout(this);
    output = new QLabel(translate("output_idle"), this);
    progress = new QProgressBar(this);
    layout->addWidget(new QLabel(title, this));
    layout->addWidget(progress);
    layout->addWidget(output);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(4);
    progress->setEnabled(false);
    progress->setMaximumHeight(12);
    progress->setTextVisible(false);
    setLayout(layout);
}

bool Task::isFinished()
{
    return (async == NULL) ? false : async->isFinished();
}

bool Task::isRunning()
{
    return (async == NULL) ? false : async->isRunning();
}

void Task::onFinished(const QVariant &variant)
{
    callback(variant);
}

void Task::start()
{
    if (async != NULL)
        delete async;
    async = new Async(this);
    connections.append(connect(async, SIGNAL(finished(QVariant)), this, SLOT(onFinished(QVariant))));
    async->start(runnable);
}

void Task::stop()
{
    if (async == NULL)
        return;
    async->terminate();
}

Task::~Task()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
