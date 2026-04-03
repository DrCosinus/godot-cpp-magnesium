#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

class IndexedImageEditorPlugin : public godot::EditorPlugin
{
	GDCLASS(IndexedImageEditorPlugin, EditorPlugin);

protected:
	static void _bind_methods()
	{
	}

public:
	godot::String _get_plugin_name() const override
	{
		return "IndexedImage Importer";
	}
	godot::String get_plugin_version() const
	{
		return "1.0";
	}
	void _enter_tree() override;
	void _exit_tree() override;
};
