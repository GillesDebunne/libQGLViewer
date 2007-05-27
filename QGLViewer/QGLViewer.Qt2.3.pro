#		l i b Q G L V i e w e r   Q t  - 2 . 3
#	C o m p i l a t i o n    c o n f i g u r a t i o n

# Use this file instead of QGLViewer.pro to compile libQGLViewer with Qt 2.3 on windows.

# Use the MS Visual "Generate Qt Project" icon to create a workspace out of this file.
# Select the generated .dsp file using File / Open workspace and compile the project.

# See doc/installWindows.html for compilation instructions.

#		--  M a i n   v a r i a b l e s  --
TEMPLATE = lib
CONFIG  *= qt opengl warn_on release thread dll
TARGET   = QGLViewer
VERSION  = 2.2.5

HEADERS  = qglviewer.h \
	   camera.h \
	   manipulatedFrame.h \
	   manipulatedCameraFrame.h \
	   frame.h \
	   constraint.h \
	   keyFrameInterpolator.h \
	   mouseGrabber.h \
	   keySequence.h \
	   quaternion.h \
	   vec.h \
	   domUtils.h \
	   config.h

SOURCES  = qglviewer.cpp \
	   camera.cpp \
	   manipulatedFrame.cpp \
	   manipulatedCameraFrame.cpp \
	   frame.cpp \
	   saveSnapshot.cpp \
	   constraint.cpp \
	   keySequence.cpp \
	   keyFrameInterpolator.cpp \
	   mouseGrabber.cpp \
	   quaternion.cpp \
	   vec.cpp

# Use the DLL version of Qt
DEFINES *= QT_DLL QT_THREAD_SUPPORT

# Creates a lib and a dll instead of a static lib
DEFINES *= CREATE_QGLVIEWER_DLL

INTERFACES *= ImageInterface.Qt2.ui

# Uncomment this line and remove the rest of the file if you encounter
# compilation problems with the vectorial rendering code.
# DEFINES *= NO_VECTORIAL_RENDER

INTERFACES *= VRenderInterface.Qt2.ui

SOURCES  *= \
        VRender/BackFaceCullingOptimizer.cpp \
        VRender/BSPSortMethod.cpp \
        VRender/EPSExporter.cpp \
        VRender/Exporter.cpp \
        VRender/FIGExporter.cpp \
        VRender/gpc.cpp \
        VRender/ParserGL.cpp \
        VRender/Primitive.cpp \
        VRender/PrimitivePositioning.cpp \
        VRender/TopologicalSortMethod.cpp \
        VRender/VisibilityOptimizer.cpp \
        VRender/Vector2.cpp \
        VRender/Vector3.cpp \
        VRender/NVector3.cpp \
        VRender/VRender.cpp

HEADERS  *= \
        VRender/AxisAlignedBox.h \
        VRender/Exporter.h \
        VRender/gpc.h \
        VRender/NVector3.h \
        VRender/Optimizer.h \
        VRender/ParserGL.h \
        VRender/Primitive.h \
        VRender/PrimitivePositioning.h \
        VRender/SortMethod.h \
        VRender/Types.h \
        VRender/Vector2.h \
        VRender/Vector3.h \
        VRender/VRender.h

