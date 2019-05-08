/************************************************************************************

Authors     :   Bradley Austin Davis <bdavis@saintandreas.org>
Copyright   :   Copyright Brad Davis. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/

#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>

#include <Windows.h>

#define __STDC_FORMAT_MACROS 1

#define FAIL(X) throw std::runtime_error(X)

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL 
//

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Skybox.h"

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

///////////////////////////////////////////////////////////////////////////////
//
// GLEW gives cross platform access to OpenGL 3.x+ functionality.  
//

#include <GL/glew.h>

bool checkFramebufferStatus(GLenum target = GL_FRAMEBUFFER)
{
  GLuint status = glCheckFramebufferStatus(target);
  switch (status)
  {
  case GL_FRAMEBUFFER_COMPLETE:
    return true;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    std::cerr << "framebuffer incomplete attachment" << std::endl;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    std::cerr << "framebuffer missing attachment" << std::endl;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    std::cerr << "framebuffer incomplete draw buffer" << std::endl;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
    std::cerr << "framebuffer incomplete read buffer" << std::endl;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
    std::cerr << "framebuffer incomplete multisample" << std::endl;
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
    std::cerr << "framebuffer incomplete layer targets" << std::endl;
    break;

  case GL_FRAMEBUFFER_UNSUPPORTED:
    std::cerr << "framebuffer unsupported internal format or image" << std::endl;
    break;

  default:
    std::cerr << "other framebuffer error" << std::endl;
    break;
  }

  return false;
}

bool checkGlError()
{
  GLenum error = glGetError();
  if (!error)
  {
    return false;
  }
  else
  {
    switch (error)
    {
    case GL_INVALID_ENUM:
      std::cerr <<
        ": An unacceptable value is specified for an enumerated argument.The offending command is ignored and has no other side effect than to set the error flag.";
      break;
    case GL_INVALID_VALUE:
      std::cerr <<
        ": A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag";
      break;
    case GL_INVALID_OPERATION:
      std::cerr <<
        ": The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag..";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr <<
        ": The framebuffer object is not complete.The offending command is ignored and has no other side effect than to set the error flag.";
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr <<
        ": There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
      break;
    case GL_STACK_UNDERFLOW:
      std::cerr <<
        ": An attempt has been made to perform an operation that would cause an internal stack to underflow.";
      break;
    case GL_STACK_OVERFLOW:
      std::cerr << ": An attempt has been made to perform an operation that would cause an internal stack to overflow.";
      break;
    }
    return true;
  }
}

void glDebugCallbackHandler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg,
                            GLvoid* data)
{
  OutputDebugStringA(msg);
  std::cout << "debug call: " << msg << std::endl;
}

//////////////////////////////////////////////////////////////////////
//
// GLFW provides cross platform window creation
//

#include <GLFW/glfw3.h>
#include "Definitions.h"
#include "Input.h"
#include "ObjectManager.h"

//init controller
bool Input::indexTriggerL = false;
bool Input::handTriggerL = false;
bool Input::indexTriggerR = false;
bool Input::handTriggerR = false;
bool Input::buttonA = false;
bool Input::buttonB = false;
bool Input::buttonX = false;
bool Input::buttonY = false;
glm::vec2 Input::stickR = glm::vec2(0, 0);
glm::vec2 Input::stickL = glm::vec2(0, 0);
bool Input::buttonStickL = false;
bool Input::buttonStickR = false;

namespace glfw
{
  inline GLFWwindow* createWindow(const uvec2& size, const ivec2& position = ivec2(INT_MIN))
  {
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
    if (!window)
    {
      FAIL("Unable to create rendering window");
    }
    if ((position.x > INT_MIN) && (position.y > INT_MIN))
    {
      glfwSetWindowPos(window, position.x, position.y);
    }
    return window;
  }
}

// A class to encapsulate using GLFW to handle input and render a scene
class GlfwApp{
protected:
  uvec2 windowSize;
  ivec2 windowPosition;
  GLFWwindow* window{nullptr};
  unsigned int frame{0};
  
  //project vars
  ObjectManager * projectManager;
  StereoMode stereoMode = STEREO_BOTH;
  DisplayMode displayMode = MODE_STEREO;
  TrackingMode trackingMode = TRACKING_FULL;

  //toggles
  bool a_press = false;
  bool b_press = false;
  bool x_press = false;
  bool y_press = false;
  bool ls_press = false;
  bool rs_press = false;
  bool itr_press = false;
  bool itl_press = false;
  bool htr_press = false;
  bool htl_press = false;

public:
  GlfwApp()
  {
    // Initialize the GLFW system for creating and positioning windows
    if (!glfwInit())
    {
      FAIL("Failed to initialize GLFW");
    }
    glfwSetErrorCallback(ErrorCallback);
  }

  virtual ~GlfwApp()
  {
    if (nullptr != window)
    {
      glfwDestroyWindow(window);
    }
    glfwTerminate();
  }

  virtual int run(){
    preCreate();

    window = createRenderingTarget(windowSize, windowPosition);

    if (!window)
    {
      std::cout << "Unable to create OpenGL window" << std::endl;
      return -1;
    }

    postCreate();

    initGl();
	projectManager = new ObjectManager();

    while (!glfwWindowShouldClose(window)){
      ++frame;
      glfwPollEvents();
      update();
      draw();
      finishFrame();
    }

    shutdownGl();

    return 0;
  }

protected:
  virtual GLFWwindow* createRenderingTarget(uvec2& size, ivec2& pos) = 0;

  virtual void draw() = 0;

  void preCreate()
  {
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  void postCreate()
  {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwMakeContextCurrent(window);

    // Initialize the OpenGL bindings
    // For some reason we have to set this experminetal flag to properly
    // init GLEW if we use a core context.
    glewExperimental = GL_TRUE;
    if (0 != glewInit())
    {
      FAIL("Failed to initialize GLEW");
    }
    glGetError();

    if (GLEW_KHR_debug)
    {
      GLint v;
      glGetIntegerv(GL_CONTEXT_FLAGS, &v);
      if (v & GL_CONTEXT_FLAG_DEBUG_BIT)
      {
        //glDebugMessageCallback(glDebugCallbackHandler, this);
      }
    }
  }

  virtual void initGl()
  {
  }

  virtual void shutdownGl()
  {
  }

  virtual void finishFrame()
  {
    glfwSwapBuffers(window);
  }

  virtual void destroyWindow()
  {
    glfwSetKeyCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwDestroyWindow(window);
  }

  virtual void onKey(int key, int scancode, int action, int mods)
  {
    if (GLFW_PRESS != action)
    {
      return;
    }

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, 1);
      return;
    }
  }

  virtual void update(){
  
  
  }

  virtual void onMouseButton(int button, int action, int mods)
  {
  }

protected:
  virtual void viewport(const ivec2& pos, const uvec2& size)
  {
    glViewport(pos.x, pos.y, size.x, size.y);
  }

private:

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    GlfwApp* instance = (GlfwApp *)glfwGetWindowUserPointer(window);
    instance->onKey(key, scancode, action, mods);
  }

  static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
  {
    GlfwApp* instance = (GlfwApp *)glfwGetWindowUserPointer(window);
    instance->onMouseButton(button, action, mods);
  }

  static void ErrorCallback(int error, const char* description)
  {
    FAIL(description);
  }
};

//////////////////////////////////////////////////////////////////////
//
// The Oculus VR C API provides access to information about the HMD
//

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

namespace ovr
{
  // Convenience method for looping over each eye with a lambda
  template <typename Function>
  inline void for_each_eye(Function function)
  {
    for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
         eye < ovrEyeType::ovrEye_Count;
         eye = static_cast<ovrEyeType>(eye + 1))
    {
      function(eye);
    }
  }

  inline mat4 toGlm(const ovrMatrix4f& om)
  {
    return glm::transpose(glm::make_mat4(&om.M[0][0]));
  }

  inline mat4 toGlm(const ovrFovPort& fovport, float nearPlane = 0.01f, float farPlane = 10000.0f)
  {
    return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
  }

  inline vec3 toGlm(const ovrVector3f& ov)
  {
    return glm::make_vec3(&ov.x);
  }

  inline vec2 toGlm(const ovrVector2f& ov)
  {
    return glm::make_vec2(&ov.x);
  }

  inline uvec2 toGlm(const ovrSizei& ov)
  {
    return uvec2(ov.w, ov.h);
  }

  inline quat toGlm(const ovrQuatf& oq)
  {
    return glm::make_quat(&oq.x);
  }

  inline mat4 toGlm(const ovrPosef& op)
  {
    mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
    mat4 translation = glm::translate(mat4(), ovr::toGlm(op.Position));
    return translation * orientation;
  }

  inline ovrMatrix4f fromGlm(const mat4& m)
  {
    ovrMatrix4f result;
    mat4 transposed(glm::transpose(m));
    memcpy(result.M, &(transposed[0][0]), sizeof(float) * 16);
    return result;
  }

  inline ovrVector3f fromGlm(const vec3& v)
  {
    ovrVector3f result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    return result;
  }

  inline ovrVector2f fromGlm(const vec2& v)
  {
    ovrVector2f result;
    result.x = v.x;
    result.y = v.y;
    return result;
  }

  inline ovrSizei fromGlm(const uvec2& v)
  {
    ovrSizei result;
    result.w = v.x;
    result.h = v.y;
    return result;
  }

  inline ovrQuatf fromGlm(const quat& q)
  {
    ovrQuatf result;
    result.x = q.x;
    result.y = q.y;
    result.z = q.z;
    result.w = q.w;
    return result;
  }
}

class RiftManagerApp
{
protected:
  ovrSession _session;
  ovrHmdDesc _hmdDesc;
  ovrGraphicsLuid _luid;

public:
  RiftManagerApp()
  {
    if (!OVR_SUCCESS(ovr_Create(&_session, &_luid)))
    {
      FAIL("Unable to create HMD session");
    }

    _hmdDesc = ovr_GetHmdDesc(_session);
  }

  ~RiftManagerApp()
  {
    ovr_Destroy(_session);
    _session = nullptr;
  }
};
#include <Windows.h>
class RiftApp : public GlfwApp, public RiftManagerApp{
public:

private:
  GLuint _fbo{0};
  GLuint _depthBuffer{0};
  ovrTextureSwapChain _eyeTexture;

  GLuint _mirrorFbo{0};
  ovrMirrorTexture _mirrorTexture;

  ovrEyeRenderDesc _eyeRenderDescs[2];

  mat4 _eyeProjections[2];

  ovrLayerEyeFov _sceneLayer;
  ovrViewScaleDesc _viewScaleDesc;

  uvec2 _renderTargetSize;
  uvec2 _mirrorSize;

public:

  RiftApp()
  {
    using namespace ovr;
    _viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;

    memset(&_sceneLayer, 0, sizeof(ovrLayerEyeFov));
    _sceneLayer.Header.Type = ovrLayerType_EyeFov;
    _sceneLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

    ovr::for_each_eye([&](ovrEyeType eye)
    {
      ovrEyeRenderDesc& erd = _eyeRenderDescs[eye] = ovr_GetRenderDesc(_session, eye, _hmdDesc.DefaultEyeFov[eye]);
      ovrMatrix4f ovrPerspectiveProjection =
        ovrMatrix4f_Projection(erd.Fov, 0.01f, 1000.0f, ovrProjection_ClipRangeOpenGL);
      _eyeProjections[eye] = ovr::toGlm(ovrPerspectiveProjection);
      _viewScaleDesc.HmdToEyePose[eye] = erd.HmdToEyePose;

      ovrFovPort& fov = _sceneLayer.Fov[eye] = _eyeRenderDescs[eye].Fov;
      auto eyeSize = ovr_GetFovTextureSize(_session, eye, fov, 1.0f);
      _sceneLayer.Viewport[eye].Size = eyeSize;
      _sceneLayer.Viewport[eye].Pos = {(int)_renderTargetSize.x, 0};

      _renderTargetSize.y = std::max(_renderTargetSize.y, (uint32_t)eyeSize.h);
      _renderTargetSize.x += eyeSize.w;
    });
    // Make the on screen window 1/4 the resolution of the render target
    _mirrorSize = _renderTargetSize;
    _mirrorSize /= 4;
  }

protected:
  GLFWwindow* createRenderingTarget(uvec2& outSize, ivec2& outPosition) override
  {
    return glfw::createWindow(_mirrorSize);
  }

  void initGl() override
  {
    GlfwApp::initGl();

    // Disable the v-sync for buffer swap
    glfwSwapInterval(0);

    ovrTextureSwapChainDesc desc = {};
    desc.Type = ovrTexture_2D;
    desc.ArraySize = 1;
    desc.Width = _renderTargetSize.x;
    desc.Height = _renderTargetSize.y;
    desc.MipLevels = 1;
    desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.SampleCount = 1;
    desc.StaticImage = ovrFalse;
    ovrResult result = ovr_CreateTextureSwapChainGL(_session, &desc, &_eyeTexture);
    _sceneLayer.ColorTexture[0] = _eyeTexture;
    if (!OVR_SUCCESS(result))
    {
      FAIL("Failed to create swap textures");
    }

    int length = 0;
    result = ovr_GetTextureSwapChainLength(_session, _eyeTexture, &length);
    if (!OVR_SUCCESS(result) || !length)
    {
      FAIL("Unable to count swap chain textures");
    }
    for (int i = 0; i < length; ++i)
    {
      GLuint chainTexId;
      ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, i, &chainTexId);
      glBindTexture(GL_TEXTURE_2D, chainTexId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Set up the framebuffer object
    glGenFramebuffers(1, &_fbo);
    glGenRenderbuffers(1, &_depthBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _renderTargetSize.x, _renderTargetSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    ovrMirrorTextureDesc mirrorDesc;
    memset(&mirrorDesc, 0, sizeof(mirrorDesc));
    mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
    mirrorDesc.Width = _mirrorSize.x;
    mirrorDesc.Height = _mirrorSize.y;
    if (!OVR_SUCCESS(ovr_CreateMirrorTextureGL(_session, &mirrorDesc, &_mirrorTexture)))
    {
      FAIL("Could not create mirror texture");
    }
    glGenFramebuffers(1, &_mirrorFbo);
  }

  void onKey(int key, int scancode, int action, int mods) override
  {
    if (GLFW_PRESS == action)
      switch (key)
      {
      case GLFW_KEY_R:
        ovr_RecenterTrackingOrigin(_session);
        return;
      }

    GlfwApp::onKey(key, scancode, action, mods);
  }

  double lastTime = 0;
  glm::mat4 lastView [2] = { glm::mat4(1), glm::mat4(1) };
  ovrVector3f lastPosition[2] = {ovr::fromGlm(glm::vec3(0)), ovr::fromGlm(glm::vec3(0))};
  ovrQuatf lastRotation[2] = { ovr::fromGlm(glm::quat()), ovr::fromGlm(glm::quat())};
  float iod = 0.0f;
  bool renderCustomBoxes = false;
  glm::mat4 ringBufferL[30] = { glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1)};
  glm::mat4 ringBufferR[30] = { glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1) };
  glm::mat4 ringBufferCL[30] = { glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1) };
  glm::mat4 ringBufferCR[30] = { glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),
								glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1),glm::mat4(1) };
  int ringIndex = 0;
  int ringLag = 0;
  int renderingIndex = 0;
  int renderingDelay = 0;

  void draw() final override{
    ovrPosef eyePoses[2];
    ovr_GetEyePoses(_session, frame, true, _viewScaleDesc.HmdToEyePose, eyePoses, &_sceneLayer.SensorSampleTime);

    int curIndex;
    ovr_GetTextureSwapChainCurrentIndex(_session, _eyeTexture, &curIndex);
    GLuint curTexId;
    ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, curIndex, &curTexId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
	//==============================================================================CONTROLLER
	// Query Touch controllers. Query their parameters:
	double displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, 0);
	ovrTrackingState trackState = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);

	// Process controller status. Useful to know if controller is being used at all, and if the cameras can see it. 
	// Bits reported:
	// Bit 1: ovrStatus_OrientationTracked  = Orientation is currently tracked (connected and in use)
	// Bit 2: ovrStatus_PositionTracked     = Position is currently tracked (false if out of range)
	unsigned int handStatus[2];
	handStatus[0] = trackState.HandStatusFlags[0];
	handStatus[1] = trackState.HandStatusFlags[1];

	// Process controller position and orientation:
	ovrPosef handPoses[2];  // These are position and orientation in meters in room coordinates, relative to tracking origin. Right-handed cartesian coordinates.
							// ovrQuatf     Orientation;
							// ovrVector3f  Position;
	handPoses[0] = trackState.HandPoses[0].ThePose;
	handPoses[1] = trackState.HandPoses[1].ThePose;
	ovrVector3f handPosition[2];
	handPosition[0] = handPoses[0].Position;
	handPosition[1] = handPoses[1].Position;

	//=============================================================================BUTTON PRESSES
	ovrInputState inputState;

	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		//Index Trigger
		Input::setIndexTriggerL((inputState.IndexTrigger[ovrHand_Left] > 0.5f));
		Input::setIndexTriggerR((inputState.IndexTrigger[ovrHand_Right] > 0.5f));

		//Hand Trigger
		Input::setHandTriggerL((inputState.HandTrigger[ovrHand_Left] > 0.5f));
		Input::setHandTriggerR((inputState.HandTrigger[ovrHand_Right] > 0.5f));

		//Buttons
		Input::setButtonA(inputState.Buttons & ovrButton_A);
		Input::setButtonB(inputState.Buttons & ovrButton_B);
		Input::setButtonX(inputState.Buttons & ovrButton_X);
		Input::setButtonY(inputState.Buttons & ovrButton_Y);

		//Sticks
		Input::setStickL(ovr::toGlm(inputState.Thumbstick[0]));
		Input::setStickR(ovr::toGlm(inputState.Thumbstick[1]));
		Input::setButtonStickL(inputState.Buttons & ovrButton_LThumb);
		Input::setButtonStickR(inputState.Buttons & ovrButton_RThumb);
	}

	//==============================================================================CONTROLLER BUTTON HANDLING
	//Left Stick
	setCubeScale(Input::getStickL().x * 0.0025f);
	//Right Stick
	iod += Input::getStickR().x * 0.007f;
	if (iod < -0.1f) iod = -0.1f;
	else if (iod > 0.3f) iod = 0.3f;
	
	//Hand Triggers - Right
	if (Input::getHandTriggerR()) {
		if (!htr_press) {
			htr_press = true;
			renderingDelay++;
			if (renderingDelay > 10) renderingDelay = 10;
			std::cout << "!! Rendering Delay: " << renderingDelay << " frames !!" << std::endl;
		}
	}
	else htr_press = false;

	//Hand Triggers - Left
	if (Input::getHandTriggerL()) {
		if (!htl_press) {
			htl_press = true;
			renderingDelay--;
			if (renderingDelay < 0) renderingDelay = 0;
			std::cout << "!! Rendering Delay: " << renderingDelay << " frames !!" << std::endl;
		}
	}
	else htl_press = false;

	//Index Triggers - Right
	if (Input::getIndexTriggerR()) {
		if (!itr_press) {
			itr_press = true;
			ringLag++;
			if (ringLag >= 30) ringLag = 0;
			std::cout << "!! Tracking Lag: " << ringLag << " frames !!" << std::endl;
		}
	}
	else itr_press = false;

	//Index Triggers - Left
	if (Input::getIndexTriggerL()) {
		if (!itl_press) {
			itl_press = true;
			ringLag--;
			if (ringLag < 0) ringLag = 30 + ringLag;
			std::cout << "!! Tracking Lag: " << ringLag << " frames !!" << std::endl;
		}
	}
	else itl_press = false;

	//Button X
	if (Input::getButtonX()) {
		if (!x_press) {
			x_press = true;
			switch (stereoMode) {
			case STEREO_BOTH:	//Skybox and cubes are stereo
				stereoMode = STEREO_SKY;
				break;
			case STEREO_SKY:	//Skybox is stereo
				stereoMode = STEREO_ONESKY;
				break;
			case STEREO_ONESKY:	//Cubes are stereo
				stereoMode = STEREO_CUSTOM_SKYBOX;
				break;
			case STEREO_CUSTOM_SKYBOX:	//Custom Skybox
				stereoMode = STEREO_BOTH;
				break;
			default:
				std::cerr << "====== Something went wrong with ButtonPressX ======" << std::endl;
				break;
			};
		}
	}
	else x_press = false;
	//Button Y
	if (Input::getButtonY()) {
		if (!y_press) {
			y_press = true;
			renderCustomBoxes = !renderCustomBoxes;
		}
	}
	else y_press = false;
	//Button B
	if (Input::getButtonB()) {
		if (!b_press) {
			b_press = true;
			switch (trackingMode) {
			case TRACKING_FULL:
				trackingMode = TRACKING_ORIENTATION;
				break;
			case TRACKING_ORIENTATION:
				trackingMode = TRACKING_POSITION;
				break;
			case TRACKING_POSITION:
				trackingMode = TRACKING_NONE;
				break;
			case TRACKING_NONE:
				trackingMode = TRACKING_FULL;
				break;
			default:
				std::cerr << "====== Something went wrong with PressButtonB ======" << std::endl;
			}
		}
	}
	else b_press = false;
	//Button A
	if (Input::getButtonA()) {
		if (!a_press) {
			a_press = true;
			switch (displayMode) {
			case MODE_STEREO:
				displayMode = MODE_MONO;
				break;
			case MODE_MONO:
				displayMode = MODE_LEFT;
				break;
			case MODE_LEFT:
				displayMode = MODE_RIGHT;
				break;
			case MODE_RIGHT:
				displayMode = MODE_INVERTED;
				break;
			case MODE_INVERTED:
				displayMode = MODE_STEREO;
				break;
			default:
				std::cerr << "====== Something went wrong with ButtonPressA ======" << std::endl;
				break;
			}
		}
	}
	else a_press = false;
	//Button LS
	if (Input::getButtonStickL()) {
		if (!ls_press) {
			ls_press = true;
			resetCubeScale();
		}
	}
	else ls_press = false;
	//Button RS
	if (Input::getButtonStickR()) {
		if (!rs_press) {
			rs_press = true;
			iod = 0;
		}
	}
	else rs_press = false;

	//==============================================================================UPDATE

	//Render ring info
	int renderIndex = ringIndex - ringLag;
	if (renderIndex < 0) renderIndex = 30 + renderIndex;

	//Send hands information to the project manager
	ringBufferCL[ringIndex] = ovr::toGlm(handPoses[ovrHand_Left]);
	ringBufferCR[ringIndex] = ovr::toGlm(handPoses[ovrHand_Right]);
	

	//Calls update in children
	projectManager->update(ovr_GetTimeInSeconds() - lastTime);
	lastTime = ovr_GetTimeInSeconds();

	//Rendering lag
	if (renderingIndex < renderingDelay) { renderingIndex++; return; }
	renderingIndex = 0;

	//==============================================================================DRAW
	ovr::for_each_eye([&](ovrEyeType eye){
		//---------------------------------------------------Setups
		const auto& vp = _sceneLayer.Viewport[eye];
		glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
		_sceneLayer.RenderPose[eye] = eyePoses[eye];
		//---------------------------------------------------IOD
		ovrEyeRenderDesc& erd = _eyeRenderDescs[eye] = ovr_GetRenderDesc(_session, eye, _hmdDesc.DefaultEyeFov[eye]);
		_viewScaleDesc.HmdToEyePose[eye] = erd.HmdToEyePose;

		if (eye == 0)	_viewScaleDesc.HmdToEyePose[eye].Position.x -= iod;
		else			_viewScaleDesc.HmdToEyePose[eye].Position.x += iod;
		//---------------------------------------------------Display Mode
		bool render = true;
		short curEye = eye;
		
		switch (displayMode) {
		case MODE_STEREO:		//3d stereo
			curEye = eye;
			break;
		case MODE_MONO:			//Same image both eyes
			curEye = 0;
			break;
		case MODE_LEFT:			//Render left eye, right eye black
			if (eye == 1) render = false;
			break;
		case MODE_RIGHT:		//Render right eye, left eye black
			if (eye == 0) render = false;
			break;
		case MODE_INVERTED:		//Render left in right, and right in left
			renderIndex--; if (renderIndex < 0) renderIndex = 29;
			if (eye == 0) { curEye = 1; }//  renderIndex--; if (renderIndex < 0) renderIndex = 29; }
			else { curEye = 0; } //renderIndex++; if (renderIndex >= 30) renderIndex = 0; }
			break;
		default:
			std::cerr << "====== Something went wrong with displayMode ======" << std::endl;
			break;
		}
		//---------------------------------------------------Tracking Mode
		glm::mat4 view = glm::mat4(1);

		switch (trackingMode) {
		case TRACKING_FULL:			//Full Tracking
			view = glm::inverse(ovr::toGlm(eyePoses[curEye]));
			break;
		case TRACKING_ORIENTATION:	//Orientation Tracking
			eyePoses[curEye].Position = lastPosition[curEye];
			view = glm::inverse(ovr::toGlm(eyePoses[curEye]));
			break;
		case TRACKING_POSITION:		//Position Tracking
			eyePoses[curEye].Orientation = lastRotation[curEye];
			view = glm::inverse(ovr::toGlm(eyePoses[curEye]));
			break;
		case TRACKING_NONE:			//No Tracking
			view = lastView[curEye];
			break;
		default:
			std::cerr << "====== Something went wrong with trackingMode ======= " << std::endl;
			break;
		}
		//---------------------------------------------------Ring Buffer
		glm::mat4 curView;

		if (eye == 0)	ringBufferL[ringIndex] = view;
		else			ringBufferR[ringIndex] = view;

		if (curEye == 0)	curView = ringBufferL[renderIndex]; 
		else				curView = ringBufferR[renderIndex];

		projectManager->updateHands(ringBufferCL[renderIndex], ringBufferCR[renderIndex]);

		//---------------------------------------------------Stereo Mode
		if (render) {
			//PARAMETERS: projection, view, render in stereo, render cubes, render custom skybox, render custom boxes
			switch (stereoMode) {
			case STEREO_BOTH:	//Skybox and cubes are stereo
				renderScene(_eyeProjections[curEye], curView, (curEye == 0), true, true, false, renderCustomBoxes);
				break;
			case STEREO_SKY:	//Skybox is stereo
				renderScene(_eyeProjections[curEye], curView, (curEye == 0), false, true, false, renderCustomBoxes);
				break;
			case STEREO_ONESKY:	//Stereo but one skybox image
				renderScene(_eyeProjections[curEye], curView, (curEye == 0), false, false, false, renderCustomBoxes);
				break;
			case STEREO_CUSTOM_SKYBOX:
				renderScene(_eyeProjections[curEye], curView, (curEye == 0), false, false, true, renderCustomBoxes);
				break;
			default:
				std::cerr << "====== Something went wrong in StereoMode ======" << std::endl;
				break;
			}

			projectManager->draw(curView, _eyeProjections[curEye]);
		}
		//---------------------------------------------------Store variables for next frame
		lastView[curEye] = view;
		lastPosition[curEye] = eyePoses[curEye].Position;
		lastRotation[curEye] = eyePoses[curEye].Orientation;
    });

	//================================================================================= AFTER RENDERING
	if(displayMode == MODE_INVERTED) renderIndex++; if (renderIndex >= 30) renderIndex = 0;

	ringIndex++;
	if (ringIndex >= 30) ringIndex = 0;

	//=================================================================================

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    ovr_CommitTextureSwapChain(_session, _eyeTexture);
    ovrLayerHeader* headerList = &_sceneLayer.Header;
    ovr_SubmitFrame(_session, frame, &_viewScaleDesc, &headerList, 1);

    GLuint mirrorTextureId;
    ovr_GetMirrorTextureBufferGL(_session, _mirrorTexture, &mirrorTextureId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureId, 0);
    glBlitFramebuffer(0, 0, _mirrorSize.x, _mirrorSize.y, 0, _mirrorSize.y, _mirrorSize.x, 0, GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	//Sleep(150);
  }

	virtual void renderScene(const glm::mat4& projection, const glm::mat4& headPose, bool eye, bool box, bool stereoSky, bool customSkybox, bool customBox) = 0;
	virtual void setCubeScale(float s) = 0;
	virtual void resetCubeScale() = 0;
};

//////////////////////////////////////////////////////////////////////
//
// The remainder of this code is specific to the scene we want to 
// render.  I use glfw to render an array of cubes, but your 
// application would perform whatever rendering you want
//

#include <vector>
#include "shader.h"
#include "Cube.h"

// a class for building and rendering cubes
class Scene{
  // Program
  std::vector<glm::mat4> instance_positions;
  GLuint instanceCount;
  GLuint shaderID;

  std::unique_ptr<TexturedCube> cube;
  std::unique_ptr<TexturedCube> customCubeL;
  std::unique_ptr<TexturedCube> customCubeR;

  std::unique_ptr<Skybox> skyboxLeft;
  std::unique_ptr<Skybox> skyboxRight;
  std::unique_ptr<Skybox> skyboxCustom;

  const unsigned int GRID_SIZE{5};

public:
	// Scale to 20cm: 200cm * 0.1
	float scaleFactor = 0.1f;

	Scene(){
		// Create two cube
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.3)));
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.9)));

		instanceCount = instance_positions.size();

		// Shader Program 
		shaderID = LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT);

		cube = std::make_unique<TexturedCube>(TEXTURE_CUBE_STEAM);
		customCubeL = std::make_unique<TexturedCube>(TEXTURE_CUBE_LEFT);
		customCubeR = std::make_unique<TexturedCube>(TEXTURE_CUBE_RIGHT);

		// 10m wide sky box: size doesn't matter though
		skyboxLeft = std::make_unique<Skybox>(TEXTURE_SKYBOX_LEFT);
		skyboxRight = std::make_unique<Skybox>(TEXTURE_SKYBOX_RIGHT);
		skyboxCustom = std::make_unique<Skybox>(TEXTURE_SKYBOX_CUSTOM);
		skyboxLeft->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
		skyboxRight->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
		skyboxCustom->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	}

	void render(const glm::mat4& projection, const glm::mat4& view, bool custom = false, bool eye = false) {
		// Render two custom cubes
		if (custom) {
			for (int i = 0; i < instanceCount; i++) {
				//Render custom LEFT box
				if (eye) {
					customCubeL->toWorld = instance_positions[i] * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
					customCubeL->draw(shaderID, projection, view);
				}
				//Render custom RIGHT box
				else {
					customCubeR->toWorld = instance_positions[i] * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
					customCubeR->draw(shaderID, projection, view);
				}
			}
		}
		//Render project cubes
		else {
			for (int i = 0; i < instanceCount; i++) {
				cube->toWorld = instance_positions[i] * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
				cube->draw(shaderID, projection, view);
			}
		}
	}

	void renderSkyboxLeft(const glm::mat4& projection, const glm::mat4& view) {
		skyboxLeft->draw(shaderID, projection, view);
	}

	void renderSkyboxRight(const glm::mat4& projection, const glm::mat4& view) {
		skyboxRight->draw(shaderID, projection, view);
	}

	void renderCustomSkybox(const glm::mat4& projection, const glm::mat4& view) {
		skyboxCustom->draw(shaderID, projection, view);
	}
};

// An example application that renders a simple cube
class ExampleApp : public RiftApp{
	std::shared_ptr<Scene> scene;

public:
	ExampleApp(){
	}

protected:
	void initGl() override{
		RiftApp::initGl();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);
		scene = std::shared_ptr<Scene>(new Scene());
	}

	void shutdownGl() override
  {
    scene.reset();
  }

	void renderScene(const glm::mat4& projection, const glm::mat4& headPose, bool eye, bool box, bool stereoSky, bool customSkybox, bool customBox) override {
		//Render Box
		if (box) {
			if(customBox) scene->render(projection, headPose, true, eye);
			else scene->render(projection, headPose);
		}

		//Renders custom skybox
		if (customSkybox) scene->renderCustomSkybox(projection, headPose);
		//Renders project skybox
		else {
			//Render Skybox in stereo (different image per eye)
			if (stereoSky) {
				if (eye) scene->renderSkyboxLeft(projection, headPose);
				else scene->renderSkyboxRight(projection, headPose);
			}
			//Render Skybox in mono (same image per eye)
			else scene->renderSkyboxLeft(projection, headPose);
		}
	}

	void setCubeScale(float s) override {
		scene->scaleFactor += s;

		if (scene->scaleFactor <= 0.01f) scene->scaleFactor = 0.01f;
		else if (scene->scaleFactor >= 0.5f) scene->scaleFactor = 0.5f;
	}

	void resetCubeScale() {
		scene->scaleFactor = 0.1f;
	}
};

// Execute our example class
int main(int argc, char** argv)
{
  int result = -1;

  if (!OVR_SUCCESS(ovr_Initialize(nullptr)))
  {
    FAIL("Failed to initialize the Oculus SDK");
  }
  result = ExampleApp().run();

  ovr_Shutdown();
  return result;
}
