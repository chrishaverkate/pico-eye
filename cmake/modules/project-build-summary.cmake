message("**************************************************")
message("             ${PROJECT_NAME} Summary")
message("**************************************************")

message("Project Target:    ${PRJ_TARGET}")
message("Toolchain:         ${CMAKE_TOOLCHAIN_FILE}")
message("Build Type:        ${CMAKE_BUILD_TYPE}")
message("")

message("Flags")
message("\tC:    ${CMAKE_C_FLAGS}")
message("\tCXX:  ${CMAKE_CXX_FLAGS}")
message("")

message("Applications")
message("\tpico-eye:  ${PRJ_APPS_BUILD_APP}")
message("")

message("Options")
message("\tTests:       ${PRJ_TESTS_BUILD}")

message("**************************************************\n")
