message("PRJ: Configuring options for Pico target")

set(PRJ_APPS_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/" CACHE FILEPATH "Output directory to place binary files when built.")

option(PRJ_APPS_BUILD_APP "Build Pico specific applications" OF)
option(PRJ_TESTS_BUILD "Tests are not built for pico target" ON)

