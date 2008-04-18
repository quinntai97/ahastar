VPATH = aha/:abstraction/:driver/:shared/:simulation/:util/:objs/:apps/libs:bin/

AHASTAR_SRC = $(notdir $(wildcard aha/*.cpp))
AHASTAR_OBJ = $(AHASTAR_SRC:.cpp=.o)

ABSTRACTION_SRC = $(notdir $(wildcard abstraction/*.cpp))
ABSTRACTION_OBJ = $(ABSTRACTION_SRC:.cpp=.o)

DRIVER_SRC = $(notdir $(wildcard driver/*.cpp))
DRIVER_OBJ = $(DRIVER_SRC:.cpp=.o)

SHARED_SRC = $(notdir $(wildcard shared/*.cpp))
SHARED_OBJ = $(SHARED_SRC:.cpp=.o)

SIMULATION_SRC = $(notdir $(wildcard simulation/*.cpp))
SIMULATION_OBJ = $(SIMULATION_SRC:.cpp=.o)

UTIL_SRC = $(notdir $(wildcard util/*.cpp))
UTIL_OBJ = $(UTIL_SRC:.cpp=.o)

# TARGETS = $(notdir $(wildcard apps/*))
TARGETS = $(filter-out %~ CVS Makefile %.mk libs,$(notdir $(wildcard apps/*)))

.PHONY : $(addprefix lib, $(addsuffix .a, $(TARGETS)))

CFLAGS = -Wall -O3 -g -I./aha -I./abstraction -I./driver -I./shared -I./simulation -I./util 

CC = c++

ifeq ($(findstring "Darwin", "$(shell uname -s)"), "Darwin")

LIBFLAGS = -framework Foundation -framework AppKit -Lapps/libs
CFLAGS += -DOS_MAC  

ifeq ("$(OPENGL)", "STUB")
CFLAGS += -I./driver/STUB/ -I./driver/STUB/GL/ -DNO_OPENGL
else
LIBFLAGS += -framework GLUT -framework OpenGL
CFLAGS += -I/System/Library/Frameworks/GLUT.framework/Versions/A/Headers/
CFLAGS += -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/
CFLAGS += -I/System/Library/Frameworks/AGL.framework/Versions/A/Headers/
endif

else # not darwin
LIBFLAGS = -Lapps/libs -L/usr/X11R6/lib64 -L/usr/X11R6/lib -L/usr/lib -L$(HOME)/lib

ifeq ("$(OPENGL)", "STUB")
CFLAGS += -I./driver/STUB/ -I./driver/STUB/GL/ -DNO_OPENGL
else
CFLAGS += -I/usr/include/GL
LIBFLAGS +=  -lGL -lGLU -lglut -lXi -lXmu
endif

endif

ifeq ("$(CPU)", "G5")
 CFLAGS += -mcpu=970 -mpowerpc64 -mtune=970
 CFLAGS += -mpowerpc-gpopt -force_cpusubtype_ALL
endif

all : $(TARGETS)

$(TARGETS) : % : lib%.a $(UTIL_OBJ) $(SIMULATION_OBJ) $(ABSTRACTION_OBJ) $(SHARED_OBJ) $(AHASTAR_OBJ) $(DRIVER_OBJ) 
	$(CC)	$(CFLAGS) $(LIBFLAGS) -o $(addprefix bin/,$(@)) \
		$(addprefix objs/,$(UTIL_OBJ)) \
		$(addprefix objs/,$(SIMULATION_OBJ)) \
		$(addprefix objs/,$(ABSTRACTION_OBJ)) \
		$(addprefix objs/,$(SHARED_OBJ)) \
		$(addprefix objs/,$(AHASTAR_OBJ)) \
		$(addprefix objs/,$(DRIVER_OBJ)) \
		-l$(@)

$(addprefix lib, $(addsuffix .a, $(TARGETS))) :
	@cd apps; $(MAKE); $(MAKE) -f $(patsubst lib%,%.mk,$(basename $(@))) OPENGL=$(OPENGL) $(@); cd ..


$(UTIL_OBJ) : %.o : %.cpp $(UTIL_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix util/,$(notdir $(@:.o=.cpp)))

$(SIMULATION_OBJ) : %.o : %.cpp $(SIMULATION_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix simulation/,$(notdir $(@:.o=.cpp)))

$(ABSTRACTION_OBJ) : %.o : %.cpp $(ABSTRACTION_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix abstraction/,$(notdir $(@:.o=.cpp)))

$(SHARED_OBJ) : %.o : %.cpp $(SHARED_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h) $(SIMULATION_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix shared/,$(notdir $(@:.o=.cpp)))

$(AHASTAR_OBJ) : %.o : %.cpp $(AHASTAR_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h) $(SIMULATION_SRC:.cpp=.h) $(SHARED_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix aha/,$(notdir $(@:.o=.cpp)))

$(DRIVER_OBJ) : %.o : %.cpp $(AHASTAR_SRC:.cpp=.h) $(DRIVER_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h) $(SIMULATION_SRC:.cpp=.h) $(SHARED_SRC:.cpp=.h)
	$(CC) $(CFLAGS) -c -o $(addprefix objs/,$(@)) \
		$(addprefix driver/,$(notdir $(@:.o=.cpp)))
clean:
	@-$(RM) objs/*.o
	@-$(RM) bin/*
	@cd apps; $(MAKE) clean; cd ..

