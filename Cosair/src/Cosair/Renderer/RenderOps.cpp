#include "crpch.h"
#include "RenderOps.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Cosair {

	Scope<RendererAPI> RenderOps::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();

}