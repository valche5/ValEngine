#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM), m_direction(NONE)
{
    m_offsetBufferEnd = 0;
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
	initOffsetBuffer();
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
	initOffsetBuffer();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::update(float dt)
{
    float velocity = m_movementSpeed * dt;

    switch (m_direction) {
    case FORWARD:
        m_position += m_front * velocity;
        break;
    case BACKWARD:
        m_position -= m_front * velocity;
        break;
    case LEFT:
        m_position -= m_right * velocity;
        break;
    case RIGHT:
        m_position += m_right * velocity;
        break;
    case FORWARDLEFT:
        m_position += (- m_right * invsqrt2 * velocity + m_front * invsqrt2 * velocity);
        break;
    case FORWARDRIGHT:
        m_position += (m_right * invsqrt2 * velocity + m_front * invsqrt2 * velocity);
        break;
    case BACKWARDLEFT:
        m_position += (- m_right * invsqrt2 * velocity - m_front * invsqrt2 * velocity);
        break;
    case BACKWARDRIGHT:
        m_position += (m_right * invsqrt2 * velocity - m_front * invsqrt2 * velocity);
        break;
    case UP:
        m_position += m_worldUp * velocity;
        break;
    case DOWN:
        m_position -= m_worldUp * velocity;
        break;
    case NONE:
        break;
    }

	updateOffsetBuffer();
	glm::vec2 offset = getSmoothMouseOffset();

	if (offset != glm::vec2(0)) {
		m_yaw   += offset.x;
		m_pitch += offset.y;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

        updateCameraVectors();
	}

	m_mouseOffset = glm::vec2(0);
}

void Camera::processKeyPress(Key key)
{
    switch (key)
    {
        case KEY_FORWARD:
            if (m_direction == LEFT)
                m_direction = FORWARDLEFT;
            else if (m_direction == RIGHT)
                m_direction = FORWARDRIGHT;
            else
                m_direction = FORWARD;
            break;
        case KEY_BACKWARD:
            if (m_direction == LEFT)
                m_direction = BACKWARDLEFT;
            else if (m_direction == RIGHT)
                m_direction = BACKWARDRIGHT;
            else
                m_direction = BACKWARD;
            break;
        case KEY_RIGHT:
            if (m_direction == FORWARD)
                m_direction = FORWARDRIGHT;
            else if (m_direction == BACKWARD)
                m_direction = BACKWARDRIGHT;
            else
                m_direction = RIGHT;
            break;
        case KEY_LEFT:
            if (m_direction == FORWARD)
                m_direction = FORWARDLEFT;
            else if (m_direction == BACKWARD)
                m_direction = BACKWARDLEFT;
            else
                m_direction = LEFT;
            break;
        case KEY_UP:
                m_direction = UP;
            break;
        case KEY_DOWN:
                m_direction = DOWN;
            break;
    }
}

void Camera::processKeyRelease(Key key)
{
    switch (key)
    {
    case KEY_FORWARD:
        if (m_direction == Camera::FORWARD)
            m_direction = Camera::NONE;
        else if (m_direction == Camera::FORWARDLEFT)
            m_direction = Camera::LEFT;
        else if (m_direction == Camera::FORWARDRIGHT)
            m_direction = Camera::RIGHT;
        break;
    case KEY_BACKWARD:
        if (m_direction == Camera::BACKWARD)
            m_direction = Camera::NONE;
        else if (m_direction == Camera::BACKWARDLEFT)
            m_direction = Camera::LEFT;
        else if (m_direction == Camera::BACKWARDRIGHT)
            m_direction = Camera::RIGHT;
        break;
    case KEY_RIGHT:
        if (m_direction == Camera::RIGHT)
            m_direction = Camera::NONE;
        else if (m_direction == Camera::FORWARDRIGHT)
            m_direction = Camera::FORWARD;
        else if (m_direction == Camera::BACKWARDRIGHT)
            m_direction = Camera::BACKWARD;
        break;
    case KEY_LEFT:
        if (m_direction == Camera::LEFT)
            m_direction = Camera::NONE;
        else if (m_direction == Camera::FORWARDLEFT)
            m_direction = Camera::FORWARD;
        else if (m_direction == Camera::BACKWARDLEFT)
            m_direction = Camera::BACKWARD;
        break;
    case KEY_UP:
        if (m_direction == Camera::UP)
            m_direction = Camera::NONE;
        break;
    case KEY_DOWN:
        if (m_direction == Camera::DOWN)
            m_direction = Camera::NONE;
        break;
    }
}

void Camera::processMousePress(int mouseX, int mouseY)
{
	m_mouseLastPos = glm::vec2(mouseX, - mouseY);
}

void Camera::processMouseRelease()
{
}

void Camera::processMouseMove(int mouseX, int mouseY)
{
	glm::vec2 mousePos(mouseX, - mouseY);
	m_mouseOffset = mousePos - m_mouseLastPos;

	m_mouseLastPos = mousePos;
}

void Camera::setMouseOffsetBufferSize(size_t size)
{
	m_bufferSize = size;
	initOffsetBuffer();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yoffset * 0.1;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::initOffsetBuffer() {
	m_offsetBuffer.clear();
	m_offsetBuffer.reserve(m_bufferSize);
    for (int i = 0; i < m_bufferSize; i++) {
		m_offsetBuffer.push_back(glm::vec2(0));
    }
    m_offsetBufferEnd = 0;
}

void Camera::updateOffsetBuffer()
{
	//Remplissage du buffer de mouvements de la souris
	m_offsetBuffer[m_offsetBufferEnd] = m_mouseOffset * m_mouseSensitivity;
	m_offsetBufferEnd++;
	if (m_offsetBufferEnd == m_bufferSize) m_offsetBufferEnd = 0;
}

glm::vec2 Camera::getSmoothMouseOffset()
{
	glm::vec2 offset(0);
	for (int i = 0; i < m_bufferSize; i++) {
		offset += m_offsetBuffer[i];
	}
	return offset;
}




