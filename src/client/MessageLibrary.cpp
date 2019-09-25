#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <json/json.h>
/**
 * Copyright 2015 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Purpose: demonstrate json serialization and deserialization in C++
 * Ser321 Foundations of Distributed Applications
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2015
 */

class MessageLibrary {
private:
	string name, userId, userPwd;
	std::map<std::string, MessageLibrary> message;
	MessageLibrary();
	~MessageLibrary() {
		message.clear();
	}
	;
	MessageLibrary(string aUserId, string aUserPwd);
	void setValues(string aUserId, string aUserPwd);
	void print();
	void initGroupFromJsonFile(string jsonFileName);
	void toJsonFile(string jsonFileName);
	void add(Message message);
	void remove(Message message);
	MessageLibrary get(string usrName);
	std::vector<string> getNames();
	void printGroup();

	void initGroupFromJsonFile(string jsonFileName) {
		ifstream jsonFile(jsonFileName.c_str());
		string line;
		cout << "The content of the Json file as a string: " << endl;
		if (jsonFile.is_open()) {
			while (getline(jsonFile, line)) {
				cout << line << endl;
			}
		} else {
			cout << "Json file not opened properly" << endl;
		}
		jsonFile.close();
		Json::Reader reader;
		Json::Value root;
		std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
		bool parseSuccess = reader.parse(json, root, false);
		if (parseSuccess) {
			//cout << "successful parse" << endl;
			Json::Value::Members mbr = root.getMemberNames();
			for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end();
					i++) {
				//cout << *i << " " << endl;
				Json::Value jsonUser = root[*i];
				string nameStr = "name";
				if (nameStr.compare(*i) == 0) {
					name = jsonUser.asString();
					//cout << "found name " << *i << " value: " << name << endl;
				} else {
					//cout << "found " << *i << endl;
					string userId = jsonUser["userId"].asString();
					string userPwd = jsonUser["userPwd"].asString();
					//cout << "pwd: " << userPwd << " user: " << userId << endl;
					MessageLibrary *messsage = new MessageLibrary(userId,
							userPwd);
					//user->print();
					message[*i] = *messsage;
				}
			}
		}
	}

	void toJsonFile(string jsonFileName) {
		Json::Value jsonLib;
		jsonLib["name"] = name;
		for (std::map<string, MessageLibrary>::iterator i = message.begin();
				i != message.end(); i++) {
			string key = i->first;
			//cout << key << " " << endl;
			Json::Value jsonUser;
			MessageLibrary usr = message[key];
			jsonUser["userId"] = usr.userId;
			jsonUser["userPwd"] = usr.userPwd;
			jsonLib[key] = jsonUser;
		}
		Json::StyledStreamWriter ssw("  ");
		std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
		ssw.write(jsonOutFile, jsonLib);
	}

//	void add(MessageLibrary message){
//	   message[usr.userId]=usr;
//	}
//
//	void remove(MessageLibrary message){
//	   message.erase(usr.userId);
//	}
//
//	get(string usrName){
//	   User ret = message[usrName];
//	   return ret;
//	}

	std::vector<string> getNames() {
		vector < string > myVec;
		for (map<string, MessageLibrary>::iterator it = message.begin();
				it != message.end(); ++it) {
			myVec.push_back(it->first);
			//cout << it->first << "\n";
		}
		return myVec;
	}

	void printGroup() {
		cout << "Group " << name << " has users" << endl;
		for (map<string, MessageLibrary>::iterator it = message.begin();
				it != message.end(); ++it) {
			cout << "key " << it->first << " is ";
			(it->second).print();
		}
		cout << endl;

	}
};
//// do not implement sendClearText in Assign2.
//   public boolean sendClearText(Message aMessage, String fromUser);
//
//   // do not implement sendCipher in Assign2.
//   public boolean sendCipher(Message aMessage, String fromUser);
//
//   // getMessageFromHeaders returns a string array of message headers being sent to toAUserName.
//   // Headers returned are of the form: (from user name @ server and message date)
//   // e.g., a message from J Buffett with header: Jimmy.Buffet  Tue 18 Dec 5:32:29 2018
//   public String[] getMessageFromHeaders(String toAUserName);
//
//   // getMessage returns the Message having the corresponding header. Assume headers are unique.
//   // As above, the header has includes (from user name - server and message date)
//   public Message getMessage(String header);
//
//   // deletes the message having the header (from user name - server and message date)
//   public boolean deleteMessage(String header, String toAUserName);

