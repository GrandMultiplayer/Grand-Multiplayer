all:
	make -C Vendor/RakNet
	make -C Server/Launcher
	make -C API.Plugins/Lua
	make -C API.Plugins/Freeroam

clean:
	make -C Vendor/RakNet clean
	make -C Server/Launcher clean
	make -C API.Plugins/Lua clean
	make -C API.Plugins/Freeroam clean