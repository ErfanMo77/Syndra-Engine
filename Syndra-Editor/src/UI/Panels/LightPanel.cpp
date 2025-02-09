#include "lpch.h"
#include "LightPanel.h"

namespace Syndra {

	LightPanel::LightPanel()
	{
	}

	void LightPanel::DrawLight(Entity& entity)
	{
		static bool LightRemoved = false;
		if (UI::DrawComponent<LightComponent>(ICON_FA_LIGHTBULB" Light", entity, true, &LightRemoved)) {
			auto& component = entity.GetComponent<LightComponent>();
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 80);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::Text("LightType\0");

			ImGui::PopStyleVar();
			ImGui::NextColumn();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			std::string label = LightTypeToLightName(component.type);

			static int item_current_idx = 0;                    // Here our selection data is an index.
			const char* combo_label = label.c_str();				// Label to preview before opening the combo (technically it could be anything)
			if (ImGui::BeginCombo("##Lights", combo_label))
			{
				for (int n = 0; n < 4; n++)
				{
					const bool is_selected = (item_current_idx == n);

					if (ImGui::Selectable(LightTypeToLightName((LightType)n).c_str(), is_selected)) {
						component.type = (LightType)n;
						if (component.type == LightType::Point) {
							component.light = CreateRef<PointLight>(component.light->GetColor());
						}
						if (component.type == LightType::Directional) {
							component.light = CreateRef<DirectionalLight>(component.light->GetColor());
						}
						if (component.type == LightType::Spot) {
							component.light = CreateRef<SpotLight>(component.light->GetColor());
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::Columns(1);

			ImGui::Separator();

			auto& color4 = glm::vec4(component.light->GetColor(), 1);

			ImGui::Columns(2,0,false);
			ImGui::SetColumnWidth(0, 80);
			ImGui::AlignTextToFramePadding();
			//light's color
			ImGui::Text("Color\0");
			ImGui::SameLine();
			ImGui::NextColumn();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGuiColorEditFlags colorFlags = ImGuiColorEditFlags_HDR;
			ImGui::ColorEdit4("##color", glm::value_ptr(color4), colorFlags);
			ImGui::PopItemWidth();
			ImGui::Columns(1);
			component.light->SetColor(glm::vec3(color4));

			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 80);
			ImGui::AlignTextToFramePadding();
			//light's intensity
			float intensity = component.light->GetIntensity();
			ImGui::Text("Intensity\0");
			ImGui::SameLine();
			ImGui::NextColumn();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::DragFloat("##Intensity", &intensity, 0.1, 0, 100);
			ImGui::PopItemWidth();
			ImGui::Columns(1);
			component.light->SetIntensity(intensity);

			auto PI = glm::pi<float>();

			if (component.type == LightType::Directional) {
				auto p = dynamic_cast<DirectionalLight*>(component.light.get());
				auto dir = p->GetDirection();
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 80);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Direction\0");
				ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
				ImGui::SliderFloat3("##direction", glm::value_ptr(dir), -2 * PI, 2 * PI, "%.3f");
				ImGui::PopItemWidth();
				ImGui::Columns(1);
				p->SetDirection(dir);
				p = nullptr;
			}

			if (component.type == LightType::Point)
			{
				auto p = dynamic_cast<PointLight*>(component.light.get());
				float range = p->GetRange();

				UI::DragFloat("Range", &range);

				p->SetRange(range);
				p = nullptr;
			}

			if (component.type == LightType::Spot)
			{
				auto p = dynamic_cast<SpotLight*>(component.light.get());
				auto dir = p->GetDirection();
				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, 80);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Direction\0");
				ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
				ImGui::SliderFloat3("##direction", glm::value_ptr(dir), -2 * PI, 2 * PI, "%.3f");
				ImGui::PopItemWidth();
				ImGui::Columns(1);
				p->SetDirection(dir);

				float iCut = p->GetInnerCutOff();
				float oCut = p->GetOuterCutOff();

				UI::DragFloat("Cutoff", &iCut, 0.5f, 0, p->GetOuterCutOff() - 0.01f);
				UI::DragFloat("Outer Cutoff", &oCut, 0.5f, p->GetOuterCutOff() + 0.01f, 180);

				p->SetCutOff(iCut, oCut);
				p = nullptr;
			}

			ImGui::TreePop();
		}
		if (LightRemoved) {
			entity.RemoveComponent<LightComponent>();
			LightRemoved = false;
		}
	}

}