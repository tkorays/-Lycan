#ifndef __LYCAN_IMGPROC_H__
#define __LYCAN_IMGPROC_H__

#include "lycan.h"
#include "graphic.h"
#include <math.h>

namespace lycan { namespace imgproc {

template <typename T>
void fill(Matrix<T>& mat, Rect<int> rect, T d) {
    for (int i = lycan::max<int>(0, std::move(rect.top)); i < lycan::min<int>(mat.rows(), std::move(rect.bottom)); i++) {
        lycan::Vector<T> row = mat[i];
        for (int j = lycan::max<int>(0, std::move(rect.left)); j < lycan::min<int>(mat.cols(), std::move(rect.right)); j++) {
            row[j] = d;
        }
    }
}

template <typename T>
Matrix<T> move(Matrix<T>& mat, Position<int> pos, T fillValue) {
    Matrix<T> matRet(mat.cols(), mat.rows());
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = 0; j < mat.cols(); j++) {
            if (i - pos.y >= 0 && i - pos.y < mat.rows() && j - pos.x >= 0 && j - pos.x < mat.cols()) {
                matRet[i][j] = mat[i - pos.y][j - pos.x];
            }
            else {
                matRet[i][j] = fillValue;
            }
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> mirrorH(Matrix<T>& mat) {
    Matrix<T> matRet(mat.cols(), mat.rows());
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = 0; j < mat.cols(); j++) {
            matRet[i][j] = mat[i][mat.cols() - 1 - j];
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> mirrorV(Matrix<T>& mat) {
    Matrix<T> matRet(mat.cols(), mat.rows());
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = 0; j < mat.cols(); j++) {
            matRet[i][j] = mat[mat.rows() - 1 - i][j];
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> transpose(Matrix<T>& mat) {
    Matrix<T> matRet(mat.rows(), mat.cols());
    for (int i = 0; i < matRet.rows(); i++) {
        for (int j = 0; j < matRet.cols(); j++) {
            matRet[i][j] = mat[j][i];
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> scale(Matrix<T>& mat, float scaleX, float scaleY) {
    Matrix<T> matRet(static_cast<int>(mat.cols()*scaleX), static_cast<int>(mat.rows()*scaleY));
    if (scaleX == 0.0 || scaleY == 0.0) return matRet;

    for (int i = 0; i < matRet.rows(); i++) {
        for (int j = 0; j < matRet.cols(); j++) {
            int u = static_cast<int>(i / scaleY + 0.5);
            int v = static_cast<int>(j / scaleX + 0.5);
            matRet[i][j] = mat[u][v];
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> rotate(Matrix<T>&& mat, float angle, T fillValue) {
    const float PI = 3.1415926f;
    Matrix<T> matRet(mat.cols(), mat.rows());
    for (int i = 0; i < matRet.rows(); i++) {
        for (int j = 0; j < matRet.cols(); j++) {
            int u = static_cast<int>(i*cos(angle*PI / 180) + j*sin(angle*PI / 180) + 0.5);
            int v = static_cast<int>(j*cos(angle*PI / 180) - i*sin(angle*PI / 180) + 0.5);
            if (u >= 0 && u < mat.rows() && v >= 0 && v < mat.cols()) {
                matRet[i][j] = mat[u][v];
            }
            else {
                matRet[i][j] = fillValue;
            }
        }
    }
    return matRet;
}

template <typename T>
Matrix<T> guassBlur(Matrix<T>& mat) {
    Matrix<T> matRet(mat.cols(), mat.rows());
    return matRet;
}


} }

#endif /* __LYCAN_IMGPROC_H__ */
