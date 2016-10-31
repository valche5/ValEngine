#include "GLWidget.h"

#include <qcoreapplication.h>
#include <QKeyevent>
#include <QTimer>

GLWidget::GLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	setMouseTracking(false);
	setFocusPolicy(Qt::StrongFocus);

	QTimer *t = new QTimer(this);
	connect(t, SIGNAL(timeout()), this, SLOT(update()));
	t->start(0);
};

void GLWidget::initializeGL() {
	QOpenGLWidget::initializeGL();

	m_engine = std::make_unique<Engine>();
	m_engine->init();
	m_camera = m_engine->camera();

	m_wireframe = false;
}

void GLWidget::resizeGL(int w, int h) {
	m_engine->resize(w, h);
}

void GLWidget::paintGL() {
	QOpenGLWidget::paintGL();

	m_engine->paint();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
	//if (e->isAutoRepeat())
	//	return QOpenGLWidget::keyPressEvent(e);

	switch (e->key())
	{
	case Qt::Key_Escape:
		QCoreApplication::instance()->quit();
		m_engine->clean();
		break;
		//Wireframe
	case Qt::Key_W:
		m_wireframe = !m_wireframe;
		m_engine->setWireframe(m_wireframe);
		break;
		//Reload Shaders
	case Qt::Key_R:
		m_engine->reloadShaders();
		break;
		//Movement
	case Qt::Key_Up:
	case Qt::Key_Z:
		m_camera->processKeyPress(Camera::KEY_FORWARD);
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
		m_camera->processKeyPress(Camera::KEY_BACKWARD);
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		m_camera->processKeyPress(Camera::KEY_RIGHT);
		break;
	case Qt::Key_Left:
	case Qt::Key_Q:
		m_camera->processKeyPress(Camera::KEY_LEFT);
		break;
	case Qt::Key_E:
		m_camera->processKeyPress(Camera::KEY_UP);
		break;
	case Qt::Key_F:
		m_camera->processKeyPress(Camera::KEY_DOWN);
		break;
	default:
		QOpenGLWidget::keyPressEvent(e);
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent *e) {
	switch (e->key())
	{
	case Qt::Key_Up:
	case Qt::Key_Z:
		m_camera->processKeyRelease(Camera::KEY_FORWARD);
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
		m_camera->processKeyRelease(Camera::KEY_BACKWARD);
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		m_camera->processKeyRelease(Camera::KEY_RIGHT);
		break;
	case Qt::Key_Left:
	case Qt::Key_Q:
		m_camera->processKeyRelease(Camera::KEY_LEFT);
		break;
	case Qt::Key_E:
		m_camera->processKeyRelease(Camera::KEY_UP);
		break;
	case Qt::Key_F:
		m_camera->processKeyRelease(Camera::KEY_DOWN);
		break;
	//case Qt::Key_F11:
	//	isFullScreen() ? showNormal() : showFullScreen();
	//	break;
	default:
		QOpenGLWidget::keyReleaseEvent(e);
	}
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
	if (e->button() == Qt::LeftButton)
	{
		m_camera->processMousePress(e->x(), e->y());
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	if (e->button() == Qt::LeftButton)
	{
		m_camera->processMouseRelease();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons() == Qt::LeftButton)
	{
		m_camera->processMouseMove(e->x(), e->y());
	}
}

void GLWidget::wheelEvent(QWheelEvent *e) {
	m_camera->ProcessMouseScroll(e->delta());
}

void GLWidget::resizeEvent(QResizeEvent *e) {

}

