TEMPLATE      = subdirs
SUBDIRS       = \
    animation \
    callback \
    cameraLight \
    clippingPlane \
    constrainedCamera \
    constrainedFrame \
    drawLight \
    fastDraw \
    frameTransform \
    frustumCulling \
    interface \
    keyboardAndMouse \
    keyFrames \
    luxo \
    manipulatedFrame \
    mouseGrabber \
    multiSelect \
    multiView \
    screenCoordSystem \
    select \
    simpleViewer \
    standardCamera

!equals (QT_MAJOR_VERSION, 6) {
    SUBDIRS *= overpainting stereoViewer
}
