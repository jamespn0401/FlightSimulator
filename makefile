CC = g++
CFLAGS = -g -Wall
RMF = rm -r -f

BUILD_DIR := ./Build
SRC_DIRS := ./Sources
SRCS := $(wildcard $(SRC_DIRS)/*.cpp $(SRC_DIRS)/*.c $(SRC_DIRS)/*.s)
OBJS := $(patsubst $(SRC_DIRS)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAIN := AudioMain

# The final build step.
$(BUILD_DIR)/$(MAIN): $(OBJS)
	@echo "Linking $(MAIN)..."
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(MAIN) $(OBJS)

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.cpp
	@echo "Compiling $<..."
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run the executable
run: $(BUILD_DIR)/$(MAIN)
	@echo "Running $(MAIN)..."
	$(BUILD_DIR)/$(MAIN)

# Clean rule to remove generated files
clean:
	@echo "Cleaning up..."
	$(RMF) $(BUILD_DIR)

