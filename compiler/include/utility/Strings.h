#ifndef HAZE_STRINGS_H
#define HAZE_STRINGS_H

// Haze Strings.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	concept IsString = std::convertible_v<T, std::string_view>;


	template<typename AllocatorT = std::allocator<char>>
	struct StringInterner
	{
	private:
		using SetAllocatorT = typename std::allocator_traits<AllocatorT>::template rebindallocator<
			std::pair<const StringType, StringType>>;
		using StringType = std::basic_string<char, std::char_traits<char>, AllocatorT>;

		struct StringViewHash
		{
			// heterogenous lookup indicator for the STL
			using is_transparent = void;
			
			std::size_t operator()(std::string_view view) const noexcept
			{
				return std::hash<std::string_view>{}(view);
			}
			
			std::size_t operator()(const StringType& string) const noexcept
			{
				return std::hash<std::string_view>{}(std::string_view{ string });
			}
		};

		struct StringViewEqual
		{
			// heterogenous lookup indicator for the STL
			using is_transparent = void;
			
			bool operator()(std::string_view left, std::string_view right) const noexcept
			{
				return left == right;
			}

			bool operator()(const StringType& left, std::string_view right) const noexcept
			{
				return std::string_view(left) == right;
			}
			
			bool operator()(std::string_view left, const StringType& right) const noexcept
			{
				return left == std::string_view(right);
			}
		};

	private:
		std::unordered_set<StringType, StringViewHash, StringViewEqual, SetAllocatorT> storage;
		AllocatorT allocator;
		
	public:
		template<IsString T>
		std::string_view intern(T&& string)
		{
			const auto view = std::string_view{ std::forward<T>(string) };

			if (const auto it = storage.find(view); it != storage.end())
			{
				return std::string_view{ *it };
			}

			const auto [it, success] = storage.emplace(StringType{ view, allocator });
			return std::string_view{ *it };
		}
		
		template<typename Self>
			requires std::derived_from<std::remove_cvref_t<Self>, StringInterner>
		bool contains(this Self&& self, std::string_view view) const noexcept
		{
			return self.storage.contains(view);
		}
		
		std::size_t size() const noexcept
		{
			return storage.size();
		}
		
		[[nodiscard]] bool empty() const noexcept
		{
			return storage.empty();
		}
		
		void clear() noexcept
		{
			storage.clear();
		}
	
	public:
		explicit StringInterner(const AllocatorT& allocator = Allocator{})
			: storage{ SetAllocator{ allocator } }, allocator{ allocator }
		{
		}

		// noncopyable
		StringInterner(const StringInterner&) = delete;
		StringInterner& operator=(const StringInterner&) = delete;

		StringInterner(StringInterner&&) noexcept = default;
		StringInterner& operator=(StringInterner&&) noexcept = default;
	};
}

#endif
