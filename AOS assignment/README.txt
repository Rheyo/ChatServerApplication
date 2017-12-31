# ChatServerApp
A simplistic version of Chat Server Application developed using C/C++ and Socket Programming

Goal: To simulate a chat-server using Unix domain sockets.

Overview: You are required to code client and server programs to simulate the chat-server for
Unix/Linux environment. The simulation is to be implemented using Unix domain sockets. User-clients
who wish to chat are required to login to the chat-server. They can send messages to any other client
in the lobby. Though a user-client can chat with multiple users, (s)he is not allowed to multicast or
broadcast.

Functional requirements:
Client Functional Requirements:
 User can either login with existing username and password or can register with new ones. The
commands for the same are
register <username> <password>
login <username> <password>
 Upon successful login, user can see names of all logged-in users in the lobby. User can send
messages to other active users using the command
send <receiver_username> <message>
 Client can logout. The command for the same is
logout
 Client can demand the names of online users using the command
lobbystatus
 Users get proper messages for every new user coming online or for every existing user logging
out.
 Every time the lobby gets updated, user automatically gets the new lobby status without
having to explicitly demand it.

Server Functional Requirements:
 Server authenticates every user.
 Server manages registrations of new users.
 Server maintains the lobby details at every time.
 Server can broadcast messages. These messages will be broadcasted to every online clientusers.
The command for broadcasting is
broadcast <message>
 Server can send message to any specific client using the command
send <receiver_username> <message>
 Server can kick any user from the lobby using the command
kick <username> <optional-field-reason_for_the_kick>
All other users will get proper message informing them about the kick.
 Server can ban any user. This user will be kicked from the lobby (if online) and won’t be able to
log in further using same username and password. Also any client can get unbanned too. The
commands to ban and unban are
ban <username> <optional-field-reason_for_the_ban>
unban <username>
All other users will get proper message informing them about the ban/unban.
 Server also maintains several logs for various activities using proper timestamps.
 Server can terminate using the command
logout
