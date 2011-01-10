/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

#ifndef INCLUDE_Moments_h
#define INCLUDE_Moments_h

// System includes
#include <math.h>

struct MomentStats
{
    int   area;
    // X is 0 on the left side of the image and increases to the right
    // Y is 0 on the top of the image and increases to the bottom
    float centroidX, centroidY;
    // angle is 0 to PI, in radians.
    // 0 points to the right (positive X)
    // PI/2 points downward (positive Y)
    float angle;
    float majorDiameter;
    float minorDiameter;
    MomentStats() :
            area(0), centroidX(0), centroidY(0),
            angle(0), majorDiameter(0), minorDiameter(0) {}
};

// Image size is 352x278
// Full-screen blob area is 97856
// Full-screen centroid is 176,139
// sumX, sumY is then 17222656, 13601984; well within 32 bits
struct Moments
{
    // Skip major/minor axis computation when this is false
    static bool computeAxes;

    int area; // number of pixels
    int sumX; // sum of pixel x coords
    int sumY; // sum of pixel y coords
    // XX, XY, YY used for major/minor axis calculation
    long long sumXX; // sum of x^2 for each pixel
    long long sumYY; // sum of y^2 for each pixel
    long long sumXY; // sum of x*y for each pixel

    void Add(const Moments &moments)
    {
        area += moments.area;
        sumX += moments.sumX;
        sumY += moments.sumY;
        if (computeAxes)
        {
            sumXX += moments.sumXX;
            sumYY += moments.sumYY;
            sumXY += moments.sumXY;
        }
    }

    void GetStats(MomentStats &stats)
    {
        stats.area= area;
        stats.centroidX = (float)sumX / (float)area;
        stats.centroidY = (float)sumY / (float)area;

        if (computeAxes)
        {
            // Find the eigenvalues and eigenvectors for the 2x2 covariance matrix:
            //
            // | sum((x-|x|)^2)        sum((x-|x|)*(y-|y|)) |
            // | sum((x-|x|)*(y-|y|))  sum((y-|y|)^2)       |

            // Values= 0.5 * ((sumXX+sumYY) +- sqrt((sumXX+sumYY)^2-4(sumXXsumYY-sumXY^2)))
            // .5 * (xx+yy) +- sqrt(xx^2+2xxyy+yy^2-4xxyy+4xy^2)
            // .5 * (xx+yy) +- sqrt(xx^2-2xxyy+yy^2 + 4xy^2)

            // sum((x-|x|)^2) =
            // sum(x^2) - 2sum(x|x|) + sum(|x|^2) =
            // sum(x^2) - 2|x|sum(x) + n|x|^2 =
            // sumXX - 2*centroidX*sumX + centroidX*sumX =
            // sumXX - centroidX*sumX

            // sum((x-|x|)*(y-|y|))=
            // sum(xy) - sum(x|y|) - sum(y|x|) + sum(|x||y|) =
            // sum(xy) - |y|sum(x) - |x|sum(y) + n|x||y| =
            // sumXY - centroidY*sumX - centroidX*sumY + sumX * centroidY =
            // sumXY - centroidX*sumY

            float xx= sumXX - stats.centroidX*sumX;
            float xyTimes2= 2*(sumXY - stats.centroidX*sumY);
            float yy= sumYY - stats.centroidY*sumY;
            float xxMinusyy = xx-yy;
            float xxPlusyy = xx+yy;
            float sq = sqrt(xxMinusyy * xxMinusyy + xyTimes2*xyTimes2);
            float eigMaxTimes2= xxPlusyy+sq;
            float eigMinTimes2= xxPlusyy-sq;
            stats.angle= 0.5*atan2(xyTimes2, xxMinusyy);
            //float aspect= sqrt(eigMin/eigMax);
            //stats.majorDiameter= sqrt(area/aspect);
            //stats.minorDiameter= sqrt(area*aspect);
            //
            // sqrt(eigenvalue/area) is the standard deviation
            // Draw the ellipse with radius of twice the standard deviation,
            // which is a diameter of 4 times, which is 16x inside the sqrt

            stats.majorDiameter= sqrt(8.0*eigMaxTimes2/area);
            stats.minorDiameter= sqrt(8.0*eigMinTimes2/area);
        }
    }

    void Reset()
    {
        area= sumX= sumY= sumXX= sumYY= sumXY= 0;
    }

    bool operator==(const Moments &rhs) const
    {
        if (area != rhs.area) return 0;
        if (sumX != rhs.sumX) return 0;
        if (sumY != rhs.sumY) return 0;
        if (computeAxes)
        {
            if (sumXX != rhs.sumXX) return 0;
            if (sumYY != rhs.sumYY) return 0;
            if (sumXY != rhs.sumXY) return 0;
        }
        return 1;
    }
};

#endif
