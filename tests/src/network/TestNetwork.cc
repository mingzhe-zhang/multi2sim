/*
 *  Multi2Sim
 *  Copyright (C) 2014  Yifan Sun (yifansun@coe.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gtest/gtest.h"

#include <lib/cpp/IniFile.h>
#include <network/Network.h>
#include <network/EndNode.h>
#include <network/Switch.h>
#include <network/Link.h>
#include <network/Bus.h>
#include <network/Connection.h>

namespace net
{

TEST(Network, INI_FILE_TEST_LINK_1)
{
	// Setup ini file
	std::string ini_file =
		"[ Network.mynet ]\n"
		"DefaultInputBufferSize = 16 \n"
		"DefaultOutputBufferSize = 16 \n"
		"DefaultBandwidth = 1 \n"
		"\n"
		"[ Network.mynet.Node.N1 ]\n"
		"Type = EndNode \n"
		"[ Network.mynet.Node.S1 ]\n"
		"Type = Switch \n"
		"[Network.mynet.Link.N1-S1]\n"
		"Type = Bidirectional\n"
		"Source = N1\n"
		"Dest = S1";
	misc::IniFile ini;
	ini.LoadFromString(ini_file);

	// Setup network
	Network net("mynet");
	net.ParseConfiguration("Network.mynet", ini);

	// Assert N1 is created
	Node *n1 = net.getNodeByName("N1");
	EXPECT_TRUE(n1 != nullptr);
	EXPECT_TRUE(dynamic_cast<EndNode *>(n1) != 0);

	// Assert S1 is created
	Node *s1 = net.getNodeByName("S1");
	EXPECT_TRUE(s1 != nullptr);
	EXPECT_TRUE(dynamic_cast<Switch *>(s1) != 0);

	// Assert link n1-s1 is created
	Connection *n1_s1 = net.getConnectionByName(
			"link_<N1.out_buf_0>_<S1.in_buf_0>");
	EXPECT_TRUE(!strcasecmp(dynamic_cast<Link *>
		(n1_s1)->getUserAssignedName().c_str(), "N1-S1"));
	EXPECT_TRUE(n1_s1 != nullptr);
	EXPECT_TRUE(dynamic_cast<Link *>(n1_s1) != 0);

	Connection *s1_n1 = net.getConnectionByName(
			"link_<S1.out_buf_0>_<N1.in_buf_0>");
	EXPECT_TRUE(!strcasecmp(dynamic_cast<Link *>
		(s1_n1)->getUserAssignedName().c_str(), "N1-S1"));
	EXPECT_TRUE(s1_n1 != nullptr);
	EXPECT_TRUE(dynamic_cast<Link *>(s1_n1) != 0);
};

TEST(Network, INI_FILE_TEST_BUS_1)
{
	// Setup ini file
	std::string ini_file = 
		"[ Network.mynet ]\n"
		"DefaultInputBufferSize = 16 \n"
		"DefaultOutputBufferSize = 16 \n"
		"DefaultBandwidth = 1 \n"
		"\n"
		"[ Network.mynet.Node.N1 ]\n"
		"Type = EndNode \n"
		"[ Network.mynet.Node.S1 ]\n"
		"Type = Switch \n"
		"[Network.mynet.BUS.B1]\n"
		"[Network.mynet.BUS.B2]\n"
		"Lanes = 3";
	misc::IniFile ini;
	ini.LoadFromString(ini_file);

	// Setup network
	Network net("mynet");
	net.ParseConfiguration("Network.mynet", ini);

	// Assert N1 is created 
	Node *n1 = net.getNodeByName("N1");
	EXPECT_TRUE(n1 != nullptr);
	EXPECT_TRUE(dynamic_cast<EndNode *>(n1) != 0);

	// Assert S1 is created
	Node *s1 = net.getNodeByName("S1");
	EXPECT_TRUE(s1 != nullptr);
	EXPECT_TRUE(dynamic_cast<Switch *>(s1) != 0);

	// Assert link n1-s1 is created
	Connection *B1 = net.getConnectionByName("B1");
	EXPECT_TRUE(B1 != nullptr);
	Bus * b1 = dynamic_cast<Bus *> (B1);
	EXPECT_TRUE(b1 != 0);
	EXPECT_TRUE(b1->getNumberLanes() == 1);

	Connection *B2 = net.getConnectionByName("B2");
	EXPECT_TRUE( B2 != nullptr );
	Bus * b2 = dynamic_cast<Bus *> (B2);
	EXPECT_TRUE(b2 != 0);
	EXPECT_TRUE(b2->getNumberLanes() == 3);
	for (int i = 0 ; i < b2->getNumberLanes(); i++)
	{
		Bus::Lane *lane;
		lane = b2->getLaneByIndex(i);
		EXPECT_TRUE( lane != nullptr);
	}


	EXPECT_TRUE(net.getNumberConnections() == 2);


}

}

