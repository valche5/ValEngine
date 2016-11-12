#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <qopenglwidget.h>

#include <core/Types.h>
#include <glm/glm.hpp>

class Camera;

class GLWidget : public QOpenGLWidget {
	Q_OBJECT
public:
	GLWidget(QWidget *parent = nullptr);

public slots:
	void openScene();
	void closeScene();
	void centerScene(const glm::vec3 &dir = glm::vec3(0));

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void focusOutEvent(QFocusEvent *event);

private:
	EnginePtr m_engine;
	CameraPtr m_camera;

	bool m_wireframe;
};

#endif // !GLWIDGET_H
