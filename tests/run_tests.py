#!/usr/bin/env python3

"""Build and execute EDP-Primitives host tests and expected compile failures."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[1]
SYSTEM_ROOT = Path(
    os.environ.get(
        "EDP_SYSTEM_ROOT",
        ROOT.parent / "EDP-System",
    )
).resolve()
SYSTEM_INCLUDE = SYSTEM_ROOT / "src"
SERIALISATION_ROOT = Path(
    os.environ.get(
        "EDP_SERIALISATION_ROOT",
        ROOT.parent / "EDP-Serialisation",
    )
).resolve()
SERIALISATION_INCLUDE = SERIALISATION_ROOT / "src"
BOUNDED_TYPES_ROOT = Path(
    os.environ.get(
        "EDP_BOUNDED_TYPES_ROOT",
        ROOT.parent / "EDP-BoundedTypes",
    )
).resolve()
BOUNDED_TYPES_INCLUDE = BOUNDED_TYPES_ROOT / "src"
MEMORY_ROOT = Path(os.environ.get("EDP_MEMORY_ROOT", ROOT.parent / "EDP-Memory")).resolve()
MEMORY_INCLUDE = MEMORY_ROOT / "src"
PLATFORM_ROOT = Path(os.environ.get("EDP_PLATFORM_ROOT", ROOT.parent / "EDP-Platform")).resolve()
PLATFORM_INCLUDE = PLATFORM_ROOT / "src"
BOUNDED_TOPOLOGY_ROOT = Path(os.environ.get("EDP_BOUNDED_TOPOLOGY_ROOT", ROOT.parent / "EDP-BoundedTopology")).resolve()
BOUNDED_TOPOLOGY_INCLUDE = BOUNDED_TOPOLOGY_ROOT / "src"
PORTABLE_ROOT = Path(os.environ.get("EDP_PLATFORM_PORTABLE_ROOT", ROOT.parent / "EDP-Platform-Portable")).resolve()
PORTABLE_INCLUDE = PORTABLE_ROOT / "src"
INCLUDE = ROOT / "src"
CXX = os.environ.get("CXX", "c++")

POSITIVE_TESTS = (
    ROOT / "tests" / "PrimitivesTests.cpp",
    ROOT / "tests" / "SharedProviderTests.cpp",
)

COMPILE_FAIL_TESTS = (
    ROOT / "tests" / "compile_fail" / "duplicate_primitive_type.cpp",
    ROOT / "tests" / "compile_fail" / "duplicate_type_identifier.cpp",
    ROOT / "tests" / "compile_fail" / "invalid_runtime_provider.cpp",
    ROOT / "tests" / "compile_fail" / "duplicate_resource.cpp",
    ROOT / "tests" / "compile_fail" / "invalid_family_identifier.cpp",
    ROOT / "tests" / "compile_fail" / "missing_family_planner.cpp",
    ROOT / "tests" / "compile_fail" / "missing_schema.cpp",
    ROOT / "tests" / "compile_fail" / "non_serialisable_field.cpp",
)

COMMON_ARGUMENTS = (
    "-std=c++20",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    f"-I{INCLUDE}",
    f"-I{SYSTEM_INCLUDE}",
    f"-I{SERIALISATION_INCLUDE}",
    f"-I{BOUNDED_TYPES_INCLUDE}",
    f"-I{MEMORY_INCLUDE}",
    f"-I{PLATFORM_INCLUDE}",
    f"-I{BOUNDED_TOPOLOGY_INCLUDE}",
    f"-I{PORTABLE_INCLUDE}",
)


def run(command: list[str], expect_success: bool) -> bool:
    """Execute one compiler/test command and validate the expected result."""
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    succeeded = result.returncode == 0

    if succeeded == expect_success:
        return True

    sys.stderr.write(result.stdout)
    sys.stderr.write(result.stderr)
    return False


def main() -> int:
    """Compile positive tests, execute them, and confirm negative tests fail compilation."""
    if not (SYSTEM_INCLUDE / "ESPressio_System.hpp").is_file():
        print(
            "ERROR: EDP-System source not found. "
            "Place EDP-System beside EDP-Primitives or set EDP_SYSTEM_ROOT.",
            file=sys.stderr,
        )
        return 2

    if not (SERIALISATION_INCLUDE / "serialisation" / "SerialisableType.hpp").is_file():
        print(
            "ERROR: EDP-Serialisation source not found. "
            "Place EDP-Serialisation beside EDP-Primitives or set EDP_SERIALISATION_ROOT.",
            file=sys.stderr,
        )
        return 2

    if not (BOUNDED_TYPES_INCLUDE / "ESPressio_BoundedTypes.hpp").is_file():
        print(
            "ERROR: EDP-BoundedTypes source not found. "
            "Place EDP-BoundedTypes beside EDP-Primitives or set EDP_BOUNDED_TYPES_ROOT.",
            file=sys.stderr,
        )
        return 2

    dependency_headers = (
        (MEMORY_INCLUDE / "ESPressio_Memory.hpp", "EDP-Memory"),
        (PLATFORM_INCLUDE / "ESPressio_Platform.hpp", "EDP-Platform"),
        (BOUNDED_TOPOLOGY_INCLUDE / "ESPressio_BoundedTopology.hpp", "EDP-BoundedTopology"),
        (PORTABLE_INCLUDE / "ESPressio_Platform_Portable.hpp", "EDP-Platform-Portable"),
    )
    for header, dependency in dependency_headers:
        if not header.is_file():
            print(f"ERROR: {dependency} source not found: {header}", file=sys.stderr)
            return 2

    with tempfile.TemporaryDirectory(prefix="edp-primitives-tests-") as temporary_directory:
        output_directory = Path(temporary_directory)

        for source in POSITIVE_TESTS:
            executable = output_directory / source.stem

            if not run(
                [
                    CXX,
                    *COMMON_ARGUMENTS,
                    str(source),
                    "-o",
                    str(executable),
                ],
                True,
            ):
                print(f"FAIL: positive test did not compile: {source.relative_to(ROOT)}")
                return 1

            if not run([str(executable)], True):
                print(f"FAIL: positive test execution failed: {source.relative_to(ROOT)}")
                return 1

            print(f"PASS: {source.relative_to(ROOT)}")

        for source in COMPILE_FAIL_TESTS:
            object_file = output_directory / f"{source.stem}.o"

            if not run(
                [
                    CXX,
                    *COMMON_ARGUMENTS,
                    "-c",
                    str(source),
                    "-o",
                    str(object_file),
                ],
                False,
            ):
                print(f"FAIL: compile-fail test unexpectedly compiled: {source.relative_to(ROOT)}")
                return 1

            print(f"PASS (expected compile failure): {source.relative_to(ROOT)}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
