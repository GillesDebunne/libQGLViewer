#ifndef QGLVIEWER_MANIPULATED_CAMERA_FRAME_H
#define QGLVIEWER_MANIPULATED_CAMERA_FRAME_H

#include "manipulatedFrame.h"

namespace qglviewer {
/*! \brief The ManipulatedCameraFrame class represents a ManipulatedFrame with Camera specific mouse bindings.
  \class ManipulatedCameraFrame manipulatedCameraFrame.h QGLViewer/manipulatedCameraFrame.h

  A ManipulatedCameraFrame is a specialization of a ManipulatedFrame, designed to be set as the
  Camera::frame(). Mouse motions are basically interpreted in a negated way: when the mouse goes to
  the right, the ManipulatedFrame translation goes to the right, while the ManipulatedCameraFrame
  has to go to the \e left, so that the \e scene seems to move to the right.

  A ManipulatedCameraFrame rotates around its pivotPoint(), which corresponds to the
  associated Camera::pivotPoint().

  A ManipulatedCameraFrame can also "fly" in the scene. It basically moves forward, and turns
  according to the mouse motion. See flySpeed(), sceneUpVector() and the QGLViewer::MOVE_FORWARD and
  QGLViewer::MOVE_BACKWARD QGLViewer::MouseAction.

  See the <a href="../mouse.html">mouse page</a> for a description of the possible actions that can
  be performed using the mouse and their bindings.
  \nosubgrouping */
class QGLVIEWER_EXPORT ManipulatedCameraFrame : public ManipulatedFrame
{
#ifndef DOXYGEN
	friend class Camera;
	friend class ::QGLViewer;
#endif

	Q_OBJECT

public:
	ManipulatedCameraFrame();
	/*! Virtual destructor. Empty. */
	virtual ~ManipulatedCameraFrame() {}

	ManipulatedCameraFrame(const ManipulatedCameraFrame& mcf);
	ManipulatedCameraFrame& operator=(const ManipulatedCameraFrame& mcf);

	/*! @name Pivot point */
	//@{
public:
	/*! Returns the point the ManipulatedCameraFrame pivot point, around which the camera rotates.

	It is defined in the world coordinate system. Default value is (0,0,0).

	When the ManipulatedCameraFrame is associated to a Camera, Camera::pivotPoint() also
	returns this value. This point can interactively be changed using the mouse (see
	Camera::setPivotPointFromPixel() and QGLViewer::RAP_FROM_PIXEL and QGLViewer::RAP_IS_CENTER
	in the <a href="../mouse.html">mouse page</a>). */
	Vec pivotPoint() const { return pivotPoint_; }
	/*! Sets the pivotPoint(), defined in the world coordinate system. */
	void setPivotPoint(const Vec& point) { pivotPoint_ = point; }

	/*! Returns \c true when the camera's rotation is constrained around the sceneUpVector(),
		and \c false otherwise, when the rotation is completely free (default).

		In free mode, the camera can be arbitrarily rotated in the scene, along its three
		axis, thus possibly leading to any arbitrary rotation.

		When you setRotatesAroundUpVector() to \c true, the sceneUpVector() defines a
		'vertical' direction around which the camera rotates. The camera can rotate left
		or right, around this axis. It can also be moved up or down to show the top and
		bottom views of the scene. As a result, the sceneUpVector() will always stay vertical
		in the scene. An other way to say it is that, in this mode, the horizon is preserved
		and stays projected along the camera's horizontal axis.

		Note that setting this value to \c true when the sceneUpVector() is not already
		vertically projected on screen may limit the possible movement of the camera.
		Use Camera::setUpVector() to define the sceneUpVector() before calling this method
		to ensure this does not happen.
	*/
	bool rotatesAroundUpVector() const { return rotatesAroundUpVector_; }
	/*! Sets the value of rotatesAroundUpVector().

	   Default value is false (free rotation).  */
	void setRotatesAroundUpVector(bool constrained) { rotatesAroundUpVector_ = constrained; }

#ifndef DOXYGEN
	Vec revolveAroundPoint() const { qWarning("revolveAroundPoint() is deprecated, use pivotPoint() instead"); return  pivotPoint(); }
	void setRevolveArountPoint(const Vec& point) { qWarning("setRevolveAroundPoint() is deprecated, use setPivotPoint() instead"); setPivotPoint(point); }
#endif
	//@}

