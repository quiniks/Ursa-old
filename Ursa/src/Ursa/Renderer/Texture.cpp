#include "ursapch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ursa {
	Ref<Texture2D> Ursa::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			URSA_CORE_ASSERT(false, "RendererAPI::None is currently noy supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}
		URSA_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}