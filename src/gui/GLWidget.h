#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <qopenglwidget.h>
#include "../core/engine.h"
#include "../core/camera.h"

class Camera;

class GLWidget : public QOpenGLWidget {
	Q_OBJECT
public:
	GLWidget(QWidget *parent = nullptr);

public slots:
	void openScene();
	void closeScene();
	void centerScene();

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
	void resizeEvent(QResizeEvent *e);

private:
	std::unique_ptr<Engine> m_engine;
	Camera *m_camera;

	bool m_wireframe;
};

#endif // !GLWIDGET_H
