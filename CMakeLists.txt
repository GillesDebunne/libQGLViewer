if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

project(libQGLViewer LANGUAGES CXX VERSION 2.9.1)
cmake_minimum_required(VERSION 3.16)

# Qt6 minimum compiler version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

# This will find the Qt files.
find_package(Qt6 COMPONENTS Core Widgets Xml OpenGL OpenGLWidgets)
if (Qt6_FOUND)
    message("Building with Qt6")
    set(QtLibs Qt::Core Qt::Widgets Qt::Xml Qt::OpenGL Qt::OpenGLWidgets)
else()
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Xml OpenGL)
    message("Building with Qt5")
    set(QtLibs Qt::Core Qt::Widgets Qt::Xml Qt::OpenGL)
endif()


find_package(OpenGL REQUIRED)

# QGLViewer target.
set(QGLViewer_SRC
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/BackFaceCullingOptimizer.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/BSPSortMethod.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/EPSExporter.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/Exporter.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/FIGExporter.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/gpc.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/NVector3.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/ParserGL.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/Primitive.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/PrimitivePositioning.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/TopologicalSortMethod.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/Vector2.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/Vector3.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/VisibilityOptimizer.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/VRender/VRender.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/camera.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/constraint.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/frame.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/keyFrameInterpolator.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/manipulatedCameraFrame.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/manipulatedFrame.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/mouseGrabber.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/qglviewer.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/quaternion.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/saveSnapshot.cpp"
    "${PROJECT_SOURCE_DIR}/QGLViewer/vec.cpp")
