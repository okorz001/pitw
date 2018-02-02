# CheckSymbolExists cannot find intrinsics
include(CheckCSourceCompiles)
include(CheckSymbolExists)

macro(check_code_compiles code var)
  check_c_source_compiles("int main(void){${code};return 0;}" ${var})
endmacro()

# Determine atomic support
check_symbol_exists(atomic_fetch_add stdatomic.h PITW_HAS_C11_ATOMIC)
check_code_compiles("int i = 0; __atomic_fetch_add(&i, 1, __ATOMIC_SEQ_CST);"
                    PITW_HAS_GCC_ATOMIC)
check_code_compiles("int i = 0; __sync_fetch_and_add(&i, 1);" PITW_HAS_GCC_SYNC)
check_symbol_exists(InterlockedIncrement windows.h PITW_HAS_WIN32_INTERLOCKED)
