NAME		:= webserv

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98

SRCS_DIR	:= srcs
INCS_DIR	:= include
SCHEMA_DIR	:= schema
OBJS_DIR	:= objs

INCLUDES	:= -I$(INCS_DIR) -I$(SCHEMA_DIR) -I$(SRCS_DIR)

CPPFLAGS	:= $(INCLUDES) -MMD -MP

SRCS_MAIN		:= main.cpp
SRCS_CONFIG		:= Config.cpp ConfigParser.cpp Parser.cpp TokenStream.cpp Tokenizer.cpp
SRCS_SERVER		:= Server.cpp PollEventManager.cpp
SRCS_HTTP		:= RequestParser.cpp
SRCS_HANDLER	:= RequestHandler.cpp
SRCS_CGI		:= CgiExecutor.cpp
SRCS_UTILS		:= Log.cpp Random.cpp SessionStore.cpp Signal.cpp

SRCS		:= $(SRCS_MAIN) \
			   $(SRCS_CONFIG) \
			   $(SRCS_SERVER) \
			   $(SRCS_HTTP) \
			   $(SRCS_HANDLER) \
			   $(SRCS_CGI) \
			   $(SRCS_UTILS)

CGI_EXE_SRC	:= cgi-bin/hello_bin.cpp
CGI_EXE		:= cgi-bin/hello_bin

OBJS		:= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

all: $(NAME) $(CGI_EXE)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(CGI_EXE): $(CGI_EXE_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME) $(CGI_EXE)

re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re
