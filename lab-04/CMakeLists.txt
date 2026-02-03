add_library(cdl04-lexer STATIC lexer.c lexer.h)

add_executable(cdl04-symbol-table symbol-table.c)
target_link_libraries(cdl04-symbol-table
  PRIVATE cdl04-lexer uthash
)
