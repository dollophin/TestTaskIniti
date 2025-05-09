# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindJava
--------

Find Java

This module finds if Java is installed and determines where the
include files and libraries are.  The caller may set variable ``JAVA_HOME``
to specify a Java installation prefix explicitly.

See also the :module:`FindJNI` module to find Java Native Interface (JNI).

.. versionadded:: 3.10
  Added support for Java 9+ version parsing.

Specify one or more of the following components as you call this find module. See example below.

::

  Runtime     = Java Runtime Environment used to execute Java byte-compiled applications
  Development = Development tools (java, javac, javah, jar and javadoc), includes Runtime component
  IdlJ        = Interface Description Language (IDL) to Java compiler
  JarSigner   = Signer and verifier tool for Java Archive (JAR) files


This module sets the following result variables:

::

  Java_JAVA_EXECUTABLE      = the full path to the Java runtime
  Java_JAVAC_EXECUTABLE     = the full path to the Java compiler
  Java_JAVAH_EXECUTABLE     = the full path to the Java header generator
  Java_JAVADOC_EXECUTABLE   = the full path to the Java documentation generator
  Java_IDLJ_EXECUTABLE      = the full path to the Java idl compiler
  Java_JAR_EXECUTABLE       = the full path to the Java archiver
  Java_JARSIGNER_EXECUTABLE = the full path to the Java jar signer
  Java_VERSION_STRING       = Version of java found, eg. 1.6.0_12
  Java_VERSION_MAJOR        = The major version of the package found.
  Java_VERSION_MINOR        = The minor version of the package found.
  Java_VERSION_PATCH        = The patch version of the package found.
  Java_VERSION_TWEAK        = The tweak version of the package found (after '_')
  Java_VERSION              = This is set to: $major[.$minor[.$patch[.$tweak]]]

.. versionadded:: 3.4
  Added the ``Java_IDLJ_EXECUTABLE`` and ``Java_JARSIGNER_EXECUTABLE``
  variables.

The minimum required version of Java can be specified using the
:command:`find_package` syntax, e.g.

.. code-block:: cmake

  find_package(Java 1.8)

NOTE: ``${Java_VERSION}`` and ``${Java_VERSION_STRING}`` are not guaranteed to
be identical.  For example some java version may return:
``Java_VERSION_STRING = 1.8.0_17`` and ``Java_VERSION = 1.8.0.17``

another example is the Java OEM, with: ``Java_VERSION_STRING = 1.8.0-oem``
and ``Java_VERSION = 1.8.0``

For these components the following variables are set:

::

  Java_FOUND                    - TRUE if all components are found.
  Java_<component>_FOUND        - TRUE if <component> is found.



Example Usages:

.. code-block:: cmake

  find_package(Java)
  find_package(Java 1.8 REQUIRED)
  find_package(Java COMPONENTS Runtime)
  find_package(Java COMPONENTS Development)
#]=======================================================================]

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFindJavaCommon.cmake)

# The HINTS option should only be used for values computed from the system.
set(_JAVA_HINTS)
if(_JAVA_HOME)
  list(APPEND _JAVA_HINTS ${_JAVA_HOME}/bin)
endif()
if (WIN32)
  macro (_JAVA_GET_INSTALLED_VERSIONS _KIND)
    cmake_host_system_information(RESULT _JAVA_VERSIONS
    QUERY WINDOWS_REGISTRY "HKLM/SOFTWARE/JavaSoft/${_KIND}"
    SUBKEYS)
    if (_JAVA_VERSIONS)
      string (REGEX MATCHALL "[0-9._]+" _JAVA_VERSIONS "${_JAVA_VERSIONS}")
      string (REGEX REPLACE "([0-9._]+)" "\\1" _JAVA_VERSIONS "${_JAVA_VERSIONS}")
      if (_JAVA_VERSIONS)
        # sort versions. Most recent first
        list (SORT _JAVA_VERSIONS COMPARE NATURAL ORDER DESCENDING)
        foreach (_JAVA_VERSION IN LISTS _JAVA_VERSIONS)
          string(REPLACE "_" "." _JAVA_CMAKE_VERSION "${_JAVA_VERSION}")
          if (Java_FIND_VERSION_EXACT
              AND NOT _JAVA_CMAKE_VERSION MATCHES "^${Java_FIND_VERSION}")
            continue()
          endif()
          list(APPEND _JAVA_HINTS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\JavaSoft\\${_KIND}\\${_JAVA_VERSION};JavaHome]/bin")
        endforeach()
      endif()
    endif()
  endmacro()

  # for version 9 and upper
  _JAVA_GET_INSTALLED_VERSIONS("JDK")
  _JAVA_GET_INSTALLED_VERSIONS("JRE")

  # for versions older than 9
  _JAVA_GET_INSTALLED_VERSIONS("Java Development Kit")
  _JAVA_GET_INSTALLED_VERSIONS("Java Runtime Environment")
endif()

