SRC = src/main.cpp
TARGET = chatd
SETLIB = settings
SHALIB = sha256d
CHLIB = MsgBased
UBLIB = UsrBased
SQLIB = sql_db
NLIB = netd
BINPATH = /usr/local/bin
LIBPATH = /usr/lib/chatd
DATAPATH = /var/lib/chatd

chatd:	$(SRC) libnet libsql libch libub libsha libcf
		g++ -o $(TARGET) $(SRC) -I/usr/include/mysql -L. -Wl,-rpath=$(LIBPATH) -l$(NLIB) -l$(SQLIB) -l$(CHLIB) -l$(UBLIB) -l$(SHALIB) -l$(SETLIB) /usr/lib/x86_64-linux-gnu/libmysqlclient.so

libnet:	src/$(NLIB).cpp src/$(NLIB).h
		g++ -o lib$(NLIB).so src/$(NLIB).cpp -shared -fPIC

libsql:	src/$(SQLIB).cpp src/$(SQLIB).h
		g++ -o lib$(SQLIB).so src/$(SQLIB).cpp -shared -fPIC

libch:  src/$(CHLIB).cpp src/$(CHLIB).h
		g++ -o lib$(CHLIB).so src/$(CHLIB).cpp -shared -fPIC

libub:	src/$(UBLIB).cpp src/$(UBLIB).h src/TmplBased.h
		g++ -o lib$(UBLIB).so src/$(UBLIB).cpp -shared -fPIC

libsha: src/$(SHALIB).cpp src/$(SHALIB).h
		g++ -o lib$(SHALIB).so src/$(SHALIB).cpp -shared -fPIC

libcf:	src/$(SETLIB).cpp src/$(SETLIB).h
		g++ -o lib$(SETLIB).so src/$(SETLIB).cpp -shared -fPIC

clean:
		rm -rf $(TARGET)
		rm -rf lib$(CHLIB).so
		rm -rf lib$(UBLIB).so
		rm -rf lib$(SHALIB).so
		rm -rf lib$(NLIB).so
		rm -rf lib$(SETLIB).so
		rm -rf lib$(SQLIB).so

install:
		install $(TARGET) $(BINPATH)
		mkdir $(LIBPATH)
		install lib$(CHLIB).so $(LIBPATH)
		install lib$(UBLIB).so $(LIBPATH)
		install lib$(SHALIB).so $(LIBPATH)
		install lib$(NLIB).so $(LIBPATH)
		install lib$(SQLIB).so $(LIBPATH)
		install lib$(SETLIB).so $(LIBPATH)
		mkdir $(DATAPATH) && chmod 777 $(DATAPATH)

uninstall:
		rm -rf $(BINPATH)/$(TARGET)
		rm -rf $(LIBPATH)/lib$(CHLIB).so
		rm -rf $(LIBPATH)/lib$(UBLIB).so
		rm -rf $(LIBPATH)/lib$(SHALIB).so
		rm -rf $(LIBPATH)/lib$(NLIB).so
		rm -rf $(LIBPATH)/lib$(SQLIB).so
		rm -rf $(LIBPATH)/lib$(SETLIB).so
		rm -rf $(LIBPATH)
		rm -rf $(DATAPATH)
