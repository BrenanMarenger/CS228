//Brenan Marenger
//This program takes a .mp3 file, connects to an email server, then converts the .mp3 text,
//to base64 code and sends an email to the given address
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fdstream.h>
#include<netinet/in.h>
#include<cstdint>
#include<netdb.h>
#include"PG3.h"
using namespace std;


int main(int argc, char** argv) {

	struct hostent* h;
	sethostent(1);
	h = gethostbyname("mobile.spectrum.net"); //semail.nmu.edu mobile.spectrum.net 47.43.18.12
	endhostent();

	int s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(25);
	sock_in.sin_addr = *(in_addr*)h->h_addr_list[0];
	int c = connect(s, (sockaddr*)& sock_in, sizeof(sockaddr_in));
	if (c < 0) { perror("connect"); exit(1); }
	
	fdostream sout(s);
	fdistream sin(s);

	char* fileName = argv[2];
	ifstream attachment(fileName); 
	string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char ch;
	int padding = 0;
	int flag = 0;
	int b1 = 0;
	int b2 = 0;
	int b3 = 0;
	int b4 = 0;
	int a1 = 0;
	int a2 = 0;
	int a3 = 0;
	int counter = 0;

	string serverMsg;
	getline(sin, serverMsg);
	//connecting to server
	sout << "HELO mobile.spectrum.net" << "\r\n";
	sout.flush();
	getline(sin, serverMsg);

	sout << "MAIL FROM: "<< argv[1] << "\r\n";
	sout.flush();
	getline(sin, serverMsg);

	sout << "RCPT TO: " << argv[1] << "\r\n";
	sout.flush();
	getline(sin, serverMsg);
	
	sout << "DATA" << "\r\n";
	sout.flush();
	getline(sin, serverMsg);
	//headers
	sout << "From: bmarenge@nmu.edu" << "\r\n";
	sout << "To: bmarenge@nmu.edu" << "\r\n";
	sout << "Subject: Test Email" << "\r\n";
	sout << "MIME-Version: 1.0" << "\r\n";
	sout << "Content-Type: multipart/mixed;" << "\r\n";
	sout << "	boundary=\"----=_NextPart_000_0335_01D83532.3689D830\"" << "\r\n";
	sout << "" << "\r\n";
	sout.flush();

	sout << "This is a multi-part message in MIME format." << "\r\n";
	sout << "" << "\r\n";

	sout << "------=_NextPart_000_0335_01D83532.3689D830" << "\r\n";
	sout << "Content-Type: text/plain;" << "\r\n";
	sout << "" << "\r\n";
	sout << "Listen to my mp3!" << "\r\n";
	sout << "" << "\r\n";

	sout << "------=_NextPart_000_0335_01D83532.3689D830" << "\r\n";
	sout << "Content-Type: audio/mpeg;" << "\r\n";
	sout << "	name=\"" << fileName << "\"" << "\r\n";
	sout << "Content-Transfer-Encoding: base64" << "\r\n";
	sout << "Content-Disposition: attachment;" << "\r\n";
	sout << "	filename=\"" << fileName << "\"" << "\r\n";
	sout << "" << "\r\n";
	sout.flush();

	//base64 encoding
	while (flag == 0) {
		if (attachment.get(ch)) {
			a1 = (unsigned char)ch;
		}
		else {
			break;
		}

		if (attachment.get(ch)) {
			a2 = (unsigned char)ch;
		}
		else {
			padding = 2;
			flag = 1;
			b1 = a1 / 4;
			b2 = a1 % 4 * 16 + a2 / 16;
			break;
		}

		if (attachment.get(ch)) {
			a3 = (unsigned char)ch;
		}
		else {
			padding = 1;
			flag = 1;
			b1 = a1 / 4;
			b2 = a1 % 4 * 16 + a2 / 16;
			b3 = a2 % 16 * 4 + a3 / 64;
			break;
		}

		b1 = a1 / 4;
		b2 = a1 % 4 * 16 + a2 / 16;
		b3 = a2 % 16 * 4 + a3 / 64;
		b4 = a3 % 64;

		sout << base64[b1];
		counter++;
		if (counter == 76) {
			sout << "\n";
			counter = 0;
		}
		sout << base64[b2];
		counter++;
		if (counter == 76) {
			sout << "\n";
			counter = 0;
		}
		sout << base64[b3];
		counter++;
		if (counter == 76) {
			sout << "\n";
			counter = 0;
		}
		sout << base64[b4];
		counter++;
		if (counter == 76) {
			sout << "\n";
			counter = 0;
		}
		a1 = 0;
		a2 = 0;
		a3 = 0;
	}
	if (padding == 1) {
		sout << base64[b1];
		sout << base64[b2];
		sout << base64[b3];
		sout << "=";

	}
	if (padding == 2) {
		sout << base64[b1];
		sout << base64[b2];
		sout << "==";
	}
	attachment.close();
	sout << "\r\n"; 
	sout.flush();
	
	sout << "" << "\r\n";
	sout << "------=_NextPart_000_0335_01D83532.3689D830--" << "\r\n";
	sout << "" << "\r\n";

	sout << "." << "\r\n";
	sout.flush();
	getline(sin, serverMsg);
	//disconnecting from server
	sout << "QUIT" << "\r\n";
	sout.flush();
	getline(sin, serverMsg);

	sout.close();
	return 0;
}