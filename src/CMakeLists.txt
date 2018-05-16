add_library(chimp_jni SHARED
   ../include/chimp_jni/chimp_connector.hpp
   chimp_connector.cpp
   ../include/chimp_jni/chimp_fluent.hpp
   ../include/chimp_jni/plan.hpp
   ../include/chimp_jni/jni_helper.hpp
   jni_helper.cpp
)

target_link_libraries(chimp_jni ${JNI_LIBRARIES})

install(
    TARGETS chimp_jni
    LIBRARY DESTINATION lib/chimp_jni
)

# headers
install(
    DIRECTORY ../include/chimp_jni
    DESTINATION include
)

# pkg-config-file
configure_file("chimp_jni.pc.in" "chimp_jni.pc" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/chimp_jni.pc"
    DESTINATION lib/pkgconfig
)