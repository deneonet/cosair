#include "crpch.h"

#include "renderer_2d.h"

#include "libraries.h"
#include "render_ops.h"
#include "renderer.h"
#include "vertex_array.h"

namespace cosair {

static RendererData2D data;

void Renderer2D::Init(RendererProps2D props) {
  CR_PROFILE_FUNCTION();
#ifdef CR_EXT_BINDLESS_TEXS
  data = {props.max_quads, props.max_bindless_textures};
#else
  data = {props.max_quads};
#endif

  data.quad_vertex_array = VertexArray::Create();

  data.quad_vertex_buffer =
      VertexBuffer::Create(data.verticies_count * sizeof(QuadVertex));
  data.quad_vertex_buffer->SetLayout({{ShaderType::kFloat3, "a_Position"},
                                      {ShaderType::kFloat4, "a_Color"},
                                      {ShaderType::kFloat2, "a_TexCoord"},
                                      {ShaderType::kFloat, "a_TexIndex"}});

  data.quad_vertex_array->AddVertexBuffer(data.quad_vertex_buffer);
  data.quad_vertex_buffer_base = new QuadVertex[data.verticies_count];

  Ref<uint32_t> quad_indicies;
  quad_indicies.reset(new uint32_t[data.indicies_count]);

  uint32_t offset = 0;
  for (int i = 0; i < data.indicies_count; i += 6) {
    quad_indicies.get()[i + 0] = offset + 0;
    quad_indicies.get()[i + 1] = offset + 1;
    quad_indicies.get()[i + 2] = offset + 2;
    quad_indicies.get()[i + 3] = offset + 2;
    quad_indicies.get()[i + 4] = offset + 3;
    quad_indicies.get()[i + 5] = offset + 0;

    offset += 4;
  }

  IndexBufferRef quad_index_buffer =
      IndexBuffer::Create(quad_indicies.get(), data.indicies_count);
  data.quad_vertex_array->SetIndexBuffer(quad_index_buffer);
}

void Renderer2D::Shutdown() { delete data.quad_vertex_buffer_base; }

void Renderer2D::BeginScene(const glm::mat4& view_proj_matrix) {
  CR_PROFILE_FUNCTION();

  Material::FlushViewProjMatrix();
  data.view_proj_matrix = view_proj_matrix;

  ResetBatch();
}

void Renderer2D::EndScene() {
  CR_PROFILE_FUNCTION();

  uint64_t dataSize = (uint8_t*)data.quad_vertex_buffer_ptr -
                      (uint8_t*)data.quad_vertex_buffer_base;
  data.quad_vertex_buffer->SetData(data.quad_vertex_buffer_base, dataSize);

  Flush();
}

void Renderer2D::ResetBatch() {
  data.quad_index_count = 0;
  data.quad_vertex_buffer_ptr = data.quad_vertex_buffer_base;

#ifndef CR_EXT_BINDLESS_TEXS
  data.texture_slots_index = 0;
#endif
}

void Renderer2D::Flush() {
  CR_PROFILE_FUNCTION();

  if (data.quad_index_count == 0) return;

#ifdef CR_EXT_BINDLESS_TEXS
  data.bindless_textures_manager->BindAndFlushTextureHandles();
#else
  for (uint32_t i = 0; i < data.texture_slots_index; i++)
    data.texture_slots[i]->Bind(i);
#endif

  RenderOps::DrawIndexed(data.quad_index_count);
#ifdef CR_EXT_RENDER_STATS
  data.stats.draw_calls++;
#endif
}

#ifdef CR_EXT_BINDLESS_TEXS
void Renderer2D::AddTextureHandle(uint32_t index, uint64_t handle_id) {
  data.bindless_textures_manager->AddTextureHandle(index, handle_id);
}
#endif

void Renderer2D::DrawQuad(const MaterialRef& material, Transform& transform) {
  CR_PROFILE_FUNCTION();

  if (data.quad_index_count >= data.indicies_count) {
    EndScene();
    ResetBatch();
  }

  glm::vec3 vertex_positions[4];
  if (transform.HasRotation()) {
    const glm::mat4& transform_matrix = transform.GetTransformMatrix();
    vertex_positions[0] = transform_matrix * data.quad_vertex_positions[0];
    vertex_positions[1] = transform_matrix * data.quad_vertex_positions[1];
    vertex_positions[2] = transform_matrix * data.quad_vertex_positions[2];
    vertex_positions[3] = transform_matrix * data.quad_vertex_positions[3];
  } else {
    const glm::vec3& scale = transform.GetScale();
    const glm::vec3& position = transform.GetPos();
    vertex_positions[0] = {position.x - scale.x * 0.5f,
                           position.y - scale.y * 0.5f, position.z};
    vertex_positions[1] = {position.x + scale.x * 0.5f,
                           position.y - scale.y * 0.5f, position.z};
    vertex_positions[2] = {position.x + scale.x * 0.5f,
                           position.y + scale.y * 0.5f, position.z};
    vertex_positions[3] = {position.x - scale.x * 0.5f,
                           position.y + scale.y * 0.5f, position.z};
  }

  float texture_slots_index = material->Bind(data);
  TextureCoords texture_coords = material->GetTextureCoords();

  data.quad_vertex_buffer_ptr->position = vertex_positions[0];
  data.quad_vertex_buffer_ptr->color = material->GetColor();
  data.quad_vertex_buffer_ptr->tex_coord = {texture_coords.x1,
                                            texture_coords.y1};
  data.quad_vertex_buffer_ptr->tex_index = texture_slots_index;
  data.quad_vertex_buffer_ptr++;

  data.quad_vertex_buffer_ptr->position = vertex_positions[1];
  data.quad_vertex_buffer_ptr->color = material->GetColor();
  data.quad_vertex_buffer_ptr->tex_coord = {texture_coords.x2,
                                            texture_coords.y2};
  data.quad_vertex_buffer_ptr->tex_index = texture_slots_index;
  data.quad_vertex_buffer_ptr++;

  data.quad_vertex_buffer_ptr->position = vertex_positions[2];
  data.quad_vertex_buffer_ptr->color = material->GetColor();
  data.quad_vertex_buffer_ptr->tex_coord = {texture_coords.x3,
                                            texture_coords.y3};
  data.quad_vertex_buffer_ptr->tex_index = texture_slots_index;
  data.quad_vertex_buffer_ptr++;

  data.quad_vertex_buffer_ptr->position = vertex_positions[3];
  data.quad_vertex_buffer_ptr->color = material->GetColor();
  data.quad_vertex_buffer_ptr->tex_coord = {texture_coords.x4,
                                            texture_coords.y4};
  data.quad_vertex_buffer_ptr->tex_index = texture_slots_index;
  data.quad_vertex_buffer_ptr++;

  data.quad_index_count += 6;
#ifdef CR_EXT_RENDER_STATS
  data.stats.quad_count++;
#endif
}

#ifdef CR_EXT_RENDER_STATS
void Renderer2D::ResetStats() { memset(&data.stats, 0, sizeof(Statistics)); }
const Statistics& Renderer2D::GetStats() { return data.stats; }
#else
void Renderer2D::ResetStats() {}
const Statistics& Renderer2D::GetStats() { return data.stats; }
#endif

}  // namespace cosair