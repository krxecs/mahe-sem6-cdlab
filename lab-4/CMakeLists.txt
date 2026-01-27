add_library(cdl4-lexer STATIC lexer.c lexer.h)

add_executable(cdl4-symbol-table symbol-table.c)
target_link_libraries(cdl4-symbol-table
  PRIVATE cdl4-lexer uthash
)
