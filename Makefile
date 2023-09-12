CXX			:=	c++
CXXFLAGS	:= -std=c++98 -g
#-Wall -Wextra -Werror
LDFLAGS		:=

SRCDIR		:=	src

SRCS		:=	$(SRCDIR)/main.cpp\
				$(SRCDIR)/Webserv.cpp\
				$(SRCDIR)/Worker.cpp\
				$(SRCDIR)/Transaction.cpp\
				$(SRCDIR)/Request.cpp\
				$(SRCDIR)/Response.cpp\
				$(SRCDIR)/block_parser.cpp\
				$(SRCDIR)/tokenizer.cpp\
				$(SRCDIR)/token_utils.cpp\
				$(SRCDIR)/token_parse_error.cpp\
				$(SRCDIR)/Location.cpp\
				$(SRCDIR)/LocationLexer.cpp\
				$(SRCDIR)/WorkerLexer.cpp\
				$(SRCDIR)/CgiHandler.cpp\

OBJS		:=	$(SRCS:.cpp=.o)

NAME		:= webserv

.SUFFIXES : .cpp .o
.cpp.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@

all	: $(NAME)

$(NAME)	:	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean :
	rm -f $(OBJS)

fclean :
	make clean
	rm -f $(NAME)

re :
	make fclean
	make $(NAME)

.PHONY : all clean fclean re
