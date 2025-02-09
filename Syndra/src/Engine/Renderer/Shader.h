#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Syndra {

	struct PCMember {
		std::string name;
		size_t size;
	};

	struct PushConstant
	{
		std::string name;
		uint32_t size;
		std::vector<PCMember> members;
	};

	struct Sampler
	{
		std::string name;
		uint32_t set;
		uint32_t binding;
		bool isUsed;
	};
	
	enum class MemoryBarrierMode
	{
		image, vertex
	};

	class Shader
	{
	public:

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		//-----------Compute Shaders----------//
		virtual void DispatchCompute(uint32_t x, uint32_t y, uint32_t z) = 0;
		virtual void SetMemoryBarrier(MemoryBarrierMode mode) = 0;
		//------------------------------------//

		virtual std::vector<PushConstant> GetPushConstants() = 0;
		virtual std::vector<Sampler> GetSamplers() = 0;

		virtual const std::string& GetName() const = 0;
		virtual void Reload() = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		std::unordered_map<std::string, Ref<Shader>> GetShaders() { return m_Shaders; }
		
		void DeleteShader(const Ref<Shader>& shader);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}