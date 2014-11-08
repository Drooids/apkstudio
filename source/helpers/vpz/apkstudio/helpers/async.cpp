#include "async.hpp"

namespace VPZ {
namespace APKStudio {
namespace Helpers {

Async::Async(QObject *parent) :
    QThread(parent)
{
}

void Async::run()
{
    emit finished(runnable());
}

void Async::start(Resources::Runnable runnable)
{
    this->runnable = runnable;
    QThread::start();
}

} // namespace Helpers
} // namespace APKStudio
} // namespace VPZ
