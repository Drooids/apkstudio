#ifndef VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
#define VPZ_APKSTUDIO_COMPONENTS_TASK_HPP

#include <QBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QPushButton>
#include "async/task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Task : public QObject, public QListWidgetItem
{
    Q_OBJECT
private:
    QList<QMetaObject::Connection> connections;
public:
    QProgressBar *progress;
    Async::Task *task;
    QWidget *widget;
public:
    explicit Task(const QString &, Async::Task *, QListWidget * = 0);
public slots:
    void onStart();
    void onStop();
signals:
    void finished();
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
