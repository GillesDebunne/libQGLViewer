#include "cullingCamera.h"

using namespace qglviewer;

float CullingCamera::distanceToFrustumPlane(int index, const Vec &pos) const {
  return pos * Vec(planeCoefficients[index]) - planeCoefficients[index][3];
}

bool CullingCamera::sphereIsVisible(const Vec &center, float radius) const {
  for (int i = 0; i < 6; ++i)
    if (distanceToFrustumPlane(i, center) > radius)
      return false;
  return true;
}

bool CullingCamera::aaBoxIsVisible(const Vec &p1, const Vec &p2,
                                   bool *entirely) const {
  bool allInForAllPlanes = true;
  for (int i = 0; i < 6; ++i) {
    bool allOut = true;
    for (unsigned int c = 0; c < 8; ++c) {
      const Vec pos((c & 4) ? p1.x : p2.x, (c & 2) ? p1.y : p2.y,
                    (c & 1) ? p1.z : p2.z);
      if (distanceToFrustumPlane(i, pos) > 0.0)
        allInForAllPlanes = false;
      else
        allOut = false;
    }

    // The eight points are on the outside side of this plane
    if (allOut)
      return false;
  }

  if (entirely)
    // Entirely visible : the eight points are on the inside side of the 6
    // planes
    *entirely = allInForAllPlanes;

  // Too conservative, but tangent cases are too expensive to detect
  return true;
}
