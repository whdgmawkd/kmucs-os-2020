CXX := g++
ifeq ($(DEBUG),y)
	CXXFLAGS := -std=c++14 -g -DDEBUG
else
	CXXFLAGS := -std=c++14 -g
endif
LDFLAGS := $(CXXFLAGS) -MMD -MP -c
LIBS := -lpthread

Q := @
ECHO := $(Q) echo

OUT := build
SRC := src

TARGET := $(OUT)/a.out
SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(subst $(SRC),$(OUT),$(SOURCES:.cpp=.o))
DEPENDS := $(OBJECTS:.o=.d)

.PHONY: all clean run



all: $(OUT) $(TARGET)

$(OUT):
	$(ECHO) "'$(OUT)' folder is not exists. creating folder."
	$(Q) mkdir -p $(OUT)

$(TARGET): $(OBJECTS)
	$(ECHO) '[LD] $^ => $@'
	$(Q) $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(OUT)/%.o: $(SRC)/%.cpp Makefile
	$(ECHO) '[CXX] $< => $@'
	$(Q) $(CXX) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	$(ECHO) '[DEL] $(TARGET)'
	$(Q)rm -f $(TARGET) 
	$(ECHO) '[DEL] $(OBJECTS)'
	$(Q)rm -f $(OBJECTS)
	$(ECHO) '[DEL] $(DEPENDS)'
	$(Q)rm -f $(DEPENDS)

run:
	$(Q) $(TARGET)

-include $(DEPENDS)