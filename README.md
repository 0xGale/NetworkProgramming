# NetworkProgramming
IP PacketCapture
When program start,it will capture IP Packet,it will decide IP Protocol field is TCP(0x6),program print source port-> destination port.
if IP Protocol field is UDP(0x11),print Source Port -> Destination Port.
if TCP Packet destination Port field is 80 or 8080 ,print "you are browsing web".
if UDP Pacekt Destination Port field is 20 or 21,print "you are using FTP server".if Port field is 53,print "you are using DNS server".
