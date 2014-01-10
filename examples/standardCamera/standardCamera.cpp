#include "standardCamera.h"

#include <QWheelEvent>

using namespace qglviewer;

StandardCamera::StandardCamera()
{
  standard = true;
  orthoSize = 1.0;
}

float StandardCamera::zNear() const
{
  if (standard)
	return 0.001f;
  else
	return Camera::zNear();
}

float StandardCamera::zFar() const
{
  if (standard)
	return 1000.0;
  else
	return Camera::zFar();
}

void StandardCamera::changeOrthoFrustumSize(int delta)
{
  if (delta > 0)
	orthoSize *= 1.1f;
  else
	orthoSize /= 1.1f;
}

void StandardCamera::getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const
{
  if (standard)
  {
	halfHeight = orthoSize;
	halfWidth = aspectRatio() * orthoSize;
  }
  else
	Camera::getOrthoWidthHeight(halfWidth, halfHeight);
}
