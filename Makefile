# Get all .cc files
SRCS = $(wildcard *.cc)
# Get corresponding .o files
OBJS := $(SRCS:%.cc=%.o)
# Get corresponding .d files
DEPS := $(SRCS:%.cc=%.d)

# These will run every time (not just when the files are newer)
.PHONY: run_tcp run_udp clean zip

# Main target
ipkcpc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Dependecies
%.o: %.cc %.d
	$(CC) -MT $@ -MMD -MP -MF $*.d $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<
$(DEPS):
include $(wildcard $(DEPS))

clean:
	rm -f *.o *.d ipkcpc xkucha28.zip

run_tcp: ipkcpc
	./ipkcpc -h 127.0.0.1 -p 1234 -m tcp

run_udp: ipkcpc
	./ipkcpc -h 127.0.0.1 -p 1234 -m udp

zip: clean
	zip xkucha28.zip *
