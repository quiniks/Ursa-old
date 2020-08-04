#include "ursapch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ursa {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				URSA_CORE_ASSERT(false, "RendererAPI::None is currently noy supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}
		URSA_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}