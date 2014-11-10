#include "text.hpp"

namespace VPZ {
namespace APKStudio {
namespace Helpers {

QString Text::capitalize(const QString &text, bool strict)
{
    QString temporary = text;
    if (strict)
        temporary = text.toLower();
    temporary[0] = temporary[0].toUpper();
    return temporary;
}

QString Text::random(const int length)
{
   QString acceptable("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
   QString random;
   for (int i = 0; i < length; ++i)
       random += acceptable.at(qrand() % acceptable.length());
   return random;
}

QString Text::translate(const char *context, const char *key)
{
    return QApplication::translate(context, key);
}

} // namespace Helpers
} // namespace APKStudio
} // namespace VPZ
