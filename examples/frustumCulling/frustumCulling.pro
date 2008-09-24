# Frustum culling using <code>getFrustumPlanesCoefficients</code>.

# A hierarchical octree structure is clipped against a camera's frustum clipping planes, obtained
# using <code>getFrustumPlanesCoefficients</code>. A second viewer displays an external view of the
# scene that exhibits the clipping (using <code>drawCamera()</code> to display the frustum).

# This frustum culling implementation is quite naive. Many optimisation techniques are available in
# the litterature.

TEMPLATE = app
TARGET   = frustumCulling

HEADERS  = frustumCulling.h cullingCamera.h box.h
SOURCES  = frustumCulling.cpp cullingCamera.cpp box.cpp main.cpp

include( ../examples.pri )
