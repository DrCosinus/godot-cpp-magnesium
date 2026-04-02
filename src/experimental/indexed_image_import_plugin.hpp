#pragma once

#include <godot_cpp/classes/editor_import_plugin.hpp>

using namespace godot;

class IndexedImageImportPlugin : public EditorImportPlugin
{
	GDCLASS(IndexedImageImportPlugin, EditorImportPlugin);

protected:
	static void _bind_methods();

public:
	IndexedImageImportPlugin();

	String _get_importer_name() const override;
	String _get_visible_name() const override;
	PackedStringArray _get_recognized_extensions() const override;
	String _get_save_extension() const override;
	String _get_resource_type() const override;
	Error _import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const override;
};
