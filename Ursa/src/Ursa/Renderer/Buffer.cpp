#include "ursapch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ursa {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			URSA_CORE_ASSERT(false, "RendererAPI::None is currently noy supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		URSA_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			URSA_CORE_ASSERT(false, "RendererAPI::None is currently noy supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}
		URSA_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}