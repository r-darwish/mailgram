file(GLOB source_files *.cpp)
add_executable(mailgram ${source_files})

target_link_libraries(mailgram CONAN_PKG::boost_asio)
