#include "ursapch.h"
#include "SubTexture2D.h"
namespace Ursa {
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2 max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& tile, const glm::vec2& tileSize)
	{
		glm::vec2 min = { (tile.x * tileSize.x) / texture->GetWidth(), (tile.y * tileSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((tile.x + 1) * tileSize.x) / texture->GetWidth(), ((tile.y + 1) * tileSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}
