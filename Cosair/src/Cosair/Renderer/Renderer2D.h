#pragma once

#include "Camera.h"
#include "Material.h"
#include "Transform.h"

namespace Cosair {

    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(const char* materialName);
        static void DrawQuad(const MaterialRef& material);
        static void DrawQuad(const char* materialName, const TransformRef& transform);
        static void DrawQuad(const MaterialRef& material, const TransformRef& transform);
    };

}