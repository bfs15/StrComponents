SRC = src/

bin = fortes
Path = Path

OBJS = $(SRC)$(Path).o $(SRC)$(bin).o

LIBS = -lcgraph

all: $(bin)

rebuild: clean all

# Tool invocations
$(bin): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -std=c++11 -o "$(bin)" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(SRCDIR)%.o: $(SRCDIR)%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -O3 -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean:
	rm -rf  ./$(SRC)*.o  ./$(SRC)*.d  $(bin)
