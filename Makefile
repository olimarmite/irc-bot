# Project #
NAME	=	ircserv

# Compiler #
CXX			=	c++
CXX_FLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3 #attention a ne pas push avec
RM			=	rm -rf

# Objects #
OBJ_DIR		=	.objFiles

# Files #
SRC_DIR		=	srcs
INC_DIR		=	includes


SRC_FILES	:=	$(shell find $(SRC_DIR) -name "*.cpp") # TODO Changer ca a la fin mdr

# SRC_FILES	:=	$(wildcard $(SRC_DIR)/*.cpp) #a verifier si on a le droit a wildcard
OBJ_FILES	:=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) #a verifier si on a le droit a patsubst
INC_FILES	:=	$(wildcard $(INC_DIR)/*.hpp)

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
RESET		=	\e[0m

# Progress bar variables
TOTAL		:=	$(words $(SRC_FILES))
COUNT		:=	0

.PHONY: all clean fclean re

# Rules #
all: $(NAME)

$(NAME): $(OBJ_FILES) $(INC_FILES)
	@$(CXX) $(OBJ_FILES) -I $(INC_DIR) -o $(NAME)
	@printf "\n\n$(GREEN)	- Executable ready.\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_FILES)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXX_FLAGS) -I $(INC_DIR) -c $< -o $@
	@$(eval COUNT=$(shell expr $(COUNT) + 1))
	@printf "$(GREEN)	- Compiling: [%-50s] %d%%$(RESET)\r" "$$(printf '▉%.0s' $$(seq 1 $$(expr $(COUNT) \* 50 / $(TOTAL))))" $$(expr $(COUNT) \* 100 / $(TOTAL))

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(YELLOW)	- Object files removed.\n$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(YELLOW)	- Executable removed.\n\n$(RESET)"

re: fclean all
