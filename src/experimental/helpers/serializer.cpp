#include "serializer.hpp"

namespace experimental
{
	// bool Serializer::ProcessImpl(const SerializerDataType& data_type, void* value_ptr, const char* metadata)
	// {
	//     switch (data_type.base_type)
	//     {
	//         case SerializerDataType::BaseType::Serializable:
	//         {
	//             data_type.type_info
	//             Serializable* obj = reinterpret_cast<Serializable*>(value_ptr);
	//             if (obj)
	//             {
	//                 if (IsWriting())
	//                 {
	//                     obj->serialize(*this);
	//                     return true; // value was "changed" in the sense that it was written to the stream
	//                 }
	//                 else
	//                 {
	//                     // for reading, we assume the object knows how to deserialize itself from the stream in the correct order
	//                     obj->serialize(*this);
	//                     return true; // value was changed since we read new data into it
	//                 }
	//             }
	//             return false; // null pointer, nothing to process
	//         }
	//         case SerializerDataType::BaseType::None:
	//             return false; // nothing to process
	//         case SerializerDataType::BaseType::Int8:
	//             return ProcessInt8(*reinterpret_cast<int8_t*>(value_ptr));
	//         case SerializerDataType::BaseType::Int16:
	//             return ProcessInt16(*reinterpret_cast<int16_t*>(value_ptr));
	//         case SerializerDataType::BaseType::Int32:
	//             return ProcessInt32(*reinterpret_cast<int32_t*>(value_ptr));
	//         case SerializerDataType::BaseType::Int64:
	//             return ProcessInt64(*reinterpret_cast<int64_t*>(value_ptr));
	//         // add more cases for other types as needed
	//         default:
	//             break;
	//     }
	//     // default implementation doesn't know how to handle any types, so just return false (not changed)
	//     return false;
	// }

	// bool Serializer::ProcessInt8(int8_t& value)
	// {
	//     return ProcessImpl(SerializerDataType(SerializerDataType::BaseType::Int8), &value);
	// }

	// bool Serializer::ProcessInt16(int16_t& value)
	// {
	//     return ProcessImpl(SerializerDataType(SerializerDataType::BaseType::Int16), &value);
	// }

	// bool Serializer::ProcessInt32(int32_t& value)
	// {
	//     return ProcessImpl(SerializerDataType(SerializerDataType::BaseType::Int32), &value);
	// }

	// bool Serializer::ProcessInt64(int64_t& value)
	// {
	//     return ProcessImpl(SerializerDataType(SerializerDataType::BaseType::Int64), &value);
	// }
} //namespace experimental