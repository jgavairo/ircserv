NAME = ircserv
BONUS_SERVER_NAME = ircserv_bonus
BONUS_BOT_NAME = ircbot

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -std=c++98
STD_CXXFLAGS = $(CXXFLAGS) -I inc
BONUS_CXXFLAGS = $(CXXFLAGS) -I bonusPart/inc

SRCDIR = src
INCDIR = inc
OBJDIR = tmpObj
DEPDIR = tmpDep

BONUS_SRCDIR = bonusPart/src
BONUS_INCDIR = bonusPart/inc
BONUS_OBJDIR = bonusPart/tmpObj
BONUS_DEPDIR = bonusPart/tmpDep

SRCS = 	main.cpp Client.cpp Server.cpp Channel.cpp Parser.cpp \
		commands/Nick.cpp \
		commands/User.cpp \
		commands/Join.cpp \
		commands/Privmsg.cpp \
		commands/Pass.cpp \
		commands/Part.cpp \
		commands/Invite.cpp \
		commands/Kick.cpp \
		commands/List.cpp \
		commands/Notice.cpp \
		commands/Topic.cpp \
		commands/ACommand.cpp \
		commands/CommandsList.cpp \
		commands/Quit.cpp \
		commands/Mode.cpp 

BONUS_SERVER_SRCS = $(SRCS:.cpp=.cpp) commands/AddBot.cpp commands/BotPunchline.cpp commands/BotTime.cpp commands/BotHelp.cpp
BONUS_BOT_SRCS = ircBot/main.cpp ircBot/IrcBot.cpp

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(DEPDIR)/, $(SRCS:.cpp=.d))

BONUS_SERVER_OBJS = $(addprefix $(BONUS_OBJDIR)/, $(BONUS_SERVER_SRCS:.cpp=.o))
BONUS_BOT_OBJS = $(addprefix $(BONUS_OBJDIR)/, $(BONUS_BOT_SRCS:.cpp=.o))
BONUS_DEPS = $(addprefix $(BONUS_DEPDIR)/, $(BONUS_SERVER_SRCS:.cpp=.d) $(BONUS_BOT_SRCS:.cpp=.d))

all: $(NAME)

-include $(DEPS)
-include $(BONUS_DEPS)

$(NAME): $(OBJS)
	$(CXX) $(STD_CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	mkdir -p $(dir $@)
	mkdir -p $(dir $(DEPDIR)/$*.d)
	$(CXX) $(STD_CXXFLAGS) -c $< -o $@ -MMD -MF $(DEPDIR)/$*.d

$(OBJDIR) $(DEPDIR):
	mkdir -p $(OBJDIR)/commands $(DEPDIR)/commands

bonus: create_bonus_dirs bonus_server bonus_bot

# Cible pour créer tous les répertoires nécessaires pour les bonus
create_bonus_dirs:
	mkdir -p $(BONUS_OBJDIR)/commands $(BONUS_DEPDIR)/commands
	mkdir -p $(BONUS_OBJDIR)/ircBot $(BONUS_DEPDIR)/ircBot

bonus_server: $(BONUS_SERVER_OBJS)
	$(CXX) $(BONUS_CXXFLAGS) -o $(BONUS_SERVER_NAME) $(BONUS_SERVER_OBJS)

bonus_bot: $(BONUS_BOT_OBJS)
	$(CXX) $(BONUS_CXXFLAGS) -o $(BONUS_BOT_NAME) $(BONUS_BOT_OBJS)

$(BONUS_OBJDIR)/%.o: $(BONUS_SRCDIR)/%.cpp | create_bonus_dirs
	mkdir -p $(dir $@)
	mkdir -p $(dir $(BONUS_DEPDIR)/$*.d)
	$(CXX) $(BONUS_CXXFLAGS) -c $< -o $@ -MMD -MF $(BONUS_DEPDIR)/$*.d

clean:
	rm -f $(OBJS) $(DEPS) $(BONUS_SERVER_OBJS) $(BONUS_BOT_OBJS) $(BONUS_DEPS)
	rm -rf $(OBJDIR) $(DEPDIR) $(BONUS_OBJDIR) $(BONUS_DEPDIR)

fclean: clean
	rm -f $(NAME) $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME)

re: fclean all

bonus_re: fclean bonus

.PHONY: all clean fclean re bonus create_bonus_dirs bonus_server bonus_bot bonus_re