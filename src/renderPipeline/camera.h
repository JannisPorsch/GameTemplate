#ifndef __CAMERA_H__
#define __CAMERA_H__




#include "main.h"
#include <cglm/cglm.h>




typedef struct _CAMERA
{
    mat4 view;
    mat4 proj;
    mat4 projView;

    vec3 pos;
    vec3 dir;
    vec3 right;
    vec3 up;

    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float fov;
    float near;
    float far;
    float sensitivity;

    bool firstMouse;
    bool moved;
} CAMERA, *PCAMERA;




void cameraInit(vec3 pos, int windowWidth, int windowHeight);


void cameraUpdate();


void updateCameraView();


void updateCameraProj(int windowWidth, int windowHeight);


bool debugCameraInUse();




extern PCAMERA currentCamera;




#endif // !__CAMERA_H__
