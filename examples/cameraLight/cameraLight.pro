# This example shows how to position a GL_LIGHT on the camera.

# A spot light position and direction are set so that they always match the
# camera's current setup. 

# This is done using the <code>camera->position()</code> and <code>camera->viewDirection()</code>
# methods which return values expressed in the world coordinates system.

TEMPLATE = app
TARGET   = cameraLight

HEADERS  = cameraLight.h
SOURCES  = cameraLight.cpp main.cpp

include( ../examples.pri )