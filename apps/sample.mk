#
# --- Auto-generated file for target sample --- 
#

include apps.mk
VPATH = sample/:sample/objs

sample_OBJS = sample.o

libsample.a : $(sample_OBJS)
	@if (! -d libs) mkdir libs
	@ar -crs libs/$(@) sample/objs/*.o

$(sample_OBJS)  : %.o : %.cpp $(DRIVER_SRC:.cpp=.h) $(JUMP_SRC:.cpp=.h) $(OPTHPA_SRC:.cpp=.h) $(HPASTAR_SRC:.cpp=.h) $(AHASTAR_SRC:.cpp=.h) $(SHARED_SRC:.cpp=.h) $(UTIL_SRC:.cpp=.h) $(SIMULATION_SRC:.cpp=.h) $(RTS_OBJS:.o=.h)
	@if (! -d sample/objs) mkdir sample/objs
	@-$(RM) libs/libsample.a
	$(CC) $(CFLAGS) -c -o sample/objs/$(notdir $(@)) sample/$(patsubst %.o,%.cpp,$(@))

