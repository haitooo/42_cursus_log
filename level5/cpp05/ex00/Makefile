NAME = ex00.out
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
OBJDIR = objs
SRCS =	main.cpp \
		Bureaucrat.cpp
INC = -I .
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

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
