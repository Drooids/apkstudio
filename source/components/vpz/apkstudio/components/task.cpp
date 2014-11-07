#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

Task::Task(const QString &title, QWidget *parent) :
    QWidget(parent)
{
    QLayout *layout = new QVBoxLayout(this);
    QLabel *output = new QLabel(translate("output_idle"), this);
    connections.append(connect(this, &Task::lineRead, [ output ] (const QString &line) {
        if (line.isEmpty())
            return;
        output->setText(line);
    }));
    progress = new QProgressBar(this);
    progress->setEnabled(false);
    progress->setMaximumHeight(12);
    progress->setTextVisible(false);
    layout->addWidget(new QLabel(title, this));
    layout->addWidget(progress);
    layout->addWidget(output);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(4);
    setLayout(layout);
}

void Task::start()
{
}

void Task::stop()
{
}

void Task::run(const QString &binary, const QStringList &arguments)
{
    Q_UNUSED(binary);
    Q_UNUSED(arguments);
}

Task::~Task()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
