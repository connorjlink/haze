#ifndef HAZE_ARENAALLOCATOR_H
#define HAZE_ARENAALLOCATOR_H

// Haze ArenaAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Arena
	{
	private:
		static constexpr std::size_t DEFAULT_BLOCK_SIZE = 64 * 1024;
		static constexpr std::size_t DEFAULT_ALIGNMENT = 4 * 1024;

	private:
		std::size_t block_size;
		std::vector<std::byte*> blocks;
		std::size_t current_offset{ 0 };
		std::size_t current_block{ 0 };

	private:
		void allocate_new_block()
		{
			void* new_block = ::operator new(block_size, std::align_val_t{ DEFAULT_ALIGNMENT });

			blocks.push_back(static_cast<std::byte*>(new_block));
			current_block = blocks.size() - 1;
			current_offset = 0;
		}

	public:
		void clear()
		{
			for (auto block : blocks)
			{
				::operator delete(block, std::align_val_t{ DEFAULT_ALIGNMENT });
			}

			blocks.clear();
		}

		[[nodiscard]]
		void* allocate(std::size_t n, std::size_t alignment)
		{
			if (n > block_size)
			{
				throw std::bad_alloc();
			}

			void* block_start = blocks[current_block];
			void* pointer = static_cast<std::byte*>(block_start) + current_offset;

			const auto space = block_size - current_offset;

			if (std::align(alignment, n, pointer, space))
			{
				current_offset = (static_cast<std::byte*>(pointer) - static_cast<std::byte*>(block_start)) + n;
				return pointer;
			}

			allocate_new_block();
			return allocate(n, alignment);
		}

		void deallocate(void*, std::size_t) noexcept
		{
		}

	public:
		explicit Arena(std::size_t block_size = DEFAULT_BLOCK_SIZE)
			: block_size{ block_size }
		{
			allocate_new_block();
		}

		// noncopyable
		Arena(const Arena&) = delete;
		Arena& operator=(const Arena&) = delete;

		Arena(Arena&& other) noexcept
			: block_size{ other.block_size }
			, blocks{ std::move(other.blocks) }
			, current_block{ other.current_block }
			, current_offset{ other.current_offset }
		{
		}

		Arena& operator=(Arena&& other) noexcept
		{
			if (this != &other)
			{
				clear();
				block_size = other.block_size;
				blocks = std::move(other.blocks);
				current_block = other.current_block;
				current_offset = other.current_offset;
			}
			return *this;
		}

		~Arena()
		{ 
			clear();
		}	
	};


	template<typename T>
	struct ArenaAllocator
	{
	public:
		using value_type = T;
	
	public:
		Arena* arena;

	public:
		[[nodiscard]]
		T* allocate(std::size_t n)
		{
			if (n == 0uz)
			{
				return nullptr;
			}
			if (auto p = static_cast<T*>(arena->allocate(n * sizeof(T), alignof(T))))
			{
				return p;
			}

			throw std::bad_alloc();
		}

		void deallocate(T* p, std::size_t n) noexcept
		{
			arena->deallocate(p, n);
		}
		
		bool operator==(const ArenaAllocator& other) const noexcept
		{
			return arena == other.arena;
		}

		bool operator!=(const ArenaAllocator& other) const noexcept
		{
			return arena != other.arena;
		}

	public:
		ArenaAllocator(Arena* arent) noexcept 
			: arena{ arent }
		{
		}

		template<typename U>
		constexpr ArenaAllocator(const ArenaAllocator<U>& other) noexcept 
			: arena{ other.arena }
		{
		}
	};
}

#endif
