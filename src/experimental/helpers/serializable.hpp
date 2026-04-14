#pragma once

namespace experimental
{
	struct Serializer;

	struct Serializable
	{
		virtual ~Serializable() = default;

		virtual void serialize(Serializer& serializer) = 0;
	};
} //namespace experimental
