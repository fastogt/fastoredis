{
 "description": @CPACK_PACKAGE_DESCRIPTION_SUMMARY@,
 "qt": "@_qt5Core_install_prefix@",
 "sdk": "@ANDROID_SDK@",
 "ndk": "@ANDROID_NDK@",
 "toolchain-prefix": "@ANDROID_TOOLCHAIN_MACHINE_NAME@",
 "tool-prefix": "@ANDROID_TOOLCHAIN_MACHINE_NAME@",
 "toolchain-version": "@ANDROID_COMPILER_VERSION@",
 "ndk-host": "@ANDROID_NDK_HOST_SYSTEM_NAME@",
 "target-architecture": "@ANDROID_TARGET_ARCH@",
 "application-binary": "@CMAKE_LIBRARY_OUTPUT_DIRECTORY@/@PROJECT_NAME_LOWERCASE@",
 "android-package-source-directory": "@PACKAGE_SOURCE_ANDROID@/",
 "android-package": "net.@PROJECT_NAME_LOWERCASE@"
}
