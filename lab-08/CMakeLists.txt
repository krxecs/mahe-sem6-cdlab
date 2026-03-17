add_library(cdl08-lexer STATIC lexer.c lexer.h)

add_executable(cdl08-parser parser.c)
target_link_libraries(cdl08-parser PRIVATE cdl08-lexer)
