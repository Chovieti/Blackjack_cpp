all: install

install: uninstall
	@mkdir -p build
	@cd build && cmake ..
	@$(MAKE) -C build

uninstall:
	@rm -rf build