NAME = Webserver

CXXFLAGS = -Wall -Wextra -Wshadow -std=c++98

CXX = c++

OBJS	=	$(SRCS:.cpp=.o)

SRCS =  main.cpp\
        Request.cpp\
        Route.cpp\
        RouterBuilder.cpp\
        Server.cpp\
        ServerBuilder.cpp\
        ServerPoll.cpp\
        tools.cpp\
		Response.cpp

all: $(NAME)

$(NAME) : $(OBJS)
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
