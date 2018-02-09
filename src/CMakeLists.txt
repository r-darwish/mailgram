add_executable(mailgram main.cpp session.cpp)

target_link_libraries(mailgram CONAN_PKG::boost_asio)
