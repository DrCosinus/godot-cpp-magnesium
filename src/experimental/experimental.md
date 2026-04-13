What it is
What it was
and What it will be

# Currently

## Serializable

An abstract class providing a abstract method `Serialize` to serialize/deserialize symetrically and not have to write a writer and a reader with the risk of mismatches.

## Serializer

An abstract class providing some abstract methods `ProcessXXX` to read/write (regardly the kind of serializer). 
For now XXX can be Int8, Int16, Int32, Int64 and PackedByteArray.
Serializer has the IsWriting() method so the Serialize method can determine if the is a writing or reading operations.

### PackedByteArrayReader & PackedByteArrayWriter

They both implement `Serializer`.

## PalettizedImage

- A dedicated resource type: `PalettizedImage` ('resources/palettized_image.hpp')
- It inherits from `Serializable`, to ease load/save, read/write.
- Dedicated resource loader and save types: `PalettizedImageLoader`('readers/palettized_image_loader.hpp/.cpp') and `PalettizedImageSaver`('writers/palettized_image_saver.hpp/.cpp')

```cpp
    Ref<PalettizedImageLoader> palettized_image_loader = memnew(PalettizedImageLoader);
    ResourceLoader::get_singleton()->add_resource_format_loader(palettized_image_loader);

    Ref<PalettizedImageSaver> palettized_image_saver = memnew(PalettizedImageSaver);
    ResourceSaver::get_singleton()->add_resource_format_saver(palettized_image_saver);
```

### PalettizedImageLoader

It relies on the fact that `PalettizedImage` is a Serializable.

### PalettizedImageSaver

## PalettizedImageEditorPlugin

THE plugin. It will add the dedicated importer for `PalettizedMaterial`. Indeed, it doesn't limit itself to importing `PalettizedMaterial`, so its name is questionable.

## PalettizedImageImporter

It imports from indexed colors PNG files (for now) to `PalettizedImage`... It uses 

## PalettizedMaterial 

- A dedicated Material: `PalettizedMaterial` ('resources/palettized_material.cpp/.hpp')

# Future



