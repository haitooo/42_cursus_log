NAME = ex03.out
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
DEPFLAGS = -MMD -MP
OBJDIR = objs
SRCS =	main.cpp \
		Bureaucrat.cpp \
		AForm.cpp \
		ShrubberyCreationForm.cpp \
		RobotomyRequestForm.cpp \
		PresidentialPardonForm.cpp \
		Intern.cpp
INC = -I .
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

TEST_NAME = test.out
TEST_SRCS = test/constructor_test.cpp $(filter-out main.cpp, $(SRCS))
TEST_OBJS = $(addprefix $(OBJDIR)/, $(TEST_SRCS:.cpp=.o))
TEST_DEPS = $(TEST_OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
		rm -f $(NAME)
		rm -f $(TEST_NAME)
		rm -f *_shrubbery

test: $(TEST_NAME)
	@./$(TEST_NAME); status=$$?; rm -f *_shrubbery; exit $$status

$(TEST_NAME): $(TEST_OBJS)
		$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_NAME)

re: fclean all

$(OBJDIR)/%.o:%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INC) -c $< -o $@

-include $(DEPS) $(TEST_DEPS)

.PHONY: all clean fclean re test
