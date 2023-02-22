#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include "SType.h"

class Model{
private:
    int mk;
    QString filePath;
    Double3 scale;
    Int3 rotate;
    Double3 move;

public:
    int getMk() const {
        return mk;
    }
    void setMk(int mk) {
        Model::mk = mk;
    }

    const QString &getFilePath() const {
        return filePath;
    }
    void setFilePath(const QString &filePath) {
        Model::filePath = filePath;
    }

    const Double3 &getScale() const {
        return scale;
    }
    void setScale(const Double3 &scale) {
        Model::scale = scale;
    }

    const Int3 &getRotate() const {
        return rotate;
    }
    void setRotate(const Int3 &rotate) {
        Model::rotate = rotate;
    }

    const Double3 &getMove() const {
        return move;
    }
    void setMove(const Double3 &move) {
        Model::move = move;
    }
};

#endif // MODEL_H
