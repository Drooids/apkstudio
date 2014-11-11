#include "ui.hpp"

namespace VPZ {
namespace APKStudio {
namespace Helpers {

void UI::resizeButton(QPushButton *button)
{
    QSize size = button->fontMetrics().size(Qt::TextShowMnemonic, button->text());
    size.setWidth(size.width() + 12);
    QStyleOptionButton option;
    option.initFrom(button);
    option.rect.setSize(size);
    button->setMaximumSize(button->style()->sizeFromContents(QStyle::CT_PushButton, &option, size, button));
}

} // namespace Helpers
} // namespace APKStudio
} // namespace VPZ
