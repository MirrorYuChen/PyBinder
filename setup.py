import os
import re
import sys
import shutil
import subprocess
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from pathlib import Path

__version__ = "0.1.0"
root = Path(__file__).parent
with open(str(root / 'ReadMe.md'), 'r', encoding='utf-8') as fd:
  readme = fd.read()

PLAT_TO_CMAKE = {
  "win32": "Win32",
  "win-amd64": "x64",
  "win-arm32": "ARM",
  "win-arm64": "ARM64",
}


'''
  Set CMake source directory
'''
class CMakeExtension(Extension):
  def __init__(self, name: str, srcdir: str="") -> None:
    super().__init__(name, sources=[])
    self.srcdir = os.fspath(Path(srcdir).resolve())
    

'''
  CMake build Wrapper
'''
class CMakeBuild(build_ext):
  def build_extension(self, ext: CMakeExtension) -> None:
    self.parallel = 16
    # Must be in this form due to bug in .resolve() only fixed in Python 3.10+
    ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
    extdir = ext_fullpath.parent.resolve()

    # Using this requires trailing slash for auto-detection & inclusion of
    # auxiliary "native" libs
    debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
    cfg = "Debug" if debug else "Release"

    # CMake lets you override the generator - we need to check this.
    # Can be set with Conda-Build, for example.
    cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

    # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
    # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
    # from Python.
    cmake_args = [
      f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}{os.sep}",
      f"-DPYTHON_EXECUTABLE={sys.executable}",
      f"-DCMAKE_BUILD_TYPE={cfg}",  # not used on MSVC, but no harm
      f"-DBUILD_PYTHON=ON",
      f"-DUSE_PANGOLIN_VIEWER=ON"
    ]
    build_args = []
    # Adding CMake arguments set as environment variable
    # (needed e.g. to build for ARM OSx on conda-forge)
    if "CMAKE_ARGS" in os.environ:
      cmake_args += [item for item in os.environ["CMAKE_ARGS"].split(" ") if item]

    if self.compiler.compiler_type != "msvc":
      # Using Ninja-build since it a) is available as a wheel and b)
      # multithreads automatically. MSVC would require all variables be
      # exported for Ninja to pick it up, which is a little tricky to do.
      # Users can override the generator with CMAKE_GENERATOR in CMake
      # 3.15+.
      if not cmake_generator or cmake_generator == "Ninja":
        try:
          import ninja

          ninja_executable_path = Path(ninja.BIN_DIR) / "ninja"
          cmake_args += [
            "-GNinja",
            f"-DCMAKE_MAKE_PROGRAM:FILEPATH={ninja_executable_path}",
          ]
        except ImportError:
          pass

    else:
      # Single config generators are handled "normally"
      single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

      # CMake allows an arch-in-generator style for backward compatibility
      contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

      # Specify the arch if using MSVC generator, but only if it doesn't
      # contain a backward-compatibility arch spec already in the
      # generator name.
      if not single_config and not contains_arch:
        cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

      # Multi-config generators have a different way to specify configs
      if not single_config:
        cmake_args += [
          f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}"
        ]
        build_args += ["--config", cfg]

    if sys.platform.startswith("darwin"):
      # Cross-compile support for macOS - respect ARCHFLAGS if set
      archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
      if archs:
        cmake_args += ["-DCMAKE_OSX_ARCHITECTURES={}".format(";".join(archs))]

    # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
    # across all generators.
    if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
      # self.parallel is a Python 3 only way to set parallel jobs by hand
      # using -j in the build_ext call, not supported by pip or PyPA-build.
      if hasattr(self, "parallel") and self.parallel:
        # CMake 3.12+ only.
        build_args += [f"-j{self.parallel}"]

    build_temp = Path(self.build_temp) / ext.name
    if not build_temp.exists():
      build_temp.mkdir(parents=True)

    subprocess.run(
      ["cmake", ext.srcdir, *cmake_args], cwd=build_temp, check=True
    )
    subprocess.run(
      ["cmake", "--build", ".", *build_args], cwd=build_temp, check=True
    )

setup(
  name="vslam",
  version="0.6.0",
  author="chenjingyu",
  author_email="2458006366@qq.com",
  description="A test project using pybind11 and CMake",
  long_description=readme,
  long_description_content_type="text/markdown",
  ext_modules=[CMakeExtension("vslam")],
  cmdclass={"build_ext": CMakeBuild},
  zip_safe=False,
  extras_require={"test": ["pytest>=6.0"]},
  python_requires=">=3.6",
  packages=find_packages(),
  classifiers=[
    "Programming Language :: Python :: 3",
    "License :: OSI Approved :: Apache Software License",
    "Operating System :: OS Independent",
  ],
)