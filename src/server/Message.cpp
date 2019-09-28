#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
//#include "MessageLibrary.cpp"
#include "../server/MessageGui.cpp"

#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

/**
 * Copyright (c) 2019 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Message management.
 * This class extends the Gui component class MessageGui and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2019
 **/
class Message: public MessageGui {

	/** ClickedX is one of the callbacks for GUI controls.
	 * Callbacks need to be static functions. But, static functions
	 * cannot directly access instance data. This program uses "userdata"
	 * to get around that by passing the instance to the callback
	 * function. The callback then accesses whatever GUI control object
	 * that it needs for implementing its functionality.
	 */
	static void ClickedX(Fl_Widget *w, void *userdata) {
		std::cout << "You clicked Exit" << std::endl;
		exit(1);
	}

	static void ClickedGet(Fl_Widget *w, void *userdata) {
		// since this is a static method, it does not have access to any instance variables.
		// get access through the userdata argument which is the instance.
		Message *anInstance = (Message*) userdata;
		std::cout << "You clicked Get" << std::endl;
		Fl_Multiline_Input *messageFMI = anInstance->messageFMI;
		Fl_Input *dateFI = anInstance->dateFI;
		Fl_Input *subjectFI = anInstance->subjectFI;
		Fl_Input *toFI = anInstance->toFI;
		Fl_Input *fromFI = anInstance->fromFI;
		Fl_Check_Browser *headers = anInstance->headerFCB;

		// clear all existing entries from the list of message headers
		headers->clear();

		// lets setup some sample entries in the FCB
		// first get the date and place its string form into dateFI
		// get the time in a string
		auto aClk = std::chrono::system_clock::now();
		auto aTime = std::chrono::system_clock::to_time_t(aClk);
		std::string aDateStr(ctime(&aTime));
		// the conversion to string adds a new line at the end of the string. Trim it.
		std::string trimmed(trimMe(aDateStr));
		// load the date field with the current time in string form
		//dateFI->value(ctime(&aTime));
		dateFI->value(trimmed.c_str());
		std::cout << "date un-trimmed: " << aDateStr
				<< ". Date from the UI control: " << dateFI->value()
				<< std::endl;
		// get the instance's userId string so we know who is sending messages
		std::string userId = anInstance->userId;
		// all FLTK controls are loaded via c strings, not c++ string objects.
		headers->add((userId + "   " + trimmed).c_str());
		// select this first element in the list. the first arg is the index into the
		// list (1st entry) beginning at 1. The second arg is 0 for unchecked and 1 for
		// checked.
		headers->checked(1, 1);
		// fill in the corresponding fields as if we are displaying a real message.
		toFI->value(userId.c_str());
		fromFI->value(userId.c_str());
		dateFI->value(trimmed.c_str());
		subjectFI->value("Sample Message");
		std::string msg(
				"Hello,\n    FLTK stands for Full Light ToolKit\nEnjoy!");
		messageFMI->value(msg.c_str());
		// add another header
		headers->add("Kevin.Gary   Thu Dec 20 1:25:00 2018");
	}

	static void ClickedReply(Fl_Widget *w, void *userdata) {
//		Reply - (required functionality)
//		Switches the right panel fields from display to compose.
//		Swaps the to and from fields, pre-pends Re:
//		to the Subject field and modifies the message content
//		text area to allow new text to be entered before the old message.

		Message *anInstance = (Message*) userdata;
		std::cout << "You clicked Reply" << std::endl;
		Fl_Multiline_Input *messageFMI = anInstance->messageFMI;
		Fl_Input *subjectFI = anInstance->subjectFI;
		Fl_Input *toFI = anInstance->toFI;
		Fl_Input *fromFI = anInstance->fromFI;
		Fl_Check_Browser *headers = anInstance->headerFCB;

		fromFI->position(705, 75);
		toFI->position(400, 75);

		subjectFI->value("Re:");
		std::string msg(
				"Reply");
		messageFMI->value(msg.c_str());
	}

	static void ClickedSend(Fl_Widget *w, void *userdata) {
//		Send Text - (remote communication required in a future assignment,
//		not this one) Uses the appropriate remote method
//		to send a clear text message to the Message Server
//		as indicated by the To field.
//		You may use this button in this assignment to
//		add a new entry to the message library.

		Message *anInstance = (Message*) userdata;
		std::cout << "You clicked Send" << std::endl;
		Fl_Multiline_Input *messageFMI = anInstance->messageFMI;
		Fl_Input *dateFI = anInstance->dateFI;
		Fl_Input *subjectFI = anInstance->subjectFI;
		Fl_Input *toFI = anInstance->toFI;
		Fl_Input *fromFI = anInstance->fromFI;
		Fl_Check_Browser *headers = anInstance->headerFCB;
	}

