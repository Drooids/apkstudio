#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

Task::Task(QObject *parent)
    : QObject(parent)
{
    setAutoDelete(true);
}

void Task::run()
{
    emit started();
    start();
    emit stopped();
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
