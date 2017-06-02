
SRCDIR = src
BUILDDIR = obj

SRCNAMES := $(shell find $(SOURCEDIR) -name '*.cpp' -type f -exec basename {} \;)

OBJECTS := $(addprefix $(BUILDDIR)/, $(SRCNAMES:%.cpp=%.o))
SRCS := $(addprefix $(SRCDIR)/, $(SRCNAMES))

LIBS = -lcgraph

WARN = -Wall -Wextra -Werror
FLAGS = -O3 $(WARN)

# Executable filename
bin = fortes

all: obj_dir list_srcnames $(bin)

set_debug:
	$(eval FLAGS = -O0 -g $(WARN))

debug: set_debug all

rebuild: clean all

buildclean: all clean

obj_dir:
	mkdir -p $(BUILDDIR)

list_srcnames:
	@echo
	@echo "Found source files to compile:"
	@echo $(SRCNAMES)
	@echo

# Tool invocations
$(bin): $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -std=c++11 -o "$(bin)" $(OBJECTS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 $(FLAGS) $(LIBS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo

clean:
	rm -rf  ./$(BUILDDIR)/*.o  ./$(BUILDDIR)/*.d

cleanAll: clean
	rm -rf  $(bin)
