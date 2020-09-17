import time as t
import hashlib
import threading
import subprocess
import socket
from sys import stdin
from random import randint
import os

# CONSTANTS
# host ip
host = subprocess.check_output("ifconfig " + "en0" + " | awk '/inet / {print $2}'", shell=True).decode().split("\n")[0]
discover_port = 5000    # discovering from port 5000
game_port = 5001        # discovering from port 5001
buffer_sıze = 1024
w, h = 3, 3;            # width and height of the game table

# VARIABLES
USERS = []              # stores usernames
ADRESSES = {}           # dictionary for username -> user_ip
other_player_rand_generator = "" # variable used to decide who will start the game
nickname = ""           # host name set at the beginning of the program
game_order = "0"        # order 0 means "O", order 1 means "X", order 1 starts
game_inv_received = False  # becomes True if game invitation is received
in_game = False         # becomes True if user is in game
game_inv_sent = False   # becomes True if user has sent an game invitation and waiting for the response
inv_sent_from = ""      # sendername of the invitation
game_messages = []      # when a move is played, that received message is stored in game_messages
turn_number = 0         # turn number of the game
playing_with = ""       # name of the other user

# Matrix signifies the TicTacToe game table, initialized with empty string
Matrix = [[" " for x in range(w)] for y in range(h)]

# returns True if the Matrix is full of symbols and there is no winner
def check_tie():
    global Matrix,w, h
    if any(" " in sublist for sublist in Matrix): # if there are spots to play
        return False
    else: # all spots are played
        return True

# returns 1 if "X" has won, 0 if "O" won, 2 if it's a tie and -1 if the game continues
def check_finished():
    global Matrix,w,h
    if check_for_symbol("X"):
        return "1"
    elif check_for_symbol("O"):
        return "0"
    elif check_tie():
        return "2"
    else:
        return "-1"

# returns True if a win situation occurs for the specified symbol
def check_for_symbol(symbol):
    if (Matrix[0] == [symbol,symbol,symbol]) or (Matrix[1] == [symbol,symbol,symbol]) or (Matrix[2] == [symbol,symbol,symbol]):
        return True
    elif (getCol(0) == [symbol,symbol,symbol]) or (getCol(1)  == [symbol,symbol,symbol]) or (getCol(2)  == [symbol,symbol,symbol]):
        return True
    elif (Matrix[0][0] == symbol and Matrix[1][1] == symbol and Matrix[2][2] == symbol ):
        return True
    elif (Matrix[0][2] == symbol and Matrix[1][1] == symbol and Matrix[2][0] == symbol ):
        return True
    else:
        return False

# returns index'th column of the Matrix
def getCol(index):
    global Matrix
    return [row[index] for row in Matrix]

# prints Matrix
def print_game_board():
    global Matrix,w,h
    for x in range(w):
        print("\t"+Matrix[x][0]+" | "+ Matrix[x][1] +" | "+Matrix[x][2])
        if(x!=w-1):
            print("\t_________\n")
        else:
            print("\t\n")

# returns "O" for order 0 and "X" for order 1
def getSymbol(order):
    if order == "0":
        return "O"
    elif order == "1":
        return "X"
    else:
        return "-1"

# updates the Matrix with specified position, puts the symbol with respect to the getSymbol(order)
def update_game_board(position, order):
    global Matrix, w, h
    x = int(int(position)/3)
    y = int(int(position)%3)
    symbol = getSymbol(order)
    Matrix[x][y] = symbol

# reads commandline and gets the input
def get_input():
    return stdin.readline().rstrip(" \n\r")

# receives game messages from game_port
def receive_game():
    game_receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    game_receive_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    game_receive_socket.bind((host, game_port))
    while True:
        message,address = game_receive_socket.recvfrom(buffer_sıze)
        if message:
            game_messages.append(message) # adds received message to game_messages

# gets user move from the commandline and sends the move
def send_game(address):
    global game_order,turn_number,in_game,Matrix,w,h
    send_game_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    send_game_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    print("Current board:\n")
    print_game_board()
    print("Your turn!! Enter your move index:")
    input = get_input()
    isValid = get_validation(input) # checks if user input is valid
    while(not(isValid)): # wait until user enters a valid input (a valid int or 'q')
        print("Enter a valid input")
        input = get_input()
        isValid = get_validation(input)
    if(input == "q"):       # user chooses to quit the game
        message = "1;" + str(turn_number)+";"+input # send the game has finished message
        print("You left the game, other player has won ! ")
        in_game = False     # update in_game variable
    else:
        update_game_board(input, game_order) # user plays a move, update the table with input
        print("Updated board:\n")
        print_game_board()
        #check if game has finished
        if check_finished() == "2":    # check if the game is a tie
            print("It's a tie! ")
            in_game = False
            message = "1;" + input+";2" # send the game has finished message
        elif check_finished()==game_order : # check if game ends with the last move
            in_game = False
            print("You won!")
            message = "1;" + str(turn_number)+";"+input # send the game has finished message
        else:
            message = "0;"+ str(turn_number)+";"+input #turn number: game's turn (%2==0 means player1 is playing, othw player2)
    try:
        send_game_socket.sendto(message.encode(), (address,game_port))
    except Exception as e:
        pass
        print("not able to send")

# returns True if input is "q" or an int in range [0,9] and specified slot in Matrix is not occupied
def get_validation(input):
    global Matrix
    if(input == "q"):
        return True
    try:
        input = int(input)
        x = int(input/3)    # row number
        y = int(input%3)    # column number
        return Matrix[x][y] == " "
    except:
        pass
        print("Input is not valid, try again")
        return False

# a thread running at the background, receives discovery messages from discover_port
def receive_discovery():
    global game_order, game_inv_received, in_game, inv_sent_from,other_player_rand_generator,game_inv_sent,playing_with,turn_number
    discovery_receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_receive_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    discovery_receive_socket.bind((host, discover_port))
    while True:
        message,address = discovery_receive_socket.recvfrom(buffer_sıze)
        if message:
            try:
                mod, senderip, sendername, recv, order = message.decode().split(";")
            except ValueError:
                print("value error")
            if mod == "0": # received discovery message
                discover_pck = "1;" + host + ";" + nickname + ";" + senderip + ";" + sendername
                discovery_receive_socket.sendto(discover_pck.encode(), (senderip,discover_port))
                if sendername not in USERS: # if that user is not in the list
                    USERS.append(sendername)
                    ADRESSES[sendername] = senderip
                    print(sendername + " is now online")
                # else:
                #     if ADRESSES[sendername] != senderip:
                #         USERS.append((sendername+"1"))
            if mod == "1": # received reply to a discovery message
                if sendername not in USERS:
                    USERS.append(sendername)
                    ADRESSES[sendername] = senderip
                    print(sendername + " is now online")
            if mod == "2": # received a game invitation
                inv_sent_from = sendername
                if game_inv_sent or game_inv_received or in_game: # if current user is not available, directly reject
                    send_message(inv_sent_from,4,"x")
                else:
                    print( sendername + " wants to play a game with you!! ")
                    game_inv_received = True
                    other_player_rand_generator = recv
            if mod == "3": # other user accepted the game invitation
                print(sendername + " has accepted your invitation")
                turn_number = 0
                in_game = True
                game_inv_sent = False
                if order == "1": # means other user will start
                    print(sendername + " is going to start as X")
                    playing_with = sendername
                    game_order = "0"
                    turn_number = 0
                if order == "0": # this user starts
                    print("Your turn!")
                    playing_with = sendername
                    game_order = "1"
                    turn_number = 0
            if mod == "4": # user has rejected the game invitation
                print(sendername + " rejected")
                game_inv_sent = False
            if mod == "-1":
                del ADRESSES[sendername] # delete userip from the address list
                USERS.remove(sendername) # delete that user
                print(sendername + " is not online")

# sends game invitation to selected user (input is the username)
def send_invitation(input):
    invitation_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    invitation_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    rand = randint(0, 9) # random number to decide who will start the game
    try:
        ip_addr = ADRESSES[input] # get user's ip address
        messagetosend = str(2) + ";" + host + ";" + nickname + ";" + str(rand) + ";0"
        invitation_socket.sendto(messagetosend.encode(), (ip_addr,discover_port))
    except Exception as e:
        pass
        print("not able to send")

# sends message with given parameters, used in accept/reject invitation
def send_message(username,number,order):
    global game_order
    message_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    message_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    try:
        address = ADRESSES[username]
        messagetosend = str(number) + ";" + host + ";" + nickname + ";0;" + str(order) #sender always starts (player that accepts the inv)
        message_socket.sendto(messagetosend.encode(), (address,discover_port))
    except Exception as e:
        pass
        print("not able to send")

# sends discovery message to ip addresses in the same network
def send_discovery():
    discovery_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    discovery_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    ip_addr = host.split(".")
    ip_base = ".".join([ip_addr[0], ip_addr[1], ip_addr[2]])
    for x in range(2, 254):
       ip_addr_real = ip_base + "." + str(x)
       messagetosend = str(0) + ";" + host + ";" + nickname + ";" + "0" +";"+ "0"
       discovery_socket.sendto(messagetosend.encode(), (ip_addr_real,discover_port))

# sends a message with "-1;.." to everyone, signifying that the user has left
def exit():
    exit_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    exit_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    for username in USERS:
        ip_addr = ADRESSES[username]
        messagetosend = str(-1) + ";" + host + ";" + nickname + ";" + "0;" + "0"
        exit_socket.sendto(messagetosend.encode(), (ip_addr,discover_port))

#takes the input from the user when user is online
print("Please enter your nickname:")
nickname = get_input()
print("WELCOME ", nickname,"\n")
if __name__ == '__main__':
    #creates and starts receive discovery threads and receive game messages threads
    main_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    main_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0)
    receive_discovery_thread = threading.Thread(name='receive_discovery', target=receive_discovery)
    receive_game_thread = threading.Thread(name ='receive_game', target = receive_game)
    receive_discovery_thread.start()
    receive_game_thread.start()
    send_discovery()
#UI for user to see online users,to rediscover,to play games and to exit from the program
while True:
    #if game invitation receives run this code and prevent user to use menu
    if game_inv_received:
        Matrix = [[" " for x in range(w)] for y in range(h)] #initialize game matrix at the begining of the game
        game_messages = []  #initialize game messages at the begining of the game
        print("enter A to accept or R to reject") #take user answer as A for accept and R for reject
        input = get_input()
        #if user wants to play a game with other user run this code
        if input == "A":
            rand = randint(0, 9) #generate a randım number to decide who is going to start to play
            if rand > int(other_player_rand_generator) :    #if the random number we generate is bigger than other user's random number
                game_order = "1"                            #means this user is going to play
                send_message(inv_sent_from,3,1)             #send message to other user to indicate this user has accepted the invitation and it she is going to start
            else:
                game_order = "0"                            #if the random number we generate is smaller than other user's random number
                print("0ther player will start, waiting for player to make move!")
                send_message(inv_sent_from,3,0)             #send message to other user to indicate this user has accepted the invitation and it she is not going to start
            playing_with = inv_sent_from                    #specify other user who are the user is going to play with
            game_inv_received = False                       #game received and we are moving to play game part
            in_game = True                                  #we are now in game
            turn_number = 0                                 #beginning of the game, turn number is 0
        #if the answer of the user is reject invitiation
        elif input == "R":
            send_message(inv_sent_from,4,"x")               # send reject message to the user that has invite
            game_inv_received = False
            in_game = False
    #wait the response of the user that has invited
    elif game_inv_sent:
        print("")
    #if game starts , run this code
    elif in_game:
        #while game is on progress, run this code
        while in_game: #(check_finished == "-1")
            #if turn is the user's turn
            if(turn_number % 2 != int(game_order) % 2): #game_order 1 plays for even turns.. player 0 odd turns
                send_game(ADRESSES[playing_with])
                turn_number += 1
            #else, wait for the user to play his move
            else:
                if game_messages: # a message is received
                    message = game_messages[0]
                    game_messages.pop(0)
                    print(str(message))
                    try:
                        mod, turn, position  = message.decode().split(";")  #split message
                    except error:
                        print("error")
                        pass
                    #if game is finished message is received,run this code
                    if mod == "1" :
                        in_game = False
                        if position == "q":                     #if other user quitted the game
                            print("Other player left the game, you won !")
                        elif turn == "2":                       #if there is a tie
                            update_game_board(turn, str((int(game_order)+1)%2))
                            print_game_board()
                            print("It's a tie !")
                        else:                                   #if a players wins the game
                            update_game_board(position, str((int(game_order)+1)%2))
                            print_game_board()
                            if check_finished() == "0":
                                print("O won")
                            elif check_finished() == "1":
                                print("X won")
                    elif (int(turn) == turn_number): #if game has not finished, update turn_number signifying that turn is played
                        update_game_board(position, str((int(game_order)+1)%2))
                        turn_number = int(turn) + 1
                    else:                           #print if the turn number is not valid
                        print("not a valid turn number")
    else:                           #menu UI
        print("1. Show online users")
        print("2. Rediscover")
        print("3. Play game")
        print("4. Exit")
        input = get_input()
        if input == "1":
            print("# Users")
            for u in USERS: #online user list
                print("#   " + u)
            stdin.readline()
        elif input == "2":
            send_discovery()
        elif input == "3":
            print("With whom?")
            for u in USERS:
                print("#   " + u)
            input = get_input()  #selects a user from the list
            if input in USERS: #if the user is in the user list
                game_inv_sent = True        #make invitation sent true to specify we are waiting for a response from the user that we choose
                Matrix = [[" " for x in range(w)] for y in range(h)]
                game_messages = []
                send_invitation(input)    #send invitation
                print("Sent invitation to that user, waiting for other user")
            else:                       #in case of a typo
                print("could not find that user")
            stdin.readline()

        elif input == "4": #to exit the program and say other users that we are leaving the program 
            exit()
            print("###### GOODBYE ", nickname," ######")
            os._exit(0)
        else:
            continue
