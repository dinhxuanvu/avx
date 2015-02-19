 #Copyright (C) 2014  Cagdas Caglak

 #This program is free software: you can redistribute it and/or modify
 #it under the terms of the GNU General Public License as published by
 #the Free Software Foundation, either version 3 of the License, or
 #(at your option) any later version.

 #This program is distributed in the hope that it will be useful,
 #but WITHOUT ANY WARRANTY; without even the implied warranty of
 #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #GNU General Public License for more details.

 #You should have received a copy of the GNU General Public License
 #along with this program.  If not, see <http://www.gnu.org/licenses/>.

 #Modified by: Vu Dinh
 #Date: 02/09/2015
 
ECHO=@echo
ECHO_N=@echo -n

EXEC_HMC5883L=TestHMC5883L

CXX=g++
CFLAGS=-Wall -c

all: $(EXEC_HMC5883L)
	@mkdir -p obj
	@mv *.o obj
		
$(EXEC_HMC5883L): TestHMC5883L.o BBB_I2C.o HMC5883L.o
	$(ECHO) "$(EXEC_HMC5883L) building..."
	@$(CXX) TestHMC5883L.o BBB_I2C.o HMC5883L.o -o $(EXEC_HMC5883L)
	
TestHMC5883L.o: TestHMC5883L.cpp
	@$(CXX) $(CFLAGS) TestHMC5883L.cpp

BBB_I2C.o: BBB_I2C.cpp
	@$(CXX) $(CFLAGS) BBB_I2C.cpp
	
HMC5883L.o: HMC5883L.cpp
	@$(CXX) $(CFLAGS) HMC5883L.cpp

clean:
	$(ECHO) "Clean."
	@rm -rf *.o $(EXEC_HMC5883L) obj