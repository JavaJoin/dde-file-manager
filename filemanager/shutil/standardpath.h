#ifndef STANDARDPATH_H
#define STANDARDPATH_H

#include <QString>

class StandardPath
{
public:
    StandardPath();
    ~StandardPath();

    static QString getHomePath();
    static QString getCachePath();
    static QString getTrashPath();
};

#endif // STANDARDPATH_H