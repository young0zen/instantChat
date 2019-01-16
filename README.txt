This is an easy command line implementation of instant chat software using
standard socket API under UNIX.

To compile, type command
"make"

To set up a connection, simply start up the server program, then try to
connect to the server using client program.

server usage:
"./server port username"

client usage:
"./client host port username"

For dual-direction communication, this implementation make use of multithreading
to accomplish this goal.

To quit conversation, press control + D to send EOF or type in "END" to quit.

If your peer has quited, you'll be prompt with "peer quited", then you can quit
with control + C to interrupt the program.

For graphic result, checkout screenshot.png
