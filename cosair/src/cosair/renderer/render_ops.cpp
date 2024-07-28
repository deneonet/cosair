#include "crpch.h"

#include "render_ops.h"

#include "platforms/opengl/opengl_renderer_api.h"

namespace cosair {

Scope<RendererApi> RenderOps::renderer_api_ =
    std::make_unique<OpenGLRendererAPI>();

}