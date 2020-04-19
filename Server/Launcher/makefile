Server: Main.cpp
	g++ -I../../Shared -I../../Vendor -I../../Vendor/RakNet -L../../bin/Linux/ *.cpp -o Server.a -l:RakNet.a -l:GM.Master.a -pthread -std=c++11 -ldl -lm -lcurl -shared -fPIC
	g++ -rdynamic -I../../Shared -I../../Vendor -I../../Vendor/RakNet -L../../bin/Linux/ *.cpp -o ../../bin/Linux/Server.so -l:RakNet.a -l:GM.Master.a -Wl,-rpath,"./${ORIGIN}" -pthread -std=c++11 -ldl -lm -lcurl
