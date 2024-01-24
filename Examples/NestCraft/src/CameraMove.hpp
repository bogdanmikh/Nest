#include <Nest.hpp>

class CameraMove {
public:
    explicit CameraMove(Window* window, Shader* shader, float posX = 0, float posY = 0, float posZ = 0);
    void update(double deltaTime);
private:
    Window* window;
    Camera camera;

    glm::vec2 lastPos;
    float cameraSpeed = 5.f;
};

