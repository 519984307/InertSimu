#ifndef FillBox_H
#define FillBox_H
#include <QString>
#include "SType.h"

class FillBox {
private:
    int mk;
    QString name;
    Double3 seed;
    QString modefill = "void";
    Double3 position;
    Double3 boxsize;
public:
    FillBox() {}

    int getMk() const {
        return mk;
    }
    void setMk(int mk) {
        FillBox::mk = mk;
    }

    const QString &getName() const {
        return name;
    }
    void setName(const QString &name) {
        FillBox::name = name;
    }

    const Double3 &getSeed() const {
        return seed;
    }
    void setSeed(const Double3 &seed) {
        FillBox::seed = seed;
    }

    const QString &getModefill() const {
        return modefill;
    }
    void setModefill(const QString &modefill) {
        FillBox::modefill = modefill;
    }

    const Double3 &getPosition() const {
        return position;
    }
    void setPosition(const Double3 &position) {
        FillBox::position = position;
    }

    const Double3 &getBoxsize() const {
        return boxsize;
    }
    void setBoxsize(const Double3 &boxsize) {
        FillBox::boxsize = boxsize;
    }
};

#endif // FillBox_H
