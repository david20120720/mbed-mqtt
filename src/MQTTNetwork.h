/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MQTTNETWORK_H_
#define _MQTTNETWORK_H_

#include "NetworkInterface.h"
#include "TCPSocket.h"

class MQTTNetwork {
public:
    MQTTNetwork(NetworkInterface *aNetwork) : network(aNetwork)
    {
        socket = new TCPSocket();
    }

    ~MQTTNetwork()
    {
        delete socket;
    }

    int read(unsigned char *buffer, int len, int timeout)
    {
        int ret = socket->recv(buffer, len);
        if (ret == 0) {
            // A receive size of 0 indicates that the socket
            // was successfully closed so indicate this to MQTTClient
            ret = -1;
        }
        return ret;
    }

    int write(unsigned char *buffer, int len, int timeout)
    {
        int ret = socket->send(buffer, len);
        if (ret == 0) {
            // The socket is closed so indicate this to MQTTClient
            return -1;
        }
        return ret;
    }

    int connect(const char *hostname, int port)
    {
        int ret = NSAPI_ERROR_OK;
        if ((ret = socket->open(network)) != NSAPI_ERROR_OK) {
            return ret;
        }
        return socket->connect(hostname, port);
    }

    int disconnect()
    {
        return socket->close();
    }

private:
    NetworkInterface *network;
    TCPSocket *socket;
};

#endif // _MQTTNETWORK_H_
