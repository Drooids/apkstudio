#include "listwidget.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

QModelIndex ListWidget::index(QListWidgetItem *item)
{
    return indexFromItem(item);
}

QListWidgetItem *ListWidget::item(const QModelIndex &index)
{
    return itemFromIndex(index);
}

ListWidget::~ListWidget()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
