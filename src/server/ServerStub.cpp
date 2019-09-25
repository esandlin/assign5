#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>
#include "MessageLibrary.cpp"
/**
 * Copyright (c) 2016 Tim Lindquist,
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
 * Purpose: C++ class to represent a student. This class is part of a
 * student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu),ASU-IAFSE,Software Engineering
 * @file    Student.hpp
 * @date    July, 2016
 * @license See above
 **/
using namespace jsonrpc;
using namespace std;
class ServerStub : public AbstractStubServer
{
    public:
        ServerStub(AbstractServerConnector &connector);
        virtual void notifyServer();
        virtual std::string sayHello(const std::string& name);
};
ServerStub::ServerStub(AbstractServerConnector &connector) :
    AbstractStubServer(connector)
{
}
void ServerStub::notifyServer()
{
    cout << "Server got notified" << endl;
}
string ServerStub::sayHello(const string &name)
{
    return "Hello " + name;
}
int main()
{
    HttpServer httpserver(8383);
    ServerStub s(httpserver);
    s.StartListening();
    getchar();
    s.StopListening();
    return 0;
}



