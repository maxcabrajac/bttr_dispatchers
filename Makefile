OUT=build/libbttr_dispatchers.so

build/build.ninja:
	make setup

.PHONY: setup compile load unload update

setup:
	meson setup build

compile: build/build.ninja
	meson compile -C build

load: $(OUT)
	hyprctl plugin load $(PWD)/$(OUT)

unload: $(OUT)
	hyprctl plugin unload $(PWD)/$(OUT)

update:
	make unload
	make compile
	make load

