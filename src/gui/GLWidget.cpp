#include "GLWidget.h"

#include <iostream>

#include <qcoreapplication.h>
#include <QKeyevent>
#include <QTimer>
#include <QFileDialog>
#include <QSettings>

GLWidget::GLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	setMouseTracking(false);
	setFocusPolicy(Qt::StrongFocus);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setOption(QSurfaceFormat::DebugContext);
	setFormat(format);

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
		if (e->modifiers() & Qt::ControlModifier) {
			m_wireframe = !m_wireframe;
			makeCurrent();
			m_engine->setWireframe(m_wireframe);
			doneCurrent();
		}
		break;
		//Reload Shaders
	case Qt::Key_R:
		if (e->modifiers() & Qt::ControlModifier) {
			m_engine->reloadShaders();
		}
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
	m_camera->processMouseScroll(e->delta());
}

void GLWidget::openScene() {
	QSettings settings("ValCompany", "ValRenderer");
	QString path;
	if (settings.contains("scenePath"))
		path = settings.value("scenePath").toString();
	else
		path = "./";
	
	
	QString filename = QFileDialog::getOpenFileName(this, "Ouvrir une scène", path, QString("Wavefront Object (*.obj);;"
		"Collada  (*.dae *.xml);;"
		"Blender (*.blend);;"
		"Biovision BVH (*.bvh);;"
		"3D Studio Max 3DS (*.3ds);;"
		"3D Studio Max ASE (*.ase);;"
		"Stanford Polygon Library (*.ply);;"
		"AutoCAD DXF (*.dxf);;"
		"IFC - STEP, Industry Foundation Classes (*.ifc);;"
		"Neutral File Format (*.nff);;"
		"Sense8 WorldToolkit (*.nff);;"
		"Valve Model (*.smd, *.vta);;"
		"Quake I (*.mdl);;"
		"Quake II (*.md2);;"
		"Quake III (*.md3);;"
		"Quake 3 BSP (*.pk3);;"
		"RtCW (*.mdc);;"
		"Doom 3  (*.md5mesh *.md5anim *.md5camera);;"
		"DirectX X (*.x);;"
		"Quick3D (*.q3o; q3s);;"
		"Raw Triangles(.raw);;"
		"AC3D (*.ac);;"
		"Stereolithography (*.stl);;"
		"Autodesk DXF (*.dxf);;"
		"Irrlicht Mesh (*.irrmesh; *.xml);;"
		"Irrlicht Scene (*.irr; *.xml);;"
		"Object File Format (*.off);;"
		"Terragen Terrain (*.ter);;"
		"3D GameStudio Model (*.mdl);;"
		"3D GameStudio Terrain (*.hmp);;"
		"Ogre (*.mesh.xml, *.skeleton.xml *.material);;"
		"Milkshape 3D (*.ms3d);;"
		"LightWave Model (*.lwo);;"
		"LightWave Scene (*.lws);;"
		"Modo Model (*.lxo);;"
		"CharacterStudio Motion (*.csm);;"
		"Stanford Ply (*.ply);;"
		"TrueSpace (*.cob *.scn);;"
		"XGL (*.xgl *.zgl)"));

	QFileInfo file(filename);
	settings.setValue("scenePath", file.absolutePath());

	makeCurrent();
	m_engine->loadScene(filename.toStdString());
	m_camera = m_engine->camera();
	doneCurrent();
}

void GLWidget::closeScene() {
	makeCurrent();
	m_engine->loadDefaultScene();
	m_camera = m_engine->camera();
	doneCurrent();
}

void GLWidget::centerScene(const glm::vec3 &dir) {
	m_engine->centerScene(dir);
}
