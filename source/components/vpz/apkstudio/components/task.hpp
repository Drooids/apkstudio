#ifndef VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
#define VPZ_APKSTUDIO_COMPONENTS_TASK_HPP

#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>
#include "resources/embedded.hpp"
#include "helpers/text.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Task : public QWidget
{
    Q_OBJECT
private:
    QProgressBar *progress;
    QList<QMetaObject::Connection> connections;
private:
    void createButtons();
    void createLabel(const QString &);
    void createProgress();
    static QString translate(const char *key) {
        return Helpers::Text::translate("task", key);
    }
public:
    explicit Task(const QString &, QWidget * = 0);
    void run(const QString &, const QStringList &);
    void start();
    void stop();
    ~Task();
signals:
    void lineRead(const QString &);
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_TASK_HPP
