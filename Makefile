NAME = ircserv
BONUS_SERVER_NAME = ircserv_bonus
BONUS_BOT_NAME = ircbot

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
STD_CXXFLAGS = $(CXXFLAGS) -I inc
BONUS_CXXFLAGS = $(CXXFLAGS) -I bonusPart/inc

# Définition des couleurs
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[1;31m
RESET = \033[0m

SRCDIR = src
OBJDIR = tmpObj
DEPDIR = tmpDep

BONUS_SRCDIR = bonusPart/src
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

BONUS_SERVER_SRCS = $(BONUS_SRCDIR)/main.cpp $(BONUS_SRCDIR)/Client.cpp $(BONUS_SRCDIR)/Server.cpp \
					$(BONUS_SRCDIR)/Channel.cpp $(BONUS_SRCDIR)/Parser.cpp \
					$(BONUS_SRCDIR)/commands/Nick.cpp \
					$(BONUS_SRCDIR)/commands/User.cpp \
					$(BONUS_SRCDIR)/commands/Join.cpp \
					$(BONUS_SRCDIR)/commands/Privmsg.cpp \
					$(BONUS_SRCDIR)/commands/Pass.cpp \
					$(BONUS_SRCDIR)/commands/Part.cpp \
					$(BONUS_SRCDIR)/commands/Invite.cpp \
					$(BONUS_SRCDIR)/commands/Kick.cpp \
					$(BONUS_SRCDIR)/commands/List.cpp \
					$(BONUS_SRCDIR)/commands/Notice.cpp \
					$(BONUS_SRCDIR)/commands/Topic.cpp \
					$(BONUS_SRCDIR)/commands/ACommand.cpp \
					$(BONUS_SRCDIR)/commands/CommandsList.cpp \
					$(BONUS_SRCDIR)/commands/Quit.cpp \
					$(BONUS_SRCDIR)/commands/Mode.cpp \
					$(BONUS_SRCDIR)/commands/AddBot.cpp \
					$(BONUS_SRCDIR)/commands/BotPunchline.cpp \
					$(BONUS_SRCDIR)/commands/BotTime.cpp \
					$(BONUS_SRCDIR)/commands/BotHelp.cpp

BONUS_BOT_SRCS = $(BONUS_SRCDIR)/ircBot/main.cpp $(BONUS_SRCDIR)/ircBot/IrcBot.cpp

# Trouver tous les en-têtes
HEADERS = $(wildcard inc/*.hpp) $(wildcard inc/*/*.hpp)
BONUS_HEADERS = $(wildcard bonusPart/inc/*.hpp) $(wildcard bonusPart/inc/*/*.hpp)

OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(addprefix $(SRCDIR)/,$(SRCS)))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(addprefix $(SRCDIR)/,$(SRCS)))

BONUS_SERVER_OBJS = $(BONUS_SERVER_SRCS:$(BONUS_SRCDIR)/%.cpp=$(BONUS_OBJDIR)/%.o)
BONUS_BOT_OBJS = $(BONUS_BOT_SRCS:$(BONUS_SRCDIR)/%.cpp=$(BONUS_OBJDIR)/%.o)

# Nombre total de fichiers à compiler
STD_TOTAL_FILES := $(words $(SRCS))
BONUS_TOTAL_FILES := $(words $(notdir $(BONUS_SERVER_SRCS)) $(notdir $(BONUS_BOT_SRCS)))

# Compteurs globaux pour les barres de progression
export STD_COMPILED_FILES := 0
export BONUS_COMPILED_FILES := 0

.SILENT:

all:
	@if $(MAKE) --no-print-directory --question $(NAME); then \
		echo "$(GREEN)Rien à faire pour '$(NAME)'. Déjà compilé.$(RESET)"; \
	else \
		$(MAKE) --no-print-directory $(NAME); \
	fi

# Créer un fichier timestamp pour suivre les changements des en-têtes
$(OBJDIR)/.headers_timestamp: $(HEADERS)
	@mkdir -p $(OBJDIR)
	@touch $@

$(BONUS_OBJDIR)/.headers_timestamp: $(BONUS_HEADERS)
	@mkdir -p $(BONUS_OBJDIR)
	@touch $@

