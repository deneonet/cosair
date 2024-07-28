#include "crpch.h"

#include "opengl_renderer_api.h"

#include <GLFW/glfw3.h>

namespace cosair {

#ifdef CR_DEBUG
static void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id,
                                  unsigned severity, int length,
                                  const char* message, const void* userParam) {
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      CR_CORE_CRITICAL(message);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      CR_CORE_ERROR(message);
      break;
    case GL_DEBUG_SEVERITY_LOW:
      CR_CORE_WARN(message);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      CR_CORE_TRACE(message);
      break;
  }

  CR_CORE_CRITICAL("OpenGL error found -> Details in the console");
}
#endif

void OpenGLRendererAPI::Init() {
  CR_PROFILE_FUNCTION();

#ifdef CR_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  /*
  For reproduce-steps of my AMD driver bug report, because atio6axx.dll is
  doing a access violation when making more than 1530-1600+ texture handles
  concurrently resident.

  -> AMD Radeon RX 7700 XT, 24.6.1 (Driver Version)

  for (int i = 0; i < 1550; i++) {
    uint32_t tex_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex_id);
    glTextureStorage2D(tex_id, 1, GL_RGB8, 200, 200);

    glTextureParameteri(tex_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(tex_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(tex_id, 0, 0, 0, 200, 200, GL_RGB, GL_UNSIGNED_BYTE,
                        "0xFFFFF");

    uint64_t handle_id = glGetTextureHandleARB(tex_id);
    glMakeTextureHandleResidentARB(handle_id);
  }

  /\ works on my old NVIDIA GTX 1050 TI, 536.40 (Driver Version)
  */
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::DrawIndexed(size_t indicies_count) {
  glDrawElements(GL_TRIANGLES, (GLsizei)indicies_count, GL_UNSIGNED_INT,
                 nullptr);
}

void OpenGLRendererAPI::DrawIndexed(const VertexArrayRef& vertex_array) {
  glDrawElements(GL_TRIANGLES,
                 (GLsizei)vertex_array->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}

}  // namespace cosair