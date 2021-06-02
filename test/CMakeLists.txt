add_executable(unit_tests gtest_main.cpp
        DataAccessTest.cpp)
target_include_directories(unit_tests
        PUBLIC
        test)
target_link_libraries(unit_tests
        PUBLIC
        fleetmanager
        CONAN_PKG::gtest)

add_test(test_all unit_tests --gtest_output=xml:gtest_report.xml)
