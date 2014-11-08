#ifndef VPZ_APKSTUDIO_COMPONENTS_LISTWIDGET_HPP
#define VPZ_APKSTUDIO_COMPONENTS_LISTWIDGET_HPP

#include <QListWidget>

namespace VPZ {
namespace APKStudio {
namespace Components {

class ListWidget : public QListWidget
{
    Q_OBJECT
protected:
    QList<QMetaObject::Connection> connections;
public:
    explicit ListWidget(QWidget * = 0);
    QModelIndex index(QListWidgetItem *);
    QListWidgetItem *item(const QModelIndex &);
    ~ListWidget();
};

} // namespace Components
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_COMPONENTS_LISTWIDGET_HPP
