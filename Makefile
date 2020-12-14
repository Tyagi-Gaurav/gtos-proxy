GCC  := gcc
SRC := src
LIB := lib
OBJ_LIB := obj_lib
OBJ := obj
TST := test

HEADERS := '-I./headers'
OPENSSL := '-I/usr/local/opt/openssl/include'
CFLAGS :=-g -c -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L $(OPENSSL) -DDEBUG
SOURCES := $(wildcard $(SRC)/*.c)
SOURCES_LIB := $(wildcard $(LIB)/*.c)
TST_SOURCES := $(wildcard $(TST)/*.c)
OBJECTS :=  $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
LIB_OBJ := $(patsubst $(LIB)/%.c, $(OBJ_LIB)/%.o, $(SOURCES_LIB))
TEST_OBJECTS := $(patsubst $(TST)/%.c, $(TST)/%.o, $(TST_SOURCES))
LD_FLAGS := -lm -pthread

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	TST_LIBS += -DNORT
else
	TST_LIBS += -lrt
endif

define generate-exe
$(1)

endef

setup:
	mkdir -p obj obj_lib

all : clean setup debug all_targets

custom: setup custom_targets

debug: CFLAGS += -g 
debug: $(OBJECTS) $(TEST_OBJECTS)

all_targets: $(OBJECTS) $(LIB_OBJ)
	$(foreach ob, $(OBJECTS),$(call generate-exe, $(GCC) $(ob) $(LIB_OBJ) $(LD_FLAGS) -o $(notdir $(basename $(ob))).a))

$(OBJ)/%.o: $(SRC)/%.c
	$(GCC) $(CFLAGS) $(HEADERS) $< -o $@

$(OBJ_LIB)/%.o: $(LIB)/%.c
	$(GCC) $(CFLAGS) $(HEADERS) $< -o $@

.PHONY: setup clean all test

clean:
	rm -rf $(OBJ) $(OBJ_LIB)
	rm *.a

echo:
	echo $(filter-out src/sem_signal.c, $(wildcard $(SRC)/*.c))