# Inclure les fichiers de dépendance générés par le compilateur
-include $(DEPS)
-include $(wildcard $(BONUS_DEPDIR)/*.d)
-include $(wildcard $(BONUS_DEPDIR)/*/*.d)

$(NAME): $(OBJDIR)/.headers_timestamp
	@if $(MAKE) --no-print-directory --question $(OBJS); then \
		echo "$(GREEN)Rien à faire pour '$(NAME)'. Déjà compilé.$(RESET)"; \
	else \
		echo "$(GREEN)Compilation du serveur IRC standard...$(RESET)"; \
		export STD_COMPILED_FILES=0; \
		$(MAKE) --no-print-directory $(OBJS); \
		$(CXX) $(STD_CXXFLAGS) -o $@ $(OBJS); \
		echo "\n$(GREEN)Compilation terminée : $(NAME) est prêt !$(RESET)"; \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*.d)
	@$(eval STD_COMPILED_FILES=$(shell echo $$(($(STD_COMPILED_FILES)+1))))
	@printf "\r$(YELLOW)Compilation [%3d%%] [" "$$(echo $$(($(STD_COMPILED_FILES) * 100 / $(STD_TOTAL_FILES))))"
	@for i in $$(seq 1 $$(echo $$(($(STD_COMPILED_FILES) * 30 / $(STD_TOTAL_FILES))))); do printf "#"; done
	@for i in $$(seq 1 $$(echo $$((30 - $(STD_COMPILED_FILES) * 30 / $(STD_TOTAL_FILES))))); do printf " "; done
	@printf "] $(notdir $<)$(RESET)"
	@$(CXX) $(STD_CXXFLAGS) -MMD -MP -c $< -o $@ -MF $(DEPDIR)/$*.d

bonus: $(BONUS_OBJDIR)/.headers_timestamp
	@if $(MAKE) --no-print-directory --question $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME); then \
		echo "$(BLUE)Rien à faire pour 'bonus'. Déjà compilé.$(RESET)"; \
	else \
		echo "$(BLUE)Compilation du serveur IRC bonus et du bot...$(RESET)"; \
		export BONUS_COMPILED_FILES=0; \
		$(MAKE) --no-print-directory $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME); \
		echo "\n$(BLUE)Compilation terminée : $(BONUS_SERVER_NAME) et $(BONUS_BOT_NAME) sont prêts !$(RESET)"; \
	fi

$(BONUS_SERVER_NAME): $(BONUS_SERVER_OBJS)
	@$(CXX) $(BONUS_CXXFLAGS) -o $@ $^

$(BONUS_BOT_NAME): $(BONUS_BOT_OBJS)
	@$(CXX) $(BONUS_CXXFLAGS) -o $@ $^

$(BONUS_OBJDIR)/%.o: $(BONUS_SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(BONUS_DEPDIR)/$*.d)
	@$(eval BONUS_COMPILED_FILES=$(shell echo $$(($(BONUS_COMPILED_FILES)+1))))
	@printf "\r$(YELLOW)Compilation [%3d%%] [" "$$(echo $$(($(BONUS_COMPILED_FILES) * 100 / $(BONUS_TOTAL_FILES))))"
	@for i in $$(seq 1 $$(echo $$(($(BONUS_COMPILED_FILES) * 30 / $(BONUS_TOTAL_FILES))))); do printf "#"; done
	@for i in $$(seq 1 $$(echo $$((30 - $(BONUS_COMPILED_FILES) * 30 / $(BONUS_TOTAL_FILES))))); do printf " "; done
	@printf "] $(notdir $<)$(RESET)"
	@$(CXX) $(BONUS_CXXFLAGS) -MMD -MP -c $< -o $@ -MF $(BONUS_DEPDIR)/$*.d

clean:
	@echo "$(RED)Suppression des fichiers objets...$(RESET)"
	@rm -rf $(OBJDIR) $(DEPDIR) $(BONUS_OBJDIR) $(BONUS_DEPDIR)
	@echo "$(RED)Fichiers objets supprimés !$(RESET)"

fclean: clean
	@echo "$(RED)Suppression des exécutables...$(RESET)"
	@rm -f $(NAME) $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME)
	@echo "$(RED)Exécutables supprimés !$(RESET)"

re: fclean all

bonus_re: fclean bonus

.PHONY: all clean fclean re bonus bonus_re