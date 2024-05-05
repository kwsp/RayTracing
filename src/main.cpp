#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Timer.h"
#include <chrono>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"

class ExampleLayer : public Walnut::Layer {
public:
  ExampleLayer() : m_Camera(45.0f, 0.1f, 100.0f) {
    {
      Sphere sphere;
      sphere.Position = {0.0f, 0.0f, 0.0f};
      sphere.Radius = 0.5f;
      sphere.Albedo = {1.0f, 0.0f, 1.0f};
      m_Scene.Spheres.push_back(sphere);
    }
    {
      Sphere sphere;
      sphere.Position = {1.0f, 0.0f, -5.0f};
      sphere.Radius = 1.5f;
      sphere.Albedo = {0.2f, 0.3f, 1.0f};
      m_Scene.Spheres.push_back(sphere);
    }
  }

  virtual void OnUpdate(float ts) override { m_Camera.OnUpdate(ts); }

  virtual void OnUIRender() override {

    const auto time = std::chrono::high_resolution_clock::now();
    const auto fps =
        1000 / (float)std::chrono::duration_cast<std::chrono::milliseconds>(
                   time - m_LastFrameTime)
                   .count();
    m_LastFrameTime = time;

    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3fms", m_Renderer.GetLastRenderTime());
    ImGui::Text("FPS: %.3fms", fps);
    if (ImGui::Button("Render")) {
      Render();
    }
    ImGui::End();

    {
      ImGui::Begin("Scene");

      for (int i = 0; i < m_Scene.Spheres.size(); ++i) {
        auto &sphere = m_Scene.Spheres[i];
        ImGui::PushID(i);
        ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
        ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
        ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo), 0.1f);
        ImGui::Separator();
        ImGui::PopID();
      }
      ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    m_ViewportWidth = ImGui::GetContentRegionAvail().x;
    m_ViewportHeight = ImGui::GetContentRegionAvail().y;

    {
      const auto image = m_Renderer.GetFinalImage();
      if (image)
        ImGui::Image(image->GetDescriptorSet(),
                     {(float)image->GetWidth(), (float)image->GetHeight()},
                     ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();

    Render();
  }

  void Render() {
    m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
    m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
    m_Renderer.Render(m_Scene, m_Camera);
  }

private:
  float m_ViewportWidth;
  float m_ViewportHeight;
  std::chrono::steady_clock::time_point m_LastFrameTime{};

  Renderer m_Renderer;
  Camera m_Camera;
  Scene m_Scene;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
  Walnut::ApplicationSpecification spec;
  spec.Name = "Walnut Example";

  Walnut::Application *app = new Walnut::Application(spec);
  app->PushLayer<ExampleLayer>();
  app->SetMenubarCallback([app]() {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        app->Close();
      }
      ImGui::EndMenu();
    }
  });
  return app;
}