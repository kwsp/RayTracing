#pragma once
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Walnut/Image.h"
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
  Renderer() = default;

  void OnResize(uint32_t width, uint32_t height);
  void Render(const Scene &scene, const Camera &camera);

  auto GetFinalImage() const { return m_FinalImage; }
  auto GetLastRenderTime() const { return m_LastRenderTime; }

private:
  glm::vec4 TraceRay(const Scene &scene, const Ray &coord);

private:
  std::shared_ptr<Walnut::Image> m_FinalImage;
  uint32_t *m_ImageData = nullptr;
  float m_LastRenderTime = 0;
};