#include "camera.h"
#include "io/input.h"




PCAMERA currentCamera = NULL;

CAMERA mainCamera = { 0 };
#ifdef _DEBUG
CAMERA debugCamera = { 0 };
#endif // _DEBUG




static void handleCameraMovement();




void cameraInit(vec3 pos, int windowWidth, int windowHeight)
{
    currentCamera = &mainCamera;

    glm_vec3_copy(pos, mainCamera.pos);
    glm_vec3_copy((vec3){0.f, 0.f, -1.f}, mainCamera.dir);

    mainCamera.yaw   = -90.0f;
    mainCamera.pitch =  0.0f;
    mainCamera.lastX =  (float)windowWidth / 2.0f;
    mainCamera.lastY =  (float)windowHeight / 2.0f;
    mainCamera.fov   =  45.0f;
    mainCamera.near = .1f;
    mainCamera.far = 100.f;
    mainCamera.sensitivity = .1f;

    mainCamera.firstMouse = true;
    mainCamera.moved = true;

    glm_perspective(glm_rad(mainCamera.fov), (float)windowWidth / (float)windowHeight, mainCamera.near, mainCamera.far, mainCamera.proj);
    updateCameraView();

#ifdef _DEBUG
    u8* src = (u8*)&mainCamera;
    u8* dst = (u8*)&debugCamera;
    for (int i = 0; i < sizeof(CAMERA); i++)
        dst[i] = src[i];
#endif // DEBUG
}


void cameraUpdate()
{
    handleCameraMovement();

#ifdef _DEBUG
    if (isKeyPressed(KEY_F))
    {
        if (currentCamera == &mainCamera)
        {
            u8* src = (u8*)&mainCamera;
            u8* dst = (u8*)&debugCamera;
            for (int i = 0; i < sizeof(CAMERA); i++)
                dst[i] = src[i];
            
            currentCamera = &debugCamera;
        }
        else
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    mainCamera.proj[y][x] = debugCamera.proj[y][x];
                }
            }

            currentCamera = &mainCamera;
            mainCamera.moved = true;
            updateCameraView();
        }
    }
#endif // DEBUG
}


void updateCameraView()
{
    if (!currentCamera->moved) return;

    glm_cross((vec3){0.f, 1.f, 0.f}, currentCamera->dir, currentCamera->right);
    glm_normalize(currentCamera->right);

    glm_cross(currentCamera->dir, currentCamera->right, currentCamera->up);

    vec3 tmp;
    glm_vec3_add(currentCamera->pos, currentCamera->dir, tmp);
    glm_lookat(currentCamera->pos, tmp, currentCamera->up, currentCamera->view);

    glm_mat4_mul(currentCamera->proj, currentCamera->view, currentCamera->projView);
    currentCamera->moved = 0;
}


void updateCameraProj(int windowWidth, int windowHeight)
{
    glm_perspective(glm_rad(currentCamera->fov), (float)windowWidth / (float)windowHeight, currentCamera->near, currentCamera->far, currentCamera->proj);
    glm_mat4_mul(currentCamera->proj, currentCamera->view, currentCamera->projView);

    currentCamera->firstMouse = true;
}


bool debugCameraInUse()
{
    return currentCamera == &debugCamera ? true : false;
}




static void handleCameraMovement()
{
    static float speed = .3f;
    if(isKeyDown(KEY_W))
    {
        vec3 tmp;
        glm_vec3_copy(currentCamera->dir, tmp);
        tmp[1] = 0.f;
        glm_vec3_normalize(tmp);
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_add(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }
    if(isKeyDown(KEY_S))
    {
        vec3 tmp;
        glm_vec3_copy(currentCamera->dir, tmp);
        tmp[1] = 0.f;
        glm_vec3_normalize(tmp);
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_sub(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }
    if(isKeyDown(KEY_A))
    {
        vec3 tmp;
        glm_vec3_copy(currentCamera->right, tmp);
        tmp[1] = 0.f;
        glm_vec3_normalize(tmp);
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_add(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }
    if(isKeyDown(KEY_D))
    {
        vec3 tmp;
        glm_vec3_copy(currentCamera->right, tmp);
        tmp[1] = 0.f;
        glm_vec3_normalize(tmp);
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_sub(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }
    if(isKeyDown(KEY_SPACE))
    {
        vec3 tmp = {0.f, 1.f, 0.f};
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_add(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }
    if(isKeyDown(KEY_LSHIFT))
    {
        vec3 tmp = {0.f, 1.f, 0.f};
        glm_vec3_scale(tmp, speed, tmp);
        glm_vec3_sub(currentCamera->pos, tmp, currentCamera->pos);
        currentCamera->moved = 1;
    }

    updateCameraView();
}
