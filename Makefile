# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fathmanazmeen <fathmanazmeen@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/07 08:53:04 by afarheen          #+#    #+#              #
#    Updated: 2024/04/19 18:20:10 by fathmanazme      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

MY_SOURCES = main.cpp 

MY_OBJS = $(MY_SOURCES:.cpp=.o)

CXXFLAGS =  -Wall -Wextra -Werror -std=c++98

CXX = c++

all: $(NAME)

$(NAME): $(MY_OBJS)
	$(CXX) $(CXXFLAGS) $(MY_SOURCES) -o $(NAME)

clean:
	rm -f $(MY_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
