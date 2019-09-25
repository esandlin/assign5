/**
 * Copyright 2016 Tim Lindquist,
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
 * <p/>
 * Purpose: C++ class to represent a collection of students. This class is
 * part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include <cmath>

using namespace std;

class MessageLibrary {
protected:
	std::map<std::string, MessageLibrary> library;
	string name;
	int studentid;
	vector<string> takes;
	string toJsonString();
	Json::Value toJson();
	void fromJson(Json::Value json);
	void setValues(string aName, int aStudentId, vector<string> aTakes);
	void print();
	bool resetFromJsonFile(string jsonFileName);
	bool saveToJsonFile(string jsonFileName);
	string serviceInfo();
	bool add(const Json::Value &aStudentJson);
	bool remove(string studentName);
	Json::Value get(string studentName); //returns a student's json
	Json::Value getNames(); //returns a json array of strings
	string getById(int studentid); //returns the students name whose id is studentid

	MessageLibrary() {
		resetFromJsonFile("students.json");
		name = "";
		studentid = 0;
		takes = vector<string>();
	}

	MessageLibrary(string aName, int aStudentId, vector<string> aTakes) {
		name = aName;
		studentid = aStudentId;
		takes = aTakes;
	}

	MessageLibrary(const Json::Value &jsonObj) {
		string nameStr = "name";
		string studentidStr = "studentid";
		string takesStr = "takes";
		Json::Value::Members mbr = jsonObj.getMemberNames();
		for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end();
				i++) {
			Json::Value jsonM = jsonObj[*i];
			if (nameStr.compare(*i) == 0) {
				name = jsonM.asString();
			} else if (studentidStr.compare(*i) == 0) {
				studentid = jsonM.asInt();
			} else if (takesStr.compare(*i) == 0) {
				takes = vector<string>();
				for (int i = 0; i < jsonM.size(); i++) {
					takes.push_back(jsonM[i].asString());
				}
			}
		}
	}

	MessageLibrary(string jsonString) {
		string nameStr = "name";
		string studentidStr = "studentid";
		string takesStr = "takes";
		Json::Reader reader;
		Json::Value root;
		bool parseSuccess = reader.parse(jsonString, root, false);
		if (parseSuccess) {
			Json::Value::Members mbr = root.getMemberNames();
			for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end();
					i++) {
				Json::Value jsonM = root[*i];
				if (nameStr.compare(*i) == 0) {
					name = jsonM.asString();
				} else if (studentidStr.compare(*i) == 0) {
					studentid = jsonM.asInt();
				} else if (takesStr.compare(*i) == 0) {
					takes = vector<string>();
					for (int i = 0; i < jsonM.size(); i++) {
						takes.push_back(jsonM[i].asString());
					}
				}
			}
		} else {
			cout << "Student constructor parse error with input: " << jsonString
					<< endl;
		}
	}

	~MessageLibrary() {
		library.clear();
		name = "";
		studentid = -1;
	}

	string toJsonString() {
		string ret = "{}";
		Json::Value jsonLib;
		jsonLib["name"] = name;
		jsonLib["studentid"] = studentid;
		Json::Value tmp(Json::arrayValue);
		for (int i = 0; i < takes.size(); i++) {
			tmp[i] = takes[i];
		}
		jsonLib["takes"] = tmp;
		ret = jsonLib.toStyledString();
		return ret;
	}

	Json::Value toJson() {
		Json::Value jsonLib;
		jsonLib["name"] = name;
		jsonLib["studentid"] = studentid;
		Json::Value tmp(Json::arrayValue);
		for (int i = 0; i < takes.size(); i++) {
			tmp[i] = takes[i];
		}
		jsonLib["takes"] = tmp;
		return jsonLib;
	}

	void setValues(string aName, int aStudentId, vector<string> aTakes) {
		name = aName;
		studentid = aStudentId;
		takes = aTakes;
	}

	void print() {
		cout << "Student " << name << " with ID " << studentid << " takes ";
		for (int i = 0; i < takes.size(); i++) {
			cout << takes[i] << " ";
		}
		cout << endl;
	}

	bool resetFromJsonFile(string jsonFileName) {
		bool ret = false;
		Json::Reader reader;
		Json::Value root;
		std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
		bool parseSuccess = reader.parse(json, root, false);
		if (parseSuccess) {
			Json::Value::Members mbr = root.getMemberNames();
			for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end();
					i++) {
				//cout << *i << " " << endl;
				Json::Value jsonMedia = root[*i];
				MessageLibrary *aMessage = new MessageLibrary(jsonMedia);
				library[*i] = *aMessage;
				//cout << "adding ";
				//aMessage->print();
			}
			ret = true;
		}
		return ret;
	}

	bool saveToJsonFile(string jsonFileName) {
		bool ret = false;
		Json::Value jsonLib;
		for (std::map<string, MessageLibrary>::iterator i = library.begin();
				i != library.end(); i++) {
			string key = i->first;
			//cout << key << " " << endl;
			MessageLibrary aMessage = library[key];
			Json::Value jsonStudent = aMessage.toJson();
			jsonLib[key] = jsonStudent;
		}
		ret = true;
		Json::StyledStreamWriter ssw("  ");
		std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
		ssw.write(jsonOutFile, jsonLib);
		return ret;
	}

	bool add(const Json::Value &aStudentJson) {
		bool ret = false;
		MessageLibrary aMessage(aStudentJson);
		// The map container does not allow duplicates so adding will replace existing
		// value with new Student instance.
		//aStudent.print();
		library[aMessage.name] = aMessage;
		return true;
	}

	bool remove(string aName) {
		bool ret = false;
		// does the library contain this student? if so remove, if not return false
		if (library.count(aName) > 0) {
			library.erase(aName);
			ret = true;
		}
		return true;
	}

	Json::Value get(string aName) {
		MessageLibrary aMessage(
				"{\"name\":\"" + aName
						+ " Unknown\",\"studentid\":-1,\"takes\":[\"None\" ]}");
		if (library.count(aName) > 0) {
			aMessage = library[aName];
		}
		return aMessage.toJson();
	}

	Json::Value getNames() {
		Json::Value ret(Json::arrayValue);
		vector<string> myVec;
		for (map<string, MessageLibrary>::iterator it = library.begin();
				it != library.end(); ++it) {
			myVec.push_back(it->first);
		}
		for (std::vector<string>::iterator it = myVec.begin();
				it != myVec.end(); ++it) {
			ret.append(Json::Value(*it));
		}
		return ret;
	}

	std::string getById(int studentid) {
		std::string ret = "";
		for (map<string, MessageLibrary>::iterator it = library.begin();
				it != library.end(); ++it) {
			MessageLibrary stud = library[it->first];
			if (stud.studentid == studentid) {
				ret = stud.name;
			}
		}
		return ret;
	}

};
