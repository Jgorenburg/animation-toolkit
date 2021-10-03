#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"

using namespace Eigen;

//--------------------------------
// Hermite
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1, 0, 0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const
    {
        float t = u;
        float t0 = 1.0f - t;

        int loc = 2 * segment;
        glm::vec3 p0 = mCtrlPoints[loc];
        glm::vec3 p0s = mCtrlPoints[loc + 1];
        glm::vec3 p1 = mCtrlPoints[loc + 2];
        glm::vec3 p1s = mCtrlPoints[loc + 3];

        float h0 = (1.0f - 3.0f * pow(t, 2.0f) + 2.0f * pow(t, 3.0f));
        float h1 = t * pow(t0, 2.0f);
        float h2 = pow(t, 3.0f) - pow(t, 2.0f);
        float h3 = 3.0f * pow(t, 2.0f) - 2.0f * pow(t, 3.0f);

        return h0 * p0 + h1 * p0s + h2 * p1s + h3 * p1;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
    {
        mCtrlPoints.clear();

        int size = keys.size();
        points.resize(size, 3);
        A.resize(size, size);
        A.setIdentity();

        if (isClamped())
        {
            for (int i = 0; i < 3; i++)
            {
                points(0, i) = mClampDir[i];
                points(size - 1, i) = mClampDir[i];
            }
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                points(0, i) = 3.0f * (keys[1][i] - keys[0][i]);
                points(size - 1, i) = 3.0f * (keys[size - 1][i] - keys[size - 2][i]);
            }
            A(0, 0) = 2.0f;
            A(0, 1) = 1.0f;
            A(size - 1, size - 1) = 2.0f;
            A(size - 1, size - 2) = 1.0f;
        }

        for (int i = 1; i < size - 1; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                points(i, j) = 3.0f * (keys[i + 1][j] - keys[i - 1][j]);
            }
            A(i, i - 1) = 1.0f;
            A(i, i) = 4.0f;
            A(i, i + 1) = 1.0f;
        }

        Matrix<float, Dynamic, 3> slopes = A.inverse() * points;
        for (int i = 0; i < size; i++)
        {
            mCtrlPoints.push_back(keys[i]);
            mCtrlPoints.push_back(glm::vec3(slopes(i, 0), slopes(i, 1), slopes(i, 2)));
        }
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3 &v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
    Matrix<float, Dynamic, 3> points;
    MatrixXf A, Ainverse;
};

#endif
