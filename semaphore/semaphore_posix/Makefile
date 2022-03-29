CC:= gcc #compiler
CPICFLAGS:= -fPIC -ansi -pedantic-errors -Wall -Wextra #compiler flags with PIC
CFLAGS:= -ansi -pedantic-errors -Wall -Wextra #compiler flag for tests purpose
RELEASE= -O3 -DNDEBUG #compiler optimization flags
LDFLAGS:= -shared
CPPFLAGS := -I../../include

DEBUG:= -g -DDEBUG #debug flags

SRCS_TST := $(wildcard *_test.c) #all files have _test.c
SRCS_LIB := $(filter-out $(SRCS_TST), $(wildcard *.c)) #all c files exept of *_test.c
OBJS_LIB := $(SRCS_LIB:.c=.o) #object files without test
OBJS_TST := $(SRCS_TST:.c=.o) #test object files

DEPENDENCY:=semaphore_posix

VLG:= valgrind --leak-check=yes --track-origins=yes #valgrainde

all: release debug
 
###########################release###########################
PATH_OBJ_RELEASE = ../../obj/release
PATH_RELEASE = ../../libs/release	
LIB_RELEASE = ../../libs/release/lib$(DEPENDENCY).so #name file and location

release: $(LIB_RELEASE)

$(LIB_RELEASE): $(PATH_OBJ_RELEASE)/$(DEPENDENCY).o
	$(CC) $(LDFLAGS) -o $@ $^ 

$(PATH_OBJ_RELEASE)/$(DEPENDENCY).o: $(SRCS_LIB)
	$(CC) -c $(CPICFLAGS) $(RELEASE) $< -o $@
###########################debug###########################
PATH_OBJ_DEBUG = ../../obj/debug
PATH_DEBUG = ../../libs/debug
LIB_DEBUG = ../../libs/debug/lib$(DEPENDENCY).so

debug: $(LIB_DEBUG)

$(LIB_DEBUG): $(PATH_OBJ_DEBUG)/$(DEPENDENCY).o
	$(CC) $(LDFLAGS) -o $@ $^ 

$(PATH_OBJ_DEBUG)/$(DEPENDENCY).o: $(SRCS_LIB)
	$(CC) -c $(CPICFLAGS) $(DEBUG) $< -o $@ 
###########################test###########################
TESTS_TARGET:= a.out

test: $(TESTS_TARGET)
	./$(TESTS_TARGET)	
	
$(TESTS_TARGET): $(LIB_DEBUG)
	$(CC) -Wall $(DEBUG) $(CPPFLAGS) -pthread -o $@ $(SRCS_TST) $<
###########################vlg###########################
vlg: $(TESTS_TARGET)	
	$(VLG) ./$(TESTS_TARGET)
###########################clean & phony###########################	
clean: 
	rm -f $(TESTS_TARGET) $(PATH_OBJ_RELEASE)/*.o $(PATH_OBJ_DEBUG)/*.o $(LIB_RELEASE) $(LIB_DEBUG)

.PHONY: clean all 

# $< copy the string that is the first dependency
# $@ is a shortcut for the target (the blue text of the current segment)