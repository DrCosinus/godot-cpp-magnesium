#include "register_types.hpp"

#include "godot_cpp/core/class_db.hpp"

#include "indexed_material2d.hpp"
#
namespace experimental
{
	void register_types()
	{
		GDREGISTER_CLASS(IndexedMaterial2D);
	}

	void unregister_types()
	{
	}
} //namespace experimental
