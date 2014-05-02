CC=g++
CFLAGS=-c -Wall -std=c++11 -O3 -I./include/ -DBS_BUILD_LINUX
LFLAGS=0
OBJS=obj/Exception.o obj/Logger.o obj/Lua.o obj/LuaInstance.o obj/Systems.o obj/Window.o obj/WindowEvents.o obj/util/Unicode.o obj/linux/LinuxWindow.o
IDIR=include/brimstone

all: brimstone

brimstone: lib/libbrimstone.a

lib/libbrimstone.a: $(OBJS)
	mkdir -p luajit/ && cd luajit && ar x /usr/local/lib/libluajit-5.1.a && cd ../
	ar -crfsv lib/libbrimstone.a $(OBJS) luajit/*.o
	rm -rf luajit/

obj/Exception.o: src/Exception.cpp $(IDIR)/Exception.hpp
	$(CC) $(CFLAGS) src/Exception.cpp -o $@

obj/Logger.o: src/Logger.cpp $(IDIR)/Logger.hpp $(IDIR)/Exception.hpp
	$(CC) $(CFLAGS) src/Logger.cpp -o $@

obj/Lua.o: src/Lua.cpp $(IDIR)/Lua.hpp
	$(CC) $(CFLAGS) src/Lua.cpp -o $@

obj/LuaInstance.o: src/LuaInstance.cpp $(IDIR)/LuaInstance.hpp $(IDIR)/Exception.hpp
	$(CC) $(CFLAGS) src/LuaInstance.cpp -o $@

obj/Systems.o: src/Systems.cpp $(IDIR)/Systems.hpp
	$(CC) $(CFLAGS) src/Systems.cpp -o $@

obj/Window.o: src/Window.cpp $(IDIR)/Window.hpp
	$(CC) $(CFLAGS) src/Window.cpp -o $@

obj/WindowEvents.o: src/WindowEvents.cpp
	$(CC) $(CFLAGS) src/WindowEvents.cpp -o $@

obj/util/Unicode.o: src/util/Unicode.cpp $(IDIR)/util/Unicode.hpp $(IDIR)/Exception.hpp
	$(CC) $(CFLAGS) src/util/Unicode.cpp -o $@

obj/linux/LinuxWindow.o: src/linux/LinuxWindow.cpp $(IDIR)/linux/LinuxWindow.hpp
	$(CC) $(CFLAGS) src/linux/LinuxWindow.cpp -o $@

include/brimstone/Exception.hpp: $(IDIR)/types.hpp

include/brimstone/Logger.hpp: $(IDIR)/types.hpp

include/brimstone/Lua.hpp: $(IDIR)/Systems.hpp $(IDIR)/LuaInstance.hpp

include/brimstone/Systems.hpp: $(IDIR)/factory/FactoryManager.hpp $(IDIR)/factory/BasicFactory.hpp $(IDIR)/util/Misc.hpp

include/brimstone/factory/FactoryManager.hpp: $(IDIR)/factory/IFactory.hpp $(IDIR)/Exception.hpp $(IDIR)/util/RefType.hpp

include/brimstone/factory/BasicFactory.hpp: $(IDIR)/factory/IFactory.hpp

include/brimstone/LuaInstance.hpp: $(IDIR)/types.hpp $(IDIR)/util/Sequence.hpp $(IDIR)/util/Misc.hpp

include/brimstone/Window.hpp: $(IDIR)/Rectangle.hpp $(IDIR)/signals/Signal.hpp $(IDIR)/types.hpp $(IDIR)/WindowEvents.hpp $(IDIR)/linux/LinuxWindow.hpp

include/brimstone/Rectangle.hpp: $(IDIR)/Exception.hpp

include/brimstone/signals/Signal.hpp: $(IDIR)/signals/Delegate.hpp $(IDIR)/signals/ScopedConnection.hpp

include/brimstone/signals/Delegate.hpp: $(IDIR)/util/Cast.hpp

include/brimstone/WindowEvents.hpp: $(IDIR)/types.hpp

include/brimstone/linux/LinuxWindow.hpp: $(IDIR)/Rectangle.hpp $(IDIR)/types.hpp

include/brimstone/util/Unicode.hpp: $(IDIR)/types.hpp

.PHONY: clean
clean:
	rm -f obj/*.o
	rm -f obj/linux/*.o
	rm -f obj/util/*.o
	rm -f lib/*.a
