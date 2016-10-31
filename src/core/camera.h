#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  3.0f;
const float SENSITIVTY =  0.05f;
const float ZOOM       =  45.0f;

const float invsqrt2 = 0.7071;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    typedef enum {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        FORWARDLEFT,
        FORWARDRIGHT,
        BACKWARDLEFT,
        BACKWARDRIGHT,
        UP,
        DOWN,
        NONE
    } Direction;

    typedef enum {
        KEY_FORWARD,
        KEY_BACKWARD,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
    } Key;

    // Camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    // Eular Angles
    float m_yaw;
    float m_pitch;
    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
    Direction m_direction;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    void update(float dt);

    void processKeyPress(Key key);
    void processKeyRelease(Key key);
	void processMousePress(int mouseX, int mouseY);
	void processMouseRelease();
	void processMouseMove(int mouseX, int mouseY);

	void setMouseOffsetBufferSize(size_t size);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();

	//Mouse offset buffer
	void initOffsetBuffer();
	void updateOffsetBuffer();
	glm::vec2 getSmoothMouseOffset();

	int m_bufferSize = 5;
	std::vector<glm::vec2> m_offsetBuffer;
    int m_offsetBufferEnd;

	glm::vec2 m_mouseLastPos, m_mouseOffset;
};

#endif // CAMERA_H

