NAME = ircserv
BONUS_SERVER_NAME = ircserv_bonus
BONUS_BOT_NAME = ircbot

SERVER_PORT = 6667
SERVER_PASSWORD = 12

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

# Construire les chemins complets vers les fichiers source
FULL_SRCS = $(addprefix $(SRCDIR)/,$(SRCS))

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
	@need_compile=false; \
	for src in $(FULL_SRCS); do \
		obj=$$(echo $$src | sed "s|$(SRCDIR)|$(OBJDIR)|" | sed "s|.cpp|.o|"); \
		if [ ! -f "$$obj" ] || [ "$$src" -nt "$$obj" ]; then \
			need_compile=true; \
			break; \
		fi; \
	done; \
	if [ "$$need_compile" = "false" ]; then \
		for header in $(HEADERS); do \
			for obj in $(OBJS); do \
				if [ ! -f "$$obj" ] || [ "$$header" -nt "$$obj" ]; then \
					need_compile=true; \
					break 2; \
				fi; \
			done; \
		done; \
	fi; \
	if [ "$$need_compile" = "false" ] && [ -f $(NAME) ]; then \
		for obj in $(OBJS); do \
			if [ "$$obj" -nt "$(NAME)" ]; then \
				need_compile=true; \
				break; \
			fi; \
		done; \
	else \
		need_compile=true; \
	fi; \
	if [ "$$need_compile" = "true" ]; then \
		echo "$(GREEN)Compilation du serveur IRC standard...$(RESET)"; \
		$(MAKE) --no-print-directory $(NAME); \
	else \
		echo "$(GREEN)Rien à faire pour '$(NAME)'. Déjà compilé.$(RESET)"; \
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

$(NAME): $(OBJDIR)/.headers_timestamp $(OBJS)
	@$(CXX) $(STD_CXXFLAGS) -o $@ $(OBJS)
	@echo "\n$(GREEN)Compilation terminée : $(NAME) est prêt !$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*.d)
	@$(eval STD_COMPILED_FILES=$(shell echo $$(($(STD_COMPILED_FILES)+1))))
	@printf "\r$(YELLOW)Compilation [%3d%%] [" "$$(echo $$(($(STD_COMPILED_FILES) * 100 / $(STD_TOTAL_FILES))))"
	@for i in $$(seq 1 $$(echo $$(($(STD_COMPILED_FILES) * 30 / $(STD_TOTAL_FILES))))); do printf "#"; done
	@for i in $$(seq 1 $$(echo $$((30 - $(STD_COMPILED_FILES) * 30 / $(STD_TOTAL_FILES))))); do printf " "; done
	@printf "] $(notdir $<)$(RESET)"
	@$(CXX) $(STD_CXXFLAGS) -MMD -MP -c $< -o $@ -MF $(DEPDIR)/$*.d

bonus:
	@need_compile=false; \
	for src in $(BONUS_SERVER_SRCS) $(BONUS_BOT_SRCS); do \
		obj=$$(echo $$src | sed "s|$(BONUS_SRCDIR)|$(BONUS_OBJDIR)|" | sed "s|.cpp|.o|"); \
		if [ ! -f "$$obj" ] || [ "$$src" -nt "$$obj" ]; then \
			need_compile=true; \
			break; \
		fi; \
	done; \
	if [ "$$need_compile" = "false" ]; then \
		for header in $(BONUS_HEADERS); do \
			for obj in $(BONUS_SERVER_OBJS) $(BONUS_BOT_OBJS); do \
				if [ ! -f "$$obj" ] || [ "$$header" -nt "$$obj" ]; then \
					need_compile=true; \
					break 2; \
				fi; \
			done; \
		done; \
	fi; \
	if [ "$$need_compile" = "false" ] && [ -f $(BONUS_SERVER_NAME) ] && [ -f $(BONUS_BOT_NAME) ]; then \
		for obj in $(BONUS_SERVER_OBJS); do \
			if [ "$$obj" -nt "$(BONUS_SERVER_NAME)" ]; then \
				need_compile=true; \
				break; \
			fi; \
		done; \
		if [ "$$need_compile" = "false" ]; then \
			for obj in $(BONUS_BOT_OBJS); do \
				if [ "$$obj" -nt "$(BONUS_BOT_NAME)" ]; then \
					need_compile=true; \
					break; \
				fi; \
			done; \
		fi; \
	else \
		need_compile=true; \
	fi; \
	if [ "$$need_compile" = "true" ]; then \
		echo "$(BLUE)Compilation du serveur IRC bonus et du bot...$(RESET)"; \
		$(MAKE) --no-print-directory $(BONUS_OBJDIR)/.headers_timestamp; \
		export BONUS_COMPILED_FILES=0; \
		$(MAKE) --no-print-directory $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME); \
		echo "\n$(BLUE)Compilation terminée : $(BONUS_SERVER_NAME) et $(BONUS_BOT_NAME) sont prêts !$(RESET)"; \
	else \
		echo "$(BLUE)Rien à faire pour 'bonus'. Déjà compilé.$(RESET)"; \
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

run_bonus:
	@if [ ! -f $(BONUS_SERVER_NAME) ] || [ ! -f $(BONUS_BOT_NAME) ]; then \
		echo "$(RED)Les exécutables n'existent pas. Veuillez d'abord les compiler avec 'make bonus'.$(RESET)"; \
	else \
		echo "$(GREEN)Lancement du serveur et du bot avec les paramètres $(SERVER_PORT) $(SERVER_PASSWORD)...$(RESET)"; \
		echo "$(YELLOW)Serveur lancé en arrière-plan. Pour arrêter, utilisez 'killall $(BONUS_SERVER_NAME)'$(RESET)"; \
		./$(BONUS_SERVER_NAME) $(SERVER_PORT) $(SERVER_PASSWORD) > server.log 2>&1 & \
		SERVER_PID=$$!; \
		echo "$(YELLOW)PID du serveur : $$SERVER_PID$(RESET)"; \
		sleep 2; \
		echo "$(YELLOW)Bot lancé en arrière-plan. Pour arrêter, utilisez 'killall $(BONUS_BOT_NAME)'$(RESET)"; \
		./$(BONUS_BOT_NAME) $(SERVER_PORT) $(SERVER_PASSWORD) > bot.log 2>&1 & \
		BOT_PID=$$!; \
		echo "$(YELLOW)PID du bot : $$BOT_PID$(RESET)"; \
		echo "$(GREEN)Les logs sont disponibles dans server.log et bot.log$(RESET)"; \
	fi

# Ajouter cette cible pour arrêter les programmes
stop_bonus:
	@echo "$(RED)Arrêt du serveur et du bot...$(RESET)"
	@killall $(BONUS_SERVER_NAME) 2>/dev/null || true
	@killall $(BONUS_BOT_NAME) 2>/dev/null || true
	@echo "$(RED)Programmes arrêtés.$(RESET)"

clean:
	@echo "$(RED)Suppression des fichiers objets...$(RESET)"
	@rm -rf $(OBJDIR) $(DEPDIR) $(BONUS_OBJDIR) $(BONUS_DEPDIR)
	@echo "$(RED)Fichiers objets supprimés !$(RESET)"

fclean: clean
	@echo "$(RED)Suppression des exécutables...$(RESET)"
	@rm -f $(NAME) $(BONUS_SERVER_NAME) $(BONUS_BOT_NAME) server.log bot.log
	@echo "$(RED)Exécutables supprimés !$(RESET)"

re: fclean all

bonus_re: fclean bonus

.PHONY: all clean fclean re bonus bonus_re