#ifndef VPZ_APKSTUDIO_COMPONENTS_FRAMEWORK_HPP
#define VPZ_APKSTUDIO_COMPONENTS_FRAMEWORK_HPP

#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include <QTreeWidget>
#include "async/framework.hpp"
#include "helpers/apktool.hpp"
#include "helpers/../../../flowlayout.hpp"
#include "helpers/format.hpp"
#include "helpers/text.hpp"
#include "tasks.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

class Framework : public QWidget
{
    Q_OBJECT
private:
    QList<QMetaObject::Connection> connections;
    QTreeWidget *tree;
private:
    void fixButtonSize(QPushButton *);
    void refreshFrameworks();
    static QString translate(const char *key) {
        return Helpers::Text::translate("framework", key);
    }
private slots:
    void onFrameworkInstalled(const QVariant &);
public:
    explicit Framework(QWidget * = 0);
    ~Framework();
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_FRAMEWORK_HPP
