NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -std=c++98 -I inc

SRCDIR = src
INCDIR = inc
OBJDIR = tmpObj
DEPDIR = tmpDep

SRCS = 	main.cpp Client.cpp Server.cpp Channel.cpp Parser.cpp \
		commands/Nick.cpp \
		commands/User.cpp \
		commands/Join.cpp \
		commands/Privmsg.cpp \
		commands/Pass.cpp \
		commands/Notice.cpp \
		commands/ACommand.cpp \
		commands/CommandsList.cpp \
		commands/Quit.cpp 
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(DEPDIR)/, $(SRCS:.cpp=.d))

all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MF $(DEPDIR)/$*.d

$(OBJDIR) $(DEPDIR):
	mkdir -p $(OBJDIR)/commands $(DEPDIR)/commands

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
