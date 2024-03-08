CXX= g++
CXXFLAGS=-g -Wall -std=c++11
CXXLINK=$(CXX)
EXEOBJS= main.cpp libinput.so libfirewall.so
SOOBJS= string.o ip.o port.o
SO= libfirewall.so
EXE= firewall.exe
RM= rm -rf libfirewall.so *.o *.exe

$(EXE): $(EXEOBJS)
	$(CXXLINK) $(CXXFLAGS) $(EXEOBJS) -o $(EXE) -linput -lfirewall -L. -Wl,-rpath,.

$(SO): $(SOOBJS)
	$(CXX) $(CXXFLAGS) -shared $(SOOBJS) -o $(SO)

ip.o: ip.cpp ip.h generic-field.h	
	$(CXX) $(CXXFLAGS) -c -fpic ip.cpp

port.o: port.cpp port.h generic-field.h
	$(CXX) $(CXXFLAGS) -c -fpic port.cpp

string.o: string.cpp string.h
	$(CXX) $(CXXFLAGS) -c -fpic string.cpp

clean:
	$(RM)