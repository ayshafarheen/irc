# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhaamid <juhaamid@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/07 08:53:04 by afarheen          #+#    #+#              #
#    Updated: 2024/05/31 16:25:13 by juhaamid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

MY_SOURCES = main.cpp server.cpp client.cpp parse.cpp Channel.cpp mode.cpp

MY_OBJS = $(MY_SOURCES:.cpp=.o)

CXXFLAGS =  -Wall -Wextra -Werror -g -fsanitize=address -std=c++98  -g3

CXX = c++

all: $(NAME)

$(NAME): $(MY_OBJS)
	$(CXX) $(CXXFLAGS) $(MY_SOURCES) -o $(NAME)

clean:
	rm -f $(MY_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
