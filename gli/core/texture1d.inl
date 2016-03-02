#include "../levels.hpp"

namespace gli
{
	inline texture1d::texture1d()
	{}

	inline texture1d::texture1d(format_type Format, extent_type const& Extent, swizzles_type const& Swizzles)
		: texture(TARGET_1D, Format, texture::extent_type(Extent.x, 1, 1), 1, 1, gli::levels(Extent), Swizzles)
	{
		this->build_cache();
	}

	inline texture1d::texture1d(format_type Format, extent_type const& Extent, size_type Levels, swizzles_type const& Swizzles)
		: texture(TARGET_1D, Format, texture::extent_type(Extent.x, 1, 1), 1, 1, Levels, Swizzles)
	{
		this->build_cache();
	}

	inline texture1d::texture1d(texture const & Texture)
		: texture(Texture, TARGET_1D, Texture.format())
	{
		this->build_cache();
	}

	inline texture1d::texture1d
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel,
		swizzles_type const & Swizzles
	)
		: texture(
			Texture, TARGET_1D,
			Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel,
		Swizzles)
	{
		this->build_cache();
	}
 
	inline texture1d::texture1d
	(
		texture1d const & Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_1D,
			Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline image texture1d::operator[](texture1d::size_type Level) const
	{
		GLI_ASSERT(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture1d::extent_type texture1d::extent(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(Level)].ImageExtent;
	}

	template <typename gen_type>
	inline gen_type texture1d::load(extent_type const& TexelCoord, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Level)];

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		return *(this->data<gen_type>(0, 0, Level) + ImageOffset);
	}

	template <typename gen_type>
	inline void texture1d::store(extent_type const& TexelCoord, size_type Level, gen_type const& Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(gen_type));

		cache const& Cache = this->Caches[this->index_cache(Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.ImageExtent)));

		size_type const ImageOffset = this->Storage->image_offset(TexelCoord, Cache.ImageExtent);
		GLI_ASSERT(ImageOffset < this->size<gen_type>(Level));

		*(this->data<gen_type>(0, 0, Level) + ImageOffset) = Texel;
	}

	inline texture1d::size_type texture1d::index_cache(size_type Level) const
	{
		return Level;
	}

	inline void texture1d::build_cache()
	{
		this->Caches.resize(this->levels());

		for (size_type LevelIndex = 0, LevelCount = this->levels(); LevelIndex < LevelCount; ++LevelIndex)
		{
			cache& Cache = this->Caches[this->index_cache(LevelIndex)];
			Cache.ImageExtent = glm::max(extent_type(this->texture::extent(LevelIndex)), extent_type(1));
		}
	}
}//namespace gli