# Hard-coded guesses should still go in PATHS. This ensures that the user
# environment can always override hard guesses.
set(_JAVA_PATHS
  /usr/lib/java/bin
  /usr/share/java/bin
  /usr/local/java/bin
  /usr/local/java/share/bin
  /usr/java/j2sdk1.4.2_04
  /usr/lib/j2sdk1.4-sun/bin
  /usr/java/j2sdk1.4.2_09/bin
  /usr/lib/j2sdk1.5-sun/bin
  /opt/sun-jdk-1.5.0.04/bin
  /usr/local/jdk-1.7.0/bin
  /usr/local/jdk-1.6.0/bin
  )
find_program(Java_JAVA_EXECUTABLE
  NAMES java
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

if(Java_JAVA_EXECUTABLE)
    execute_process(COMMAND "${Java_JAVA_EXECUTABLE}" -version
      RESULT_VARIABLE _java_res
      OUTPUT_VARIABLE _java_var
      ERROR_VARIABLE _java_var # sun-java output to stderr
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE)
    if(_java_res)
      if(NOT Java_FIND_QUIETLY)
        message(STATUS "Java version check failed: "
          "${Java_JAVA_EXECUTABLE} -version returned an error: \"${_java_var}\"")
      endif()
      if(_java_var MATCHES "Unable to locate a Java Runtime|No Java runtime present, requesting install")
        # macOS distributes a java stub that provides an error message
        set(Java_JAVA_EXECUTABLE "Java_JAVA_EXECUTABLE-NOTFOUND" CACHE PATH
            "Path to the Java executable" FORCE)
      endif()
    else()
      # Extract version components (up to 4 levels) from "java -version" output.
      set(_java_version_regex [[(([0-9]+)(\.([0-9]+)(\.([0-9]+)(_([0-9]+))?)?)?.*)]])
      if(_java_var MATCHES "java version \"${_java_version_regex}\"")
        # Sun, GCJ, older OpenJDK
        set(Java_VERSION_STRING "${CMAKE_MATCH_1}")
        set(Java_VERSION_MAJOR "${CMAKE_MATCH_2}")
        if (CMAKE_MATCH_4)
        set(Java_VERSION_MINOR "${CMAKE_MATCH_4}")
        else()
          set(Java_VERSION_MINOR 0)
        endif()
        if (CMAKE_MATCH_6)
        set(Java_VERSION_PATCH "${CMAKE_MATCH_6}")
        else()
          set(Java_VERSION_PATCH 0)
        endif()
        set(Java_VERSION_TWEAK "${CMAKE_MATCH_8}")
      elseif(_java_var MATCHES "openjdk version \"${_java_version_regex}\"")
        # OpenJDK
        set(Java_VERSION_STRING "${CMAKE_MATCH_1}")
        set(Java_VERSION_MAJOR "${CMAKE_MATCH_2}")
        if (CMAKE_MATCH_4)
        set(Java_VERSION_MINOR "${CMAKE_MATCH_4}")
        else()
          set(Java_VERSION_MINOR 0)
        endif()
        if (CMAKE_MATCH_6)
        set(Java_VERSION_PATCH "${CMAKE_MATCH_6}")
        else()
          set(Java_VERSION_PATCH 0)
        endif()
        set(Java_VERSION_TWEAK "${CMAKE_MATCH_8}")
      elseif(_java_var MATCHES "openjdk version \"([0-9]+)-[A-Za-z]+\"")
        # OpenJDK 9 early access builds or locally built
        set(Java_VERSION_STRING "1.${CMAKE_MATCH_1}.0")
        set(Java_VERSION_MAJOR "1")
        set(Java_VERSION_MINOR "${CMAKE_MATCH_1}")
        set(Java_VERSION_PATCH "0")
        set(Java_VERSION_TWEAK "")
      elseif(_java_var MATCHES "java full version \"kaffe-${_java_version_regex}\"")
        # Kaffe style
        set(Java_VERSION_STRING "${CMAKE_MATCH_1}")
        set(Java_VERSION_MAJOR "${CMAKE_MATCH_2}")
        set(Java_VERSION_MINOR "${CMAKE_MATCH_4}")
        set(Java_VERSION_PATCH "${CMAKE_MATCH_6}")
        set(Java_VERSION_TWEAK "${CMAKE_MATCH_8}")
      else()
        if(NOT Java_FIND_QUIETLY)
          string(REPLACE "\n" "\n  " ver_msg "\n${_java_var}")
          message(WARNING "Java version not recognized:${ver_msg}\nPlease report.")
        endif()
        set(Java_VERSION_STRING "")
        set(Java_VERSION_MAJOR "")
        set(Java_VERSION_MINOR "")
        set(Java_VERSION_PATCH "")
        set(Java_VERSION_TWEAK "")
      endif()
      unset(_java_version_regex)
      unset(_java_var)
      set(Java_VERSION "${Java_VERSION_MAJOR}")
      if(NOT "x${Java_VERSION}" STREQUAL "x")
        foreach(_java_c IN ITEMS "MINOR" "PATCH" "TWEAK")
          if(NOT "x${Java_VERSION_${_java_c}}" STREQUAL "x")
            string(APPEND Java_VERSION ".${Java_VERSION_${_java_c}}")
          else()
            break()
          endif()
        endforeach()
        unset(_java_c)
      endif()
    endif()
    unset(_java_res)
endif()


find_program(Java_JAR_EXECUTABLE
  NAMES jar
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

find_program(Java_JAVAC_EXECUTABLE
  NAMES javac
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

find_program(Java_JAVAH_EXECUTABLE
  NAMES javah
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

find_program(Java_JAVADOC_EXECUTABLE
  NAMES javadoc
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

find_program(Java_IDLJ_EXECUTABLE
  NAMES idlj
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

find_program(Java_JARSIGNER_EXECUTABLE
  NAMES jarsigner
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

include(FindPackageHandleStandardArgs)
if(Java_FIND_COMPONENTS)
  set(_JAVA_REQUIRED_VARS)
  foreach(component IN LISTS Java_FIND_COMPONENTS)
    # User just want to execute some Java byte-compiled
    If(component STREQUAL "Runtime")
      list(APPEND _JAVA_REQUIRED_VARS Java_JAVA_EXECUTABLE)
      if(Java_JAVA_EXECUTABLE)
        set(Java_Runtime_FOUND TRUE)
      endif()
    elseif(component STREQUAL "Development")
      list(APPEND _JAVA_REQUIRED_VARS Java_JAVA_EXECUTABLE Java_JAVAC_EXECUTABLE
                                      Java_JAR_EXECUTABLE Java_JAVADOC_EXECUTABLE)
      if(Java_VERSION VERSION_LESS "10")
        list(APPEND _JAVA_REQUIRED_VARS Java_JAVAH_EXECUTABLE)
        if(Java_JAVA_EXECUTABLE AND Java_JAVAC_EXECUTABLE
            AND Java_JAVAH_EXECUTABLE AND Java_JAR_EXECUTABLE AND Java_JAVADOC_EXECUTABLE)
          set(Java_Development_FOUND TRUE)
        endif()
      else()
        if(Java_JAVA_EXECUTABLE AND Java_JAVAC_EXECUTABLE
            AND Java_JAR_EXECUTABLE AND Java_JAVADOC_EXECUTABLE)
          set(Java_Development_FOUND TRUE)
        endif()
      endif()
    elseif(component STREQUAL "IdlJ")
      list(APPEND _JAVA_REQUIRED_VARS Java_IDLJ_EXECUTABLE)
      if(Java_IDLJ_EXECUTABLE)
        set(Java_IdlJ_FOUND TRUE)
      endif()
    elseif(component STREQUAL "JarSigner")
      list(APPEND _JAVA_REQUIRED_VARS Java_JARSIGNER_EXECUTABLE)
      if(Java_JARSIGNER_EXECUTABLE)
        set(Java_JarSigner_FOUND TRUE)
      endif()
    else()
      message(FATAL_ERROR "Comp: ${component} is not handled")
    endif()
  endforeach()
  list (REMOVE_DUPLICATES _JAVA_REQUIRED_VARS)
  find_package_handle_standard_args(Java
    REQUIRED_VARS ${_JAVA_REQUIRED_VARS} HANDLE_COMPONENTS
    VERSION_VAR Java_VERSION
    )
  if(Java_FOUND)
    foreach(component IN LISTS Java_FIND_COMPONENTS)
      set(Java_${component}_FOUND TRUE)
    endforeach()
  endif()
  unset(_JAVA_REQUIRED_VARS)
else()
  # Check for Development
  if(Java_VERSION VERSION_LESS "10")
    find_package_handle_standard_args(Java
      REQUIRED_VARS Java_JAVA_EXECUTABLE Java_JAR_EXECUTABLE Java_JAVAC_EXECUTABLE
                    Java_JAVAH_EXECUTABLE Java_JAVADOC_EXECUTABLE
      VERSION_VAR Java_VERSION
      )
  else()
    find_package_handle_standard_args(Java
      REQUIRED_VARS Java_JAVA_EXECUTABLE Java_JAR_EXECUTABLE Java_JAVAC_EXECUTABLE
                    Java_JAVADOC_EXECUTABLE
      VERSION_VAR Java_VERSION
      )
  endif()
endif()


mark_as_advanced(
  Java_JAVA_EXECUTABLE
  Java_JAR_EXECUTABLE
  Java_JAVAC_EXECUTABLE
  Java_JAVAH_EXECUTABLE
  Java_JAVADOC_EXECUTABLE
  Java_IDLJ_EXECUTABLE
  Java_JARSIGNER_EXECUTABLE
)

# LEGACY
set(JAVA_RUNTIME ${Java_JAVA_EXECUTABLE})
set(JAVA_ARCHIVE ${Java_JAR_EXECUTABLE})
set(JAVA_COMPILE ${Java_JAVAC_EXECUTABLE})