	/*! @name Fly parameters */
	//@{
public Q_SLOTS:
	/*! Sets the flySpeed(), defined in OpenGL units.

	Default value is 0.0, but it is modified according to the QGLViewer::sceneRadius() when the
	ManipulatedCameraFrame is set as the Camera::frame(). */
	void setFlySpeed(float speed) { flySpeed_ = speed; }

	/*! Sets the sceneUpVector(), defined in the world coordinate system.

	Default value is (0,1,0), but it is updated by the Camera when this object is set as its Camera::frame().
	Using Camera::setUpVector() instead is probably a better solution. */
	void setSceneUpVector(const Vec& up) { sceneUpVector_ = up; }

public:
	/*! Returns the fly speed, expressed in OpenGL units.

	It corresponds to the incremental displacement that is periodically applied to the
	ManipulatedCameraFrame position when a QGLViewer::MOVE_FORWARD or QGLViewer::MOVE_BACKWARD
	QGLViewer::MouseAction is proceeded.

	\attention When the ManipulatedCameraFrame is set as the Camera::frame(), this value is set
	according to the QGLViewer::sceneRadius() by QGLViewer::setSceneRadius(). */
	float flySpeed() const { return flySpeed_; }

	/*! Returns the up vector of the scene, expressed in the world coordinate system.

	In 'fly mode' (corresponding to the QGLViewer::MOVE_FORWARD and QGLViewer::MOVE_BACKWARD
	QGLViewer::MouseAction bindings), horizontal displacements of the mouse rotate
	the ManipulatedCameraFrame around this vector. Vertical displacements rotate always around the
	Camera \c X axis.

	This value is also used when setRotationIsConstrained() is set to \c true to define the up vector
	(and incidentally the 'horizon' plane) around which the camera will rotate.

	Default value is (0,1,0), but it is updated by the Camera when this object is set as its Camera::frame().
	Camera::setOrientation() and Camera::setUpVector()) direclty modify this value and should be used
	instead. */
	Vec sceneUpVector() const { return sceneUpVector_; }

#ifndef DOXYGEN
	Vec flyUpVector() const;
	void setFlyUpVector(const Vec& up);
#endif
	//@}

	/*! @name Mouse event handlers */
	//@{
protected:
	virtual void mouseReleaseEvent(QMouseEvent* const event, Camera* const camera);
	virtual void mouseMoveEvent   (QMouseEvent* const event, Camera* const camera);
	virtual void wheelEvent       (QWheelEvent* const event, Camera* const camera);
	//@}

	/*! @name Spinning */
	//@{
protected Q_SLOTS:
	virtual void spin();
	//@}

	/*! @name XML representation */
	//@{
public:
	virtual QDomElement domElement(const QString& name, QDomDocument& document) const;
public Q_SLOTS:
	virtual void initFromDOMElement(const QDomElement& element);
	//@}

#ifndef DOXYGEN
protected:
	virtual void startAction(int ma, bool withConstraint=true); // int is really a QGLViewer::MouseAction
#endif

private Q_SLOTS:
	virtual void flyUpdate();

private:
	void updateSceneUpVector();
	Quaternion turnQuaternion(int x, const Camera* const camera);
	Quaternion pitchYawQuaternion(int x, int y, const Camera* const camera);

private:
	// Fly mode data
	float flySpeed_;
	float driveSpeed_;
	Vec sceneUpVector_;
	QTimer flyTimer_;

	bool rotatesAroundUpVector_;
	// Inverse the direction of an horizontal mouse motion. Depends on the projected
	// screen orientation of the vertical axis when the mouse button is pressed.
	bool constrainedRotationIsReversed_;

	Vec pivotPoint_;
};

} // namespace qglviewer

#endif // QGLVIEWER_MANIPULATED_CAMERA_FRAME_H
