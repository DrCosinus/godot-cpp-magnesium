Run C++ tests (tests/cpp)

- Build and run all tests:

```powershell
scons test
```

- To only build (do not run):

```powershell
scons
```

Behavior (as defined in `SConscript`):

- A small helper `../tools/fetch_doctest.py` is executed if present to fetch `tests/vendor/doctest.h`.
- All source files matching `test_*.cpp` are discovered automatically.
- Test binaries are built using a cloned environment (`env.Clone()`) to avoid pulling unrelated project sources.
- Compilation macros are added: `EXPERIMENTAL_UNIT_TEST` and `TEST_USE_MOCK_GODOT_CPP` (tests do not require the Godot runtime).
- The include path `../include` is prepended so test mocks take precedence.
- Linked libraries are cleared (`LIBS` / `LIBPATH` replaced) to avoid linking the project runtime into test executables.
- On Windows, MSVC flag `/EHsc` is added to avoid warning C4530.
- If a prebuilt extension library (`bin/<platform>/Magnesium*`) is found, its path is printed for diagnostics but it is not linked into test executables (avoids symbol conflicts).
- Executables are placed in a hidden build directory: `../.build/tests/<platform>` (or `host` if unspecified).
- A SCons `test` alias is created; after build, each executable is launched automatically. The script tries several strategies to locate the built executables and prints helpful diagnostics if an executable cannot be found.

Error behavior:

- If a test executable exits with a non-zero code, `scons test` will return that code.
- If an expected executable is not found, diagnostic information (paths, directory listings, glob matches) is printed to aid debugging.

Notes:

- Keep `tests/vendor/doctest.h` up to date if you change how it is fetched.
- If you encounter build or linkage errors, ensure submodules and the build environment are configured as when building the extension.

Mocks

- Location: `tests/include/godot_cpp` — minimal mock headers that provide the subset of the Godot C++ API used by the tests (examples: `classes/image.hpp`, `classes/image_texture.hpp`, `classes/ref.hpp`, `variant/packed_byte_array.hpp`).
- How they are used: the test SCons script prepends `../include` to the include path, and the test build defines `TEST_USE_MOCK_GODOT_CPP`, so includes like `<godot_cpp/classes/image.hpp>` resolve to these mock headers during test compilation.
- Purpose: allow tests to build and run as standalone native executables without requiring the full Godot runtime or the upstream `godot-cpp` bindings; mocks implement only the small API surface needed by tests.
- Extending mocks: to add coverage for more code paths, create additional headers under `tests/include/godot_cpp/...` mirroring the real include layout and implement the minimal behaviour your tests need.
- Tips: keep mock implementations lightweight and focused; prefer simple value-like types or small helpers (see `tests/include/godot_cpp` for examples).

For troubleshooting, run `scons test` and inspect the printed diagnostics for paths and matches.

French to follow — Français ci-dessous

Exécution des tests C++ (tests/cpp)

- Construire et lancer tous les tests:

```powershell
scons test
```

- Pour ne construire que (sans exécuter) :

```powershell
scons
```

Comportement important (tel que défini dans `SConscript`):

- Un petit script `../tools/fetch_doctest.py` est exécuté si présent pour récupérer `tests/vendor/doctest.h`.
- Tous les fichiers sources correspondant à `test_*.cpp` sont découverts automatiquement.
- Les binaires de test sont construits avec un environnement cloné (`env.Clone()`) pour éviter d'emporter d'autres sources du projet.
- Des macros de compilation sont ajoutées: `EXPERIMENTAL_UNIT_TEST` et `TEST_USE_MOCK_GODOT_CPP` (tests n'ont pas besoin du runtime Godot).
- Le chemin d'includes `../include` est préfixé afin que les mocks de test soient prioritaires.
- Les bibliothèques linkées sont remplacées (pas de `LIBS` / `LIBPATH`) pour éviter de lier le runtime du projet dans les exécutables de test.
- Sur Windows, le flag MSVC `/EHsc` est ajouté pour éviter l'avertissement C4530.
- Si une bibliothèque d'extension pré-construite (`bin/<platform>/Magnesium*`) est trouvée, son chemin est affiché à titre diagnostique mais elle n'est pas liée aux exécutables de test (évite les conflits de symboles).
- Les exécutables sont placés dans un répertoire caché de build: `../.build/tests/<platform>` (ou `host` si non spécifié).
- Une cible SCons `test` est créée; après la construction, chaque exécutable est lancé automatiquement. Le script essaie plusieurs stratégies pour localiser les exécutables construits et affiche des diagnostics utiles si un exécutable est introuvable.

Comportement en cas d'erreur:

- Si un exécutable échoue (code de sortie != 0), `scons test` renverra le même code d'erreur.
- Si un exécutable attendu n'est pas trouvé, des informations de diagnostic (chemins, listings, glob matches) sont imprimées pour aider au débogage.

Remarques:

- Conservez `tests/vendor/doctest.h` à jour si vous modifiez la méthode de récupération.
- Si vous avez des erreurs d'édition ou de linkage, vérifiez que les sous-modules et l'environnement de build sont configurés comme pour la compilation de l'extension.

Mocks (mocks pour les tests)

- Emplacement : `tests/include/godot_cpp` — en-têtes mocks fournissant le sous-ensemble minimal de l'API Godot C++ utilisé par les tests (ex. : `classes/image.hpp`, `classes/image_texture.hpp`, `classes/ref.hpp`, `variant/packed_byte_array.hpp`).
- Utilisation : le SConscript de tests préfixe `../include` au chemin d'inclusion, et la build de tests définit `TEST_USE_MOCK_GODOT_CPP`, donc les includes comme `<godot_cpp/classes/image.hpp>` résolvent vers ces headers mocks pendant la compilation des tests.
- But : permettre aux tests d'être compilés et exécutés comme des exécutables natifs autonomes sans nécessiter le runtime Godot complet ni les bindings `godot-cpp` upstream ; les mocks implémentent uniquement la surface d'API minimale requise.
- Extension : pour couvrir davantage de chemins de code, ajoutez des headers sous `tests/include/godot_cpp/...` en miroir de la hiérarchie d'includes réelle et implémentez le comportement minimal nécessaire pour vos tests.
- Conseils : gardez les implémentations des mocks légères et ciblées ; préférez des types simples ou de petits helpers (voir `tests/include/godot_cpp` pour des exemples).

Pour tout dépannage, exécutez `scons test` et consultez la sortie imprimée pour les chemins et diagnostics.
