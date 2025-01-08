########### V A R I A B L E S #############

NAME        = ircserv
INCLUDES    = -Iincludes
SRC_DIR     = srcs
OBJ_DIR     = obj
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
RM          = rm -f

############### S O U R C E S #############

SRC_FILES   := $(shell find $(SRC_DIR) -type f -name "*.cpp")
SRCS        := $(SRC_FILES)
OBJS        := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

################ R U L E S ################

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(OBJS) $(CXXFLAGS) $(INCLUDES) -o $(NAME)
	@echo "[INFO] Compilation finished: $(NAME)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "[INFO] Compiled: $<"

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "[INFO] Object files cleaned."

fclean: clean
	@$(RM) $(NAME)
	@echo "[INFO] Executable cleaned."

re: fclean all

# Testing rules
test: $(NAME)
	./$(NAME) 6667 password

vtest: $(NAME)
	valgrind ./$(NAME) 6667 password

