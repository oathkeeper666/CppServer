LIBS=-lpthread -lboost_system -lboost_filesystem -lboost_context -lprotobuf
CPPFLAG=-std=c++11 -g -Wall -O2
INCLUDE=-I include -I src
COMPILE=g++ -c
LINK=g++ -o

GATEWAY=./bin/gateway

all: $(GATEWAY) proto

FRAMEWORK_SOURCE=$(wildcard src/framework/*.cpp)
THIRD_PART_SOURCE=$(wildcard include/tiny/*.cpp)
GATEWAY_SOUCE=$(wildcard src/gateway/*.cpp)
PROTO_SOURCE=$(wildcard proto/*.cc)

FRAMEWORK_OBJ=$(patsubst %.cpp, %.o, $(FRAMEWORK_SOURCE))
THIRD_PART_OBJ=$(patsubst %.cpp, %.o, $(THIRD_PART_SOURCE))
GATEWAY_OBJ=$(patsubst %.cpp, %.o, $(GATEWAY_SOUCE))
PROTO_OBJ=$(patsubst %.cc, %.o, $(PROTO_SOURCE))

PROTO_FILE=$(wildcard ./proto/*.proto)
proto: $(PROTO_FILE)
	protoc --cpp_out=./proto/ -I=./proto/ $^

$(GATEWAY): $(FRAMEWORK_OBJ) $(THIRD_PART_OBJ) $(GATEWAY_OBJ) $(PROTO_OBJ)
	$(LINK) $@ $^ $(LIBS)

$(FRAMEWORK_OBJ): %.o: %.cpp
	$(COMPILE) $(CPPFLAG) -o $@ $<

$(THIRD_PART_OBJ): %.o: %.cpp
	$(COMPILE) $(CPPFLAG) -o $@ $<

$(GATEWAY_OBJ): %.o: %.cpp
	$(COMPILE) $(CPPFLAG) $(INCLUDE) -o $@ $^

$(PROTO_OBJ): %.o: %.cc
	$(COMPILE) $(CPPFLAG) -o $@ $^

.PHONY: clean proto

clean:
	rm -f $(FRAMEWORK_OBJ) $(THIRD_PART_OBJ) $(GATEWAY_OBJ) $(PROTO_OBJ)