add_library(QGLViewer SHARED ${QGLViewer_SRC})
target_include_directories(QGLViewer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QGLViewer PRIVATE ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: animation.
set(animation_SRC
    "${PROJECT_SOURCE_DIR}/examples/animation/animation.cpp"
    "${PROJECT_SOURCE_DIR}/examples/animation/main.cpp")
add_executable(animation ${animation_SRC})
target_include_directories(animation PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(animation QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: callback.
set(callback_SRC
    "${PROJECT_SOURCE_DIR}/examples/callback/callback.cpp"
    "${PROJECT_SOURCE_DIR}/examples/callback/main.cpp")
add_executable(callback ${callback_SRC})
target_include_directories(callback PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(callback QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: cameraLight.
set(cameraLight_SRC
    "${PROJECT_SOURCE_DIR}/examples/cameraLight/cameraLight.cpp"
    "${PROJECT_SOURCE_DIR}/examples/cameraLight/main.cpp")
add_executable(cameraLight ${cameraLight_SRC})
target_include_directories(cameraLight PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(cameraLight QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: clippingPlane.
set(clippingPlane_SRC
    "${PROJECT_SOURCE_DIR}/examples/clippingPlane/clippingPlane.cpp"
    "${PROJECT_SOURCE_DIR}/examples/clippingPlane/main.cpp")
add_executable(clippingPlane ${clippingPlane_SRC})
target_include_directories(clippingPlane PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(clippingPlane QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: constrainedCamera.
set(constrainedCamera_SRC
    "${PROJECT_SOURCE_DIR}/examples/constrainedCamera/constrainedCamera.cpp"
    "${PROJECT_SOURCE_DIR}/examples/constrainedCamera/main.cpp")
add_executable(constrainedCamera ${constrainedCamera_SRC})
target_include_directories(constrainedCamera PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(constrainedCamera QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: constrainedFrame.
set(constrainedFrame_SRC
    "${PROJECT_SOURCE_DIR}/examples/constrainedFrame/constrainedFrame.cpp"
    "${PROJECT_SOURCE_DIR}/examples/constrainedFrame/main.cpp")
add_executable(constrainedFrame ${constrainedFrame_SRC})
target_include_directories(constrainedFrame PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(constrainedFrame QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: drawLight.
set(drawLight_SRC
    "${PROJECT_SOURCE_DIR}/examples/drawLight/drawLight.cpp"
    "${PROJECT_SOURCE_DIR}/examples/drawLight/main.cpp")
add_executable(drawLight ${drawLight_SRC})
target_include_directories(drawLight PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(drawLight QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: fastDraw.
set(fastDraw_SRC
    "${PROJECT_SOURCE_DIR}/examples/fastDraw/fastDraw.cpp"
    "${PROJECT_SOURCE_DIR}/examples/fastDraw/main.cpp")
add_executable(fastDraw ${fastDraw_SRC})
target_include_directories(fastDraw PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(fastDraw QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: frameTransform.
set(frameTransform_SRC
    "${PROJECT_SOURCE_DIR}/examples/frameTransform/frameTransform.cpp"
    "${PROJECT_SOURCE_DIR}/examples/frameTransform/main.cpp")
add_executable(frameTransform ${frameTransform_SRC})
target_include_directories(frameTransform PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(frameTransform QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: frustumCulling.
set(frustumCulling_SRC
    "${PROJECT_SOURCE_DIR}/examples/frustumCulling/box.cpp"
    "${PROJECT_SOURCE_DIR}/examples/frustumCulling/cullingCamera.cpp"
    "${PROJECT_SOURCE_DIR}/examples/frustumCulling/frustumCulling.cpp"
    "${PROJECT_SOURCE_DIR}/examples/frustumCulling/main.cpp")
add_executable(frustumCulling ${frustumCulling_SRC})
target_include_directories(frustumCulling PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(frustumCulling QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: interface.
set(interface_SRC
    "${PROJECT_SOURCE_DIR}/examples/interface/interface.cpp"
    "${PROJECT_SOURCE_DIR}/examples/interface/main.cpp")
add_executable(interface ${interface_SRC})
target_include_directories(interface PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU "${PROJECT_SOURCE_DIR}/examples/interface")
target_link_libraries(interface QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: keyboardAndMouse.
set(keyboardAndMouse_SRC
    "${PROJECT_SOURCE_DIR}/examples/keyboardAndMouse/keyboardAndMouse.cpp"
    "${PROJECT_SOURCE_DIR}/examples/keyboardAndMouse/main.cpp")
add_executable(keyboardAndMouse ${keyboardAndMouse_SRC})
target_include_directories(keyboardAndMouse PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(keyboardAndMouse QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: keyFrames.
set(keyFrames_SRC
    "${PROJECT_SOURCE_DIR}/examples/keyFrames/keyFrames.cpp"
    "${PROJECT_SOURCE_DIR}/examples/keyFrames/main.cpp")
add_executable(keyFrames ${keyFrames_SRC})
target_include_directories(keyFrames PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(keyFrames QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: luxo.
set(luxo_SRC
    "${PROJECT_SOURCE_DIR}/examples/luxo/luxo.cpp"
    "${PROJECT_SOURCE_DIR}/examples/luxo/main.cpp")
add_executable(luxo ${luxo_SRC})
target_include_directories(luxo PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(luxo QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: manipulatedFrame.
set(manipulatedFrame_SRC
    "${PROJECT_SOURCE_DIR}/examples/manipulatedFrame/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/manipulatedFrame/manipulatedFrame.cpp")
add_executable(manipulatedFrame ${manipulatedFrame_SRC})
target_include_directories(manipulatedFrame PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(manipulatedFrame QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: mouseGrabber.
set(mouseGrabber_SRC
    "${PROJECT_SOURCE_DIR}/examples/mouseGrabber/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/mouseGrabber/mouseGrabber.cpp")
add_executable(mouseGrabber ${mouseGrabber_SRC})
target_include_directories(mouseGrabber PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(mouseGrabber QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: multiSelect.
set(multiSelect_SRC
    "${PROJECT_SOURCE_DIR}/examples/multiSelect/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/multiSelect/manipulatedFrameSetConstraint.cpp"
    "${PROJECT_SOURCE_DIR}/examples/multiSelect/multiSelect.cpp"
    "${PROJECT_SOURCE_DIR}/examples/multiSelect/object.cpp")
add_executable(multiSelect ${multiSelect_SRC})
target_include_directories(multiSelect PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(multiSelect QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: multiView.
set(multiView_SRC
    "${PROJECT_SOURCE_DIR}/examples/multiView/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/multiView/multiView.cpp")
add_executable(multiView ${multiView_SRC})
target_include_directories(multiView PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(multiView QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: overpainting.
set(overpainting_SRC
    "${PROJECT_SOURCE_DIR}/examples/overpainting/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/overpainting/viewer.cpp")
add_executable(overpainting ${overpainting_SRC})
target_include_directories(overpainting PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(overpainting QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: screenCoordSystem.
set(screenCoordSystem_SRC
    "${PROJECT_SOURCE_DIR}/examples/screenCoordSystem/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/screenCoordSystem/screenCoordSystem.cpp")
add_executable(screenCoordSystem ${screenCoordSystem_SRC})
target_include_directories(screenCoordSystem PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(screenCoordSystem QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: select.
set(select_SRC
    "${PROJECT_SOURCE_DIR}/examples/select/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/select/select.cpp")
add_executable(select ${select_SRC})
target_include_directories(select PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(select QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: simpleViewer.
set(simpleViewer_SRC
    "${PROJECT_SOURCE_DIR}/examples/simpleViewer/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/simpleViewer/simpleViewer.cpp")
add_executable(simpleViewer ${simpleViewer_SRC})
target_include_directories(simpleViewer PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(simpleViewer QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: standardCamera.
set(standardCamera_SRC
    "${PROJECT_SOURCE_DIR}/examples/standardCamera/cameraViewer.cpp"
    "${PROJECT_SOURCE_DIR}/examples/standardCamera/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/standardCamera/standardCamera.cpp"
    "${PROJECT_SOURCE_DIR}/examples/standardCamera/viewer.cpp")
add_executable(standardCamera ${standardCamera_SRC})
target_include_directories(standardCamera PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(standardCamera QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Example: stereoViewer.
set(stereoViewer_SRC
    "${PROJECT_SOURCE_DIR}/examples/stereoViewer/main.cpp"
    "${PROJECT_SOURCE_DIR}/examples/stereoViewer/stereoViewer.cpp")
add_executable(stereoViewer ${stereoViewer_SRC})
target_include_directories(stereoViewer PRIVATE QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(stereoViewer QGLViewer ${QtLibs} OpenGL::GL OpenGL::GLU)

# Install.
include(GNUInstallDirs)
install(TARGETS QGLViewer
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
set(QGLVIEWER_INSTALL_FULL_INCLUDEDIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}/QGLViewer")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/camera.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/config.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/constraint.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/domUtils.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/frame.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/keyFrameInterpolator.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/manipulatedCameraFrame.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/manipulatedFrame.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/mouseGrabber.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/qglviewer.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/quaternion.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
install(FILES "${PROJECT_SOURCE_DIR}/QGLViewer/vec.h"
        DESTINATION "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")

# Create pkg-config file.
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "${CMAKE_INSTALL_PREFIX}")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${QGLVIEWER_INSTALL_FULL_INCLUDEDIR}")
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_URL "https://github.com/GillesDebunne/libQGLViewer")
configure_file(libQGLViewer.pc.in "${CMAKE_BINARY_DIR}/libQGLViewer.pc" @ONLY)
install(FILES "${CMAKE_BINARY_DIR}/libQGLViewer.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
