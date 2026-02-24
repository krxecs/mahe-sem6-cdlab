add_library(cdl07-lexer STATIC lexer.c lexer.h)

add_executable(cdl07-parser parser.c)
target_link_libraries(cdl07-parser PRIVATE cdl07-lexer)
