wasm-wasi: wasm-wasi.cpp
	clang++ --target=wasm32-wasi -O3 -flto -Wl,--lto-O3 --sysroot ~/wasm/wasi-libc/sysroot -nostartfiles \
	-Wl,-z,stack-size=2097152 -Wl,--no-entry \
	-Wl,--export=__heap_base -o wasm-wasi.wasm wasm-wasi.cpp
