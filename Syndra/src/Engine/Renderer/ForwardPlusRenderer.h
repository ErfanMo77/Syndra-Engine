#pragma once
#include "RenderPipeline.h"
#include "Engine/Utils/Math.h"

namespace Syndra {

	class Entity;
	class Scene;

	class ForwardPlusRenderer : public RenderPipeline {

	public:
		virtual void Init(const Ref<Scene>& scene, const ShaderLibrary& shaders, const Ref<Environment>& env) override;

		virtual void Render() override;

		virtual void End() override;

		virtual void ShutDown() override;

		virtual void UpdateLights() override;

		virtual uint32_t GetFinalTextureID(int index) override;

		virtual uint32_t GetMouseTextureID() override;

		virtual Ref<FrameBuffer> GetMainFrameBuffer() override;

		virtual void OnImGuiRender(bool* rendererOpen, bool* environmentOpen) override;

		virtual void OnResize(uint32_t width, uint32_t height) override;
	
	private:
		void SetupLights();

	public:

		struct ShadowData {
			glm::mat4 lightViewProj;
		};

		struct VisibleIndex {
			int index;
		};

		struct pointLight
		{
			glm::vec4 position;
			glm::vec4 color;
			glm::vec4 paddingAndRadius;
		};

		struct pointLightData
		{
			pointLight lights[256];
		};

		struct spotLight {
			glm::vec4 position;
			glm::vec4 color;
			glm::vec4 direction;
			float innerCutOff;
			float outerCutOff;
			glm::vec2 padding;
		};

		struct directionalLight
		{
			glm::vec4 position;
			glm::vec4 direction;
			glm::vec4 color;
		};

		struct RenderData
		{
			//Scene object
			Ref<Scene> scene;
			//Environment
			float intensity;
			Ref<Environment> environment;
			//shaders
			ShaderLibrary shaders;
			Ref<Shader>  depthShader, postProcShader, compShader, forwardLightingShader, shadowDepthShader;
			//lighting and shadow parameters
			bool softShadow = false;
			bool disableShadow = false;
			float numPCF = 16;
			float numBlocker = 2;
			float exposure, gamma, lightSize, orthoSize, lightNear, lightFar;
			//Point lights
			uint32_t lightBuffer, visibleLightIndicesBuffer, numLights;
			uint32_t workGroupsX, workGroupsY;
			pointLightData pLights;
			//Directional light data
			Ref<UniformBuffer> ShadowBuffer;
			glm::mat4 lightProj;
			glm::mat4 lightView;
			ShadowData shadowData;
			directionalLight dirLight;
			Ref<UniformBuffer> dirLightBuffer;
			//Poisson samplers
			Ref<Texture1D> distributionSampler0, distributionSampler1;
			//Anti ALiasing
			bool useFxaa = false;
			//Render passes containing respective frameBuffers
			Ref<RenderPass> depthPass, shadowPass, lightingPass, postProcPass;
			//Scene quad VBO
			Ref<VertexArray> screenVao;
		};
	};
}