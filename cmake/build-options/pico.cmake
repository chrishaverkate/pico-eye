message("PRJ: Configuring options for Pico target")

set(PRJ_APPS_OUTPUT_DIR "/opt/src/bin/" CACHE FILEPATH "Output directory to place binary elf files when built.")

option(PRJ_APPS_BUILD_APP "Build Pico specific applications" ON)
option(PRJ_TESTS_BUILD "Tests are not built for pico target" OFF)


# Defining some variables for the Pico-SDK
set(PICO_PLATFORM rp2040)
set(PICO_COMPILER pico_arm_gcc)