	static void ClickedExport(Fl_Widget *w, void *userdata) {
//		Send Cipher - (required in a future assignment,
//		not this one) Uses the appropriate remote method
//		to send a cypher text to the Message Server as
//		indicated by the To field. The cipher text should
//		be handled by both the sender and receiver using
//		password based encryption. You should encrypt/decrypt
//		a string that consists of the From, Subject and message
//		content fields in that order separated by ^ (up-arrow)
//		characters. Up-arrow cannot appear in the
//		from or subject fields.

		Message *anInstance = (Message*) userdata;
		std::cout << "You clicked Export" << std::endl;
		Fl_Multiline_Input *messageFMI = anInstance->messageFMI;
		Fl_Input *dateFI = anInstance->dateFI;
		Fl_Input *subjectFI = anInstance->subjectFI;
		Fl_Input *toFI = anInstance->toFI;
		Fl_Input *fromFI = anInstance->fromFI;
		Fl_Check_Browser *headers = anInstance->headerFCB;
	}

	static void ClickedDelete(Fl_Widget *w, void *userdata) {
//		Delete - (required functionality)
//		Removes the message that is currently
//		selected in the message list.

		Message *anInstance = (Message*) userdata;
		Fl_Input *from = anInstance->fromFI;
		std::string fr(from->value());
		Fl_Check_Browser *headers = anInstance->headerFCB;

		// clear all existing entries from the list of message headers
		headers->clear_active();

		std::cout << "You clicked Delete" << std::endl;
	}

	static std::string& trimMe(std::string &str) {
		// right trim
		while (str.length() > 0
				&& (str[str.length() - 1] == ' '
						|| str[str.length() - 1] == '\t'
						|| str[str.length() - 1] == '\n'
						|| str[str.length() - 1] == '\r')) {
			str.erase(str.length() - 1, 1);
		}
		// left trim
		while (str.length() > 0 && (str[0] == ' ' || str[0] == '\t')) {
			str.erase(0, 1);
		}
		return str;
	}

	static void SelectedHeader(Fl_Widget *w, void *userdata) {
		Message *anInstance = (Message*) userdata;
		Fl_Check_Browser *headers = anInstance->headerFCB;
		int howMany = headers->nitems();
		int which = headers->value();
		std::cout << "You selected a new message header. Index: " << which
				<< std::endl;

		// unselect all but the item just selected. Note, first item is item 1 not 0
		for (int i = 1; i < howMany + 1; i++) {
			if (i != which) {
				headers->checked(i, 0);
				std::cout << "calling checked(" << i << ")" << std::endl;
			}
		}
		std::cout << "You selected item " << which << " text "
				<< headers->text(which) << "\nbrowser size is " << howMany
				<< std::endl;
	}

public:
	std::string userId;
	Message(const char *name = 0) :
			MessageGui(name) {
		getButt->callback(ClickedGet, (void*) this);
		deleteButt->callback(ClickedDelete, (void*) this);
		replyButt->callback(ClickedReply, (void*) this);
		sendButt->callback(ClickedSend, (void*) this);
		exportButt->callback(ClickedExport, (void*) this);
		headerFCB->callback(SelectedHeader, (void*) this);
		headerFCB->when(FL_WHEN_CHANGED);

		callback(ClickedX);

		userId = "Tim.Lindquist";
	}
};

int main() {

//	// invoke with ./bin/studentRPCServer 8080
//	   int port = 1099;
//	   if(argc > 1){
//	      port = atoi(argv[1]);
//	   }
//	   HttpServer httpserver(port);
//	   MessageLibrary ss(httpserver, port);
//	   // to use tcp sockets instead of http uncomment below (comment above), and the include
//	   // for tcpsocketserver.h above. If not running locally, you will need to input ip & port
//	   // from command line for both server and client programs.
//	   //TcpSocketServer tcpserver("localhost",port);
//	   //StudentServer ss(tcpserver, port);
//	   std::atexit(exiting);
//	   auto ex = [] (int i) {cout << "server terminating with signal " << i << endl;
//	                         // ss.StopListening();
//	                         exit(0);
//	                         //return 0;
//	                        };
//	   // ^C
//	   std::signal(SIGINT, ex);
//	   // abort()
//	   std::signal(SIGABRT, ex);
//	   // sent by kill command
//	   std::signal(SIGTERM, ex);
//	   // ^Z
//	   std::signal(SIGTSTP, ex);
//	   cout << "Student collection server listening on port " << port
//	      //<< " press return/enter to quit." << endl;
//	        << " use ps to get pid. To quit: kill -9 pid " << endl;
//	   ss.StartListening();
//	   while(true){
//	   }
//	   //int c = getchar();
//	   ss.StopListening();

	Message cm("Eric Sandlin's Message Browser");
	return (Fl::run());
}
