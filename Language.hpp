#ifndef LANGUAGE_H_
#define LANGUAGE_H_


#include <QString>

namespace Language
{
    enum Lang
    {
        None,
        CPP,
        C,
        Python,
        Java
    };

    QString toString(Lang language);
}

#endif // LANGUAGE_H_
