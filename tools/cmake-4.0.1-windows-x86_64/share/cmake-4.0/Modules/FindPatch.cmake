# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindPatch
---------

.. versionadded:: 3.10

The module defines the following variables:

``Patch_EXECUTABLE``
  Path to patch command-line executable.
``Patch_FOUND``
  True if the patch command-line executable was found.

The following :prop_tgt:`IMPORTED` targets are also defined:

``Patch::patch``
  The command-line executable.

  .. versionchanged:: 4.0
    Imported target is defined only when :prop_gbl:`CMAKE_ROLE` is ``PROJECT``.

Example usage:

.. code-block:: cmake

   find_package(Patch)
   if(Patch_FOUND)
     message("Patch found: ${Patch_EXECUTABLE}")
   endif()
#]=======================================================================]

set(_doc "Patch command line executable")
set(_patch_path )

if(CMAKE_HOST_WIN32)
  set(_patch_path
    "$ENV{LOCALAPPDATA}/Programs/Git/bin"
    "$ENV{LOCALAPPDATA}/Programs/Git/usr/bin"
    "$ENV{APPDATA}/Programs/Git/bin"
    "$ENV{APPDATA}/Programs/Git/usr/bin"
    )
endif()

# First search the PATH
find_program(Patch_EXECUTABLE
  NAMES patch
  PATHS ${_patch_path}
  DOC ${_doc}
  )

if(CMAKE_HOST_WIN32)
  # Now look for installations in Git/ directories under typical installation
  # prefixes on Windows.
  find_program(Patch_EXECUTABLE
    NAMES patch
    PATH_SUFFIXES Git/usr/bin Git/bin GnuWin32/bin
    DOC ${_doc}
    )
endif()

mark_as_advanced(Patch_EXECUTABLE)

get_property(_patch_role GLOBAL PROPERTY CMAKE_ROLE)

if(
  _patch_role STREQUAL "PROJECT"
  AND Patch_EXECUTABLE
  AND NOT TARGET Patch::patch
)
  add_executable(Patch::patch IMPORTED)
  set_property(TARGET Patch::patch PROPERTY IMPORTED_LOCATION ${Patch_EXECUTABLE})
endif()

unset(_patch_path)
unset(_patch_role)
unset(_doc)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Patch
                                  REQUIRED_VARS Patch_EXECUTABLE)
