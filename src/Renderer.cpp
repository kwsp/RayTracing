#include "Renderer.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

namespace Utils {
static uint32_t ConverToRGBA(const glm::vec4 &color) {
  uint8_t r = (color.r * 255.0f);
  uint8_t g = (color.g * 255.0f);
  uint8_t b = (color.b * 255.0f);
  uint8_t a = (color.a * 255.0f);
  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}
} // namespace Utils

void Renderer::OnResize(uint32_t width, uint32_t height) {
  if (m_FinalImage) {
    // No resize necessary
    if (m_FinalImage->GetWidth() == width &&
        m_FinalImage->GetHeight() == height)
      return;

    m_FinalImage->Resize(width, height);
  } else {
    m_FinalImage = std::make_shared<Walnut::Image>(width, height,
                                                   Walnut::ImageFormat::RGBA);
  }

  delete[] m_ImageData;
  m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Scene &scene, const Camera &camera) {
  Walnut::Timer timer;

  Ray ray;
  ray.Origin = camera.GetPosition();

  for (uint32_t y = 0; y < m_FinalImage->GetHeight(); ++y) {
    for (uint32_t x = 0; x < m_FinalImage->GetWidth(); ++x) {
      ray.Direction =
          camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
      glm::vec4 color = TraceRay(scene, ray);
      color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
      m_ImageData[y * m_FinalImage->GetWidth() + x] =
          Utils::ConverToRGBA(color);
    }
  }

  m_FinalImage->SetData(m_ImageData);

  m_LastRenderTime = timer.ElapsedMillis();
}

glm::vec4 Renderer::TraceRay(const Scene &scene, const Ray &ray) {

  if (scene.Spheres.size() == 0)
    return glm::vec4(0, 0, 0, 1);

  // (bx2 + by2)t2 + (2(axbx + ayby))t + (ax2 + ay2 - r2) = 0
  // a t2 + b t + c = 0

  // a = ray origin
  // b = ray direction
  // r = radius of sphere
  // t = hit distance

  const Sphere *closestSphere = nullptr;
  float hitDistance = std::numeric_limits<float>::max();
  for (const auto &sphere : scene.Spheres) {

    glm::vec3 origin = ray.Origin - sphere.Position;

    float a = glm::dot(ray.Direction, ray.Direction);
    float b = 2.0f * glm::dot(origin, ray.Direction);

    float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
      continue;

    //   float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a); // the smaller value

    if (t1 < hitDistance) {
      hitDistance = t1;
      closestSphere = &sphere;
    }
  }

  if (closestSphere == nullptr)
    return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  glm::vec3 origin = ray.Origin - closestSphere->Position;
  //   glm::vec3 h0 = rayOrigin + rayDirection * t0;
  glm::vec3 hitPoint = origin + ray.Direction * hitDistance; // the closer hit

  glm::vec3 normal = glm::normalize(hitPoint);

  glm::vec3 lightDir = glm::vec3(-1.0f, -1.0f, -1.0f);

  float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(angle)

  glm::vec3 sphereColor = closestSphere->Albedo;
  //   sphereColor = normal * 0.5f + 0.5f;
  sphereColor *= d;
  return glm::vec4(sphereColor, 1);
}