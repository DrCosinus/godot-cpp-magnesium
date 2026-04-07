#pragma once

namespace experimental
{
	/*
	struct SerializerDataType
	{
		enum BaseType
		{
			None,

			Int8,
			Int16,
			Int32,
			Int64,

			Serializable,
			SerializablePointer, // for pointers to Serializable objects, to handle nullability and avoid slicing issues
			// add more types as needed
		};

		SerializerDataType(BaseType type, const void* type_info = nullptr) : base_type{ type }, type_info{ type_info } {}
		BaseType base_type;
		const void* type_info{ nullptr }; // for complex types, can hold additional info like size, member types, etc.
	};

	struct SerializableVTable
	{
		void (*serialize)(const Serializable& obj, Serializer& serializer);
	};

	template <typename T>
	struct GetSerializerDataType
	{
		static SerializerDataType Get() { return T::template GetSerializerDataType<void>(); }
	};
*/
	struct Serializer;

	struct Serializable
	{
		virtual ~Serializable() = default;

		virtual void serialize(Serializer& serializer) = 0;
	};
} //namespace experimental
