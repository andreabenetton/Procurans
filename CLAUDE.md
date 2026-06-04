# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Procurans is a Qt 6.5 LTS / C++17 desktop utility that imports Italian electronic invoices (*fatturazione elettronica* / *FatturaPA* XML) and writes the parsed data into OpenDocument spreadsheet (`.ods`) files used for Italian bookkeeping. Vocabulary in code and UI is Italian (`elencoFatture`, `mastriniFornitori`, `primaNota`, `scadenziario`, `fattureElettroniche`, `naturaType`, etc.). License: AGPL-3.0.

## Build

CMake top-level project (`cmake_minimum_required(3.21)` — Qt 6.5 LTS floor); out-of-source build expected (the `.gitignore` excludes `build/`).

```sh
# Linux (Fedora 43: dnf install qt6-qtbase-devel zlib-ng-compat-devel)
cmake -S . -B build
cmake --build build -j

# Tests (Qt6Test, gated on BUILD_TESTING which CTest sets ON by default)
ctest --test-dir build --output-on-failure

# Resulting binary
./build/main/Procurans            # GUI; optional positional arg = .xml invoice
./build/main/Procurans --debug    # adds QtDebugMsg entries to Procurans.log
```

Windows builds use `CMakeSettings.json` (Visual Studio + Ninja, MSVC x64, Qt at `C:\Qt\6.5.0\msvc2019_64\` — Qt 6.5 LTS supports both VS 2019 and VS 2022 toolchains). On Windows the bundled `zlib/` is built; on Unix `find_package(ZLIB)` is required instead — `CMakeLists.txt:9-19` selects which.

Required Qt6 modules: `Core`, `Widgets`, `Gui`, `Xml`, `Sql`, `Test` (the last only when `BUILD_TESTING` is on). `qoasis/`, `qfatturapa/` and `quazip/` only need `Qt6Core`. **Not** required: `Qt6Core5Compat`. The vendored QuaZip (upstream v1.7.1) is configured with `QUAZIP_CAN_USE_QTEXTCODEC` unset, so its `QStringConverter` path is taken instead of `QTextCodec`.

By default everything builds as static libraries (`BUILD_STATIC_LIBRARIES=1` in top-level `CMakeLists.txt:27`, which sets `QOASIS_STATIC`, `QUAZIP_STATIC`, `ZLIB_STATIC`). Flipping that to shared makes `qoasis` define `ODS_SHARED -DODS_BUILDING` and `quazip` define `QUAZIP_BUILD`.

The build is memory-hungry under Qt6 AUTOMOC; on small AppVMs use `cmake --build build -j2` (or whatever matches the VM's vCPU count) to avoid OOM during link.

## Architecture

Four CMake subprojects, built bottom-up:

1. **`quazip/`** — upstream QuaZip v1.7.1 (LGPL 2.1) wrapping vendored minizip (`zip.c`/`unzip.c`) over zlib. Used by `qoasis` to read/write the ZIP container of `.ods` files. Treat as a third-party dependency; don't restructure it.
2. **`qoasis/`** — in-tree OpenDocument (ODF) read/write library, namespace `qoasis`. Produces `libqoasis`. This is the non-trivial output-side piece of the codebase and is where most ODS-related changes belong.
3. **`qfatturapa/`** — in-tree FatturaPA (Italian electronic-invoice) read library, namespace `qfatturapa`. Produces `libqfatturapa`. Owns the XML element-name strings, the FatturaPA code lists (`paymentMethodType` MP01–MP23, `naturaType` N1–N7 + sub-codes), and the typed `ParsedInvoice` domain model.
4. **`main/`** — the Qt Widgets GUI application (`Procurans` executable). Thin shell over `qfatturapa` (input) + `qoasis` (output).

Tests live under `qoasis/tests/` and `qfatturapa/tests/` (Qt6Test, registered with CTest).

### `qoasis` model

The library is organized as a tree of XML tags mirroring the ODF schema. Every node derives from `qoasis::Tag` (`qoasis/tag.h`), which owns its tag name, attributes, namespaces, and child `subtags_`. Subclasses override `readAttribute`, `readSubtag`, `writeAttributes`, `writeSubtags`, plus `instanceTag()` returning the static `kTag` constant. Cross-cutting behavior is layered via the `IFileable`, `INameable`, `IRepeatable`, `IStyleable` interfaces (one `i*.h/cpp` pair each at the qoasis root).

Concrete element hierarchy (each in its own file):

- `office/documentcontent.h` — root `<office:document-content>`, owns a `Body`.
- `office/body.h` → `office/spreadsheet.h` → `table/table.h` → `table/tablerow.h` + `table/tablecolumn.h` → `table/tablecell.h`.
- Cell value variants: `tablecellstring`, `tablecellfloat`, `tablecelldate`, `tablecellcurrency` (the last depends on `currency.h`).

File-level orchestration:

- `FileOds` (`qoasis/fileods.h`) is the entry point. It extracts the `.ods` ZIP into a `QTemporaryDir`, loads `content.xml` via `FileContent`/`FileXml` (which parse into a `DocumentContent` tree), and re-zips on `save()`. The temp dir is the source of truth between `load()` and `save()` — preserve `temp_dir_valid_` semantics when changing this path.
- `FileXml` reads/writes a single XML file by delegating to `Tag::write` / a tag's `read` constructor; `FileContent` is the `content.xml`-specific specialization.

When adding a new ODF element: subclass `Tag`, declare a `static const QString kTag`, register the constructor as a builder where its parent dispatches `readSubtag`, and override the four read/write hooks. Don't bypass `Tag::write` — namespace and attribute serialization rely on it. Subclass virtual signatures use `QStringView` (Qt6-native, Qt5-backport-compatible) — not `QStringRef`.

### `qfatturapa` model

The library exposes a typed `ParsedInvoice` (`qfatturapa/invoiceparser.h`) containing `InvoiceHeader{seller, document}`, `QList<InvoiceLine>`, `QList<Payment>`, `QList<SummaryRow>`. Field values are kept as `QString` — lossless from XML, no early `double`/`QDate` parsing that could hide invalid inputs. Each row-shaped struct also exposes `toFlatMap()` / `fromFlatMap()` plus a `toFlatMaps<T>()` template helper; `parseInvoice` itself keeps its shape-agnostic XML scan loop (the `addSubelementsDataToMap` helper) and converts each intermediate `QMap<QString,QString>` to the typed struct at the boundary. The flat-map bridge is what lets `MainWindow`'s `GridSchemaField`-driven grids keep consuming the legacy `QList<QMap<QString,QString>>` shape without rewriting the grid machinery.

Code-list constants (`qfatturapa::paymentMethodType()`, `qfatturapa::naturaType()`) are function-local-static Meyers singletons in `qfatturapa/codelists.cpp`. When the FatturaPA spec adds new MP* or N* codes (e.g. N2.1 / N2.2 in v1.5), update them there. The `tipoDocumento` codes are intentionally NOT modelled today — Procurans doesn't surface `TipoDocumento` in the UI.

The `parseInvoice` API is the only public entry point; the `parseHeader` / `parseDocument` / `parseDetail` / `parsePayment` / `parseSummary` helpers and the four `isStartElementNamed` / `isNotEndElementNamed` / `addSubelementsDataToMap` / `addElementDataToMap` utilities are intentionally hidden in an anonymous namespace.

### `main` (GUI) flow

- `main.cpp` installs a `Logger` (singleton, writes `Procurans.log` next to the executable), parses CLI flags (`--debug`, optional `file` positional), then shows `MainWindow`.
- `MainWindow` (`main/mainwindow.{h,cpp,ui}`) is one large class doing everything: menu/actions, calling `qfatturapa::parseInvoice` to populate a typed `ParsedInvoice`, applying a UI-only no-payment-fallback (synthesises one `Payment` from `ImportoTotaleDocumento` when `DatiPagamento` is absent), populating three `QTableView` grids (details / payments / summary) backed by `QStandardItemModel`, and producing the four output ODS reports via `executeElencoFatture/MastriniFornitori/PrimaNota/Scadenziario`. The customer-specific `bankAccount` IBAN map is still initialised in the constructor (the spec lookups now come from `qfatturapa::codelists`).
- `addHeaderToUI` consumes a `const qfatturapa::InvoiceHeader&` directly — typed field access, no string-keyed lookups. The three grid binders (`addDetailsToUI`, `addPaymentsToUI`, `addSummaryToUI`) take `const QList<QMap<QString,QString>>&` produced by `qfatturapa::toFlatMaps(parsed.details)` etc. at the call site.
- Grids are described declaratively by `GridSchemaField` lists (`gridschemafield.h`) — column name, source XML element, type (`LStringColumn`, `FloatColumn`, `DateColumn`, `RelatedColumn`, …), optional truncation and translation hash (`const QHash<QString,QString>*`, pointing at e.g. `&qfatturapa::naturaType()`). `RelatedColumn` cells use `ComboBoxItemDelegate` for editing.
- `Settings` (`main/settings.h`) is a singleton over `QSettings`: paths for the four output files + the input invoice folder, the per-report enable toggles, and main-window geometry/state. `load()`/`save()` are called from `MainWindow` open/close.
- Each `execute*` method opens the corresponding `.ods` via `qoasis::FileOds`, mutates `DocumentContent`, and saves back. Bookkeeping operations append to existing files — be careful with overwrite semantics (`save(path, overwrite_protected)`).

### Cross-cutting

- `SRC_FILE_NAME` is set as a per-file `COMPILE_DEFINITIONS` macro (see the `foreach` in each subproject's `CMakeLists.txt`) so `Logger` can tag entries with the originating source file. Preserve that loop if reorganizing sources.
- App identity macros `APP_VERSION`, `COMPANY_NAME`, `PRODUCT_NAME` come from `main/CMakeLists.txt:1-13`; bump `VERSION_MAJOR`/`MINOR`/`PATCH` there. The user-visible version is 3-part (`2.1.0`); `APP_VERSION_RC` pads to a 4-tuple (`2,1,0,0`) because Windows `VERSIONINFO` `FILEVERSION` requires exactly four integers.
- `application.qrc` bundles icons under `:/images/...`; new GUI assets go in `main/images/` and must be added to the `.qrc`. Resource compilation is wired via `qt6_add_resources`.
- Windows-only `fileinfo.rc.in` is configured into the binary for the file-properties metadata.

## Code discipline

### Simplicity first
- Minimum code that solves the problem. No speculative features, no abstractions for single-use code, no "flexibility" that wasn't requested.
- If you write 200 lines and it could be 50, rewrite it.
- No error handling for scenarios that cannot happen. Trust internal code and framework guarantees; only validate at system boundaries (user input, external APIs).

### Surgical changes
- Touch only what the task requires. Don't "improve" adjacent code, comments, or formatting opportunistically.
- Match existing patterns and style. When adding a new file, mirror the closest existing peer in the same package.
- Remove imports/variables/functions that YOUR changes made unused. Don't remove pre-existing dead code unless asked.
- Every changed line should trace directly to the task.

### Verify before declaring done
- Transform tasks into verifiable goals with concrete success criteria. "Add validation" → "write tests for invalid inputs, then make them pass".
- Run the relevant module's test suite before committing. If the task touches UI, test it in a browser or on a device. Type-checking and test suites verify code correctness, not feature correctness.
- If you cannot verify (e.g., no emulator available), say so explicitly rather than claiming success.

## Git discipline

After each logical unit of work:
- create a git commit
- push to the current branch

If push cannot be completed because of missing remote, credentials, branch protection, or environment limits:
- say so explicitly
- do not claim the push succeeded

Commit messages must be short, specific, and scoped to the actual change. Do not leave completed logical units of work uncommitted.

**Do not add a `Co-Authored-By` trailer to any commit message.**

### Multi-fix prompts
When a single prompt asks for **more than one unrelated fix** (different files, different bugs, different ADRs, different concerns — not the natural sub-tasks of one feature), do not bundle them into a single commit. Instead, for each fix in turn:

1. implement only that one fix
2. add or update only the tests directly related to it
3. run the impacted tests; verify they pass
4. create one commit scoped to that fix (with a commit message describing only it)
5. push, then move to the next fix

Each fix becomes one commit. Each commit is independently reviewable, revertable, and bisectable. A multi-fix prompt produces N commits, not one.

Related sub-tasks of the same fix (e.g., a code change plus its test plus a docstring update plus a doc cross-reference) belong in the same commit — they are not "different fixes". The discriminator is whether the changes share a single root cause, ADR, or feature.

Do not bundle "while I'm here" cleanups into a fix commit. If unrelated drift is discovered mid-fix, either (a) note it explicitly and defer it, or (b) handle it as its own follow-up commit after the in-scope fix is committed.

### Multi-subproject prompts
When a prompt's work spans more than one CMake subproject (e.g., `qoasis/` + `main/`, or `quazip/` + `qoasis/`), do not bundle it into a single commit — even when it's one coherent feature. For each subproject in turn:

1. implement only that subproject's portion
2. add or update only the tests directly related to it
3. build that subproject (and its reverse dependencies) and verify it links and runs
4. create one commit scoped to that subproject
5. push, then move to the next subproject

Shared edits (a CMake option, a `fileinfo.rc.in` change, a `VERSION_*` bump in `main/CMakeLists.txt`, a `LICENSE`/`LICENSE.spdx` touch) that enable only **one** subproject's commit may ride with that commit. Shared edits that enable **more than one** subproject's commit go into their own preceding commit. Order from lowest-level to highest: vendored deps (`zlib/`, `quazip/`) before `qoasis/` before `main/`.

### Debugging hygiene

When chasing a bug across multiple commits, **do not squash the chain into a single "fix X" commit**. Each independent root cause peeled back during the investigation deserves its own commit, even when the surface symptom is the same. Squashing distinct fixes into one commit loses bisectability, makes reverts blast-radius bigger than they should be, and hides the diagnostic narrative future-you will want when the same symptom resurfaces.

What MUST be cleaned up before commit:

- **Diagnostic instrumentation added while chasing the bug.** Examples: `qDebug()` / `qInfo(logInfo())` traces sprinkled in hot paths (per-row inside `MainWindow::addDetailsToUI`, per-cell inside `qoasis::table::TableCell`, etc.), `std::cout` dumps of XML payloads, transient `if (doDebug) { … }` shims piggybacking on the `--debug` global from `main.cpp`, constructor logs enumerating `paymentMethodType` / `naturaType` / `bankAccount` registrations. These served their purpose finding the bug; leaving them in pollutes `Procurans.log` and noisy hot-loop logs slow large invoice parsing.
- **Throw-away one-shot fixtures.** Hardcoded paths to a specific `.xml` invoice or `.ods` file on the developer's machine, sample XML pasted from a real client invoice, `if (debug) return early` shortcuts inside `execute*` methods.
- **Commented-out code** from earlier hypotheses (e.g., the `//bool validateBill(...)` style stubs left behind in `mainwindow.h`).

What is NOT diagnostic noise (keep it):

- A **warning log on a real fallback path** the production code can take (e.g., `qWarning() << "invoice lacks <DatiPagamento>; rendering empty payments grid"`, or `qWarning() << "ODS exists but content.xml unreadable; recreating from template"`). That's a permanent operational signal.
- A **catch-block log of swallowed exceptions** that previously surfaced silently — e.g., a `QuaZip` open failure or a `QXmlStreamReader::hasError()` branch that used to return silently. Silent swallowing is a bug magnet; the structured (non-sensitive) log is the fix.
- A **structured info log on a one-shot startup or import path** (`qInfo(logInfo()) << "invoice parsed: file=… rows=… total=…"`). Fires once per imported invoice, not per row, and contains no sensitive payload content beyond what's already in the user-visible grid.

Mechanically: either fold the cleanup into the same commit as the fix, OR add it as a follow-up commit before pushing the chain. Do not push diagnostic noise "to clean up later" — later rarely comes.
