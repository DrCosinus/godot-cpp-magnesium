#pragma once

#include "serializable.hpp"
#include <cstdint>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace godot
{
	class PackedByteArray;
}

namespace experimental
{
	struct Serializer
	{
		Serializer(bool writing) : is_writing(writing) {}
		virtual ~Serializer() = default;

		bool IsWriting() const { return is_writing; }

		virtual bool ProcessInt8(godot::StringName name, int8_t& value) = 0;
		virtual bool ProcessInt16(godot::StringName name, int16_t& value) = 0;
		virtual bool ProcessInt32(godot::StringName name, int32_t& value) = 0;
		virtual bool ProcessInt64(godot::StringName name, int64_t& value) = 0;

		virtual bool ProcessPackedByteArray(godot::StringName name, godot::PackedByteArray& value, size_t& size) = 0;

		// virtual bool ProcessImpl(const SerializerDataType& data_type, void* value_ptr, const char* metadata = nullptr);

		// bool Process(const SerializerDataType& data_type, void* value_ptr, const char* metadata = nullptr)
		// {
		// 	const bool changed = ProcessImpl(data_type, value_ptr, metadata);
		// 	DEV_ASSERT(!changed || is_writing, "Deserialization should not modify values");
		// 	return changed;
		// }

		// template <typename T>
		// bool Process(T& value, const char* metadata = nullptr)
		// {
		// 	return Process(GetSerializerDataType<T>::Get(), &value, metadata);
		// }

		// template <>
		// bool Process<int8_t>(int8_t& value) { return ProcessInt8(value); }
		// template <>
		// bool Process<int16_t>(int16_t& value) { return ProcessInt16(value); }
		// template <>
		// bool Process<int32_t>(int32_t& value) { return ProcessInt32(value); }
		// template <>
		// bool Process<int64_t>(int64_t& value) { return ProcessInt64(value); }

	protected:
		bool is_writing;
	};
} // namespace experimental
