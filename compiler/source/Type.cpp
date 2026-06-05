import std;

#include <error/CommonErrors.h>
#include <type/Type.h>

// Haze Type.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	//////////////////////////////////////////////////////
	// VoidType
	//////////////////////////////////////////////////////

	Offset VoidType::size() const
	{
		return 0;
	}

	bool VoidType::is_complete() const
	{
		return false;
	}


	//////////////////////////////////////////////////////
	// IntType
	//////////////////////////////////////////////////////

	Offset IntType::size() const
	{
		switch (int_kind)
		{
#define SWITCH_CASE(enumerator, name) case IntKind::enumerator: return sizeof(name);
			INT_KINDS(SWITCH_CASE)
#undef SWITCH_CASE
		}

		CommonErrors::invalid_int_type(int_kind, NULL_TOKEN);
		return -1;
	}

	bool IntType::is_complete() const
	{
		return true;
	}


	//////////////////////////////////////////////////////
	// FloatKind
	//////////////////////////////////////////////////////

	Offset FloatType::size() const
	{
		switch (float_kind)
		{
#define X(enumerator, name) case FloatKind::enumerator: return sizeof(name);
			FLOAT_KINDS(X)
#undef X
		}

		CommonErrors::invalid_float_type(float_kind, NULL_TOKEN);
		return -1;
	}

	bool FloatType ::is_complete() const
	{
		return true;
	}


	//////////////////////////////////////////////////////
	// StructOrUnionType
	//////////////////////////////////////////////////////
	
	Offset StructOrUnionType::size() const
	{
		if (!members.has_value())
		{
			CommonErrors::invalid_struct_or_union_type(struct_or_union_kind, NULL_TOKEN);
			return -1;
		}

		using enum StructOrUnionKind;
		switch (struct_or_union_kind)
		{
			case STRUCT:
			{
				// don't know here what the alignment is, so struct size is just the largest member's offset + its size
				Offset running_count{};
				for (const auto& member : members.value())
				{
					const auto member_size = member.type.size();
					const auto candidate = member.offset + member_size;

					if (candidate > running_count)
					{
						running_count = candidate;
					}
				}

				return running_count;
			} break;

			case UNION:
			{
				// union size is just the size of its largest member
				Offset running_count{};
				for (const auto& member : members.value())
				{
					const auto member_size = member.type.size();

					if (member_size > running_count)
					{
						running_count = member_size;
					}
				}

				return running_count;
			} break;

			default:
			{
				CommonErrors::invalid_struct_or_union_type(struct_or_union_kind, NULL_TOKEN);
				return -1;
			} break;
		}
	}

	bool StructOrUnionType::is_complete() const
	{
		return members.has_value();
	}


	//////////////////////////////////////////////////////
	// EnumType
	//////////////////////////////////////////////////////

	Offset EnumType::size() const
	{
		// C (until C23?) doesn't support specifying the underlying type of enumerators, so no need to check
		return sizeof(EnumMember::value);
	}

	bool EnumType::is_complete() const
	{
		return members.has_value();
	}


	//////////////////////////////////////////////////////
	// TypedefNameType
	//////////////////////////////////////////////////////

	Offset TypedefNameType::size() const
	{
		return type.size();
	}

	bool TypedefNameType::is_complete() const
	{
		return type.is_complete();
	}


	//////////////////////////////////////////////////////
	// PointerType
	//////////////////////////////////////////////////////

	Offset PointerType::size()
	{
		return sizeof(Address);
	}

	bool PointerType::is_complete() const
	{
		// pointer has a fixed size, so it's always complete to allow for things like void*
		return true;
	}


	//////////////////////////////////////////////////////
	// ArrayType
	//////////////////////////////////////////////////////

	Offset ArrayType::size() const
	{
		if (!length.has_value())
		{
			return PointerType::size();
		}

		return element_type.size() * length.value();
	}

	bool ArrayType::is_complete() const
	{
		// an array is incomplete if it doesn't have a length yet
		// this does not work for functions since they decay to pointers as parameters
		if (!length.has_value())
		{
			return false;
		}

		return element_type.is_complete();
	}
	
	//////////////////////////////////////////////////////
	// FunctionType
	//////////////////////////////////////////////////////

	Offset FunctionType::size()
	{
		return PointerType::size();
	}

	bool FunctionType::is_complete() const
	{
		// this is basically impossible for functions, but it is closer to complete since sizeof() works
		return true;
	}
}