NAME = Weapon
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
OBJDIR = objs
SRCS =	main.cpp \
		Weapon.cpp \
		HumanA.cpp \
		HumanB.cpp
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
INC = -I incs

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

$(OBJDIR)/%.o:%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

.PHONY: all clean fclean re
