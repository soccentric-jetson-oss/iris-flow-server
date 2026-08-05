function(target_strict_warnings TARGET)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion)
endfunction()
