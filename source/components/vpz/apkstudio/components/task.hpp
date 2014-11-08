#ifndef VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
#define VPZ_APKSTUDIO_COMPONENTS_TASK_HPP

#include <functional>
#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>
#include "helpers/async.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "resources/variant.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Task : public QWidget
{
    Q_OBJECT
private:
    Helpers::Async *async;
    Resources::Callback callback;
    QList<QMetaObject::Connection> connections;
    QLabel *output;
    QProgressBar *progress;
    Resources::Runnable runnable;
private:
    void createButtons();
    void createLabel(const QString &);
    void createProgress();
    static QString translate(const char *key) {
        return Helpers::Text::translate("task", key);
    }
private slots:
    void onFinished(const QVariant &);
public:
    explicit Task(const QString &, Resources::Runnable, Resources::Callback, QWidget * = 0);
    bool isFinished();
    bool isRunning();
    void start();
    void stop();
    ~Task();
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
