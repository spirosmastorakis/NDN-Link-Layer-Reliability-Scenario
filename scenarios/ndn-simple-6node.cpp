/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-simple.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
using ns3::ndn::StackHelper;
using ns3::ndn::AppHelper;
using ns3::ndn::GlobalRoutingHelper;
using ns3::ndn::StrategyChoiceHelper;


namespace ns3 {

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);
    return result;
}


int queueLength1 = 0, queueLength0 = 0;
ofstream queueLength1File, queueLength0File;

void
enqueue1 (Ptr<const Packet> p)
{
  queueLength1++;
  queueLength1File << float(Simulator::Now().GetMicroSeconds())/1000 << "," <<  queueLength1 << endl;
  // cout << "1 : " << queueLength1 << endl;
}

void
enqueue0 (Ptr<const Packet> p)
{
  queueLength0++;
  queueLength0File << float(Simulator::Now().GetMicroSeconds())/1000 << "," << queueLength0 << endl;
  // cout << "0 : " << queueLength0 << endl;
}

void
dequeue1 (Ptr<const Packet> p)
{
  queueLength1--;
  queueLength1File << float(Simulator::Now().GetMicroSeconds())/1000 << "," << queueLength1 << endl;
  // cout << "1 : " << queueLength1 << endl;
}

void
dequeue0 (Ptr<const Packet> p)
{
  queueLength0--;
  queueLength0File << float(Simulator::Now().GetMicroSeconds())/1000 << "," << queueLength0 << endl;
  // cout << "0 : " << queueLength0 << endl;
}


int
experiment(int argc, char* argv[])
{
  queueLength0File.open("NetDevice-0-queueLength.txt");
  queueLength1File.open("NetDevice-1-queueLength.txt");

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  double prob = 0.05;
  int runNumber = 1;
  CommandLine cmd;
  cmd.AddValue ("prob", "Error probability to be used for nodes", prob);
  cmd.AddValue ("runNumber", "Run number to be used", runNumber);

  cmd.Parse (argc,argv);

  if (prob > 1)
  {
    std::cout << "Error probability can't be greater than 1" << endl;
    exit (1);
  }
  ns3::RngSeedManager::SetRun (runNumber);
  cout << "Error probability = " << prob << endl;
  cout << "Run number = " << RngSeedManager::GetRun () << endl;

  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("5ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("80"));

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(6);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  NetDeviceContainer d0d1 = p2p.Install(nodes.Get(0), nodes.Get(1));
  NetDeviceContainer d1d2 = p2p.Install(nodes.Get(1), nodes.Get(2));
  NetDeviceContainer d2d3 = p2p.Install(nodes.Get(2), nodes.Get(3));
  NetDeviceContainer d3d4 = p2p.Install(nodes.Get(3), nodes.Get(4));
  NetDeviceContainer d4d5 = p2p.Install(nodes.Get(4), nodes.Get(5));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/best-route");

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("900"));
  consumerHelper.SetAttribute("MaxSeq", IntegerValue(20000));
  consumerHelper.SetAttribute("LifeTime",TimeValue(MilliSeconds(600)));
  consumerHelper.SetAttribute("Randomize",StringValue("exponential"));
  ApplicationContainer consumerApps = consumerHelper.Install(nodes.Get(0));
  consumerApps.Get(0)->SetStopTime(MilliSeconds(99999));


  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(5)); // last node


  Config::SetDefault ("ns3::RateErrorModel::ErrorRate", DoubleValue (prob));
  Config::SetDefault ("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));

  ObjectFactory factory;
  factory.SetTypeId ("ns3::RateErrorModel");
  Ptr<ErrorModel> em = factory.Create<ErrorModel> ();
  d0d1.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d0d1.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d1d2.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d1d2.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d2d3.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d2d3.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d3d4.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d3d4.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d4d5.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  d4d5.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));


  GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
  ndnGlobalRoutingHelper.AddOrigins("/prefix", nodes.Get(5));
  GlobalRoutingHelper::CalculateRoutes();

  Config::ConnectWithoutContext("/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue",MakeCallback(dequeue1));
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue",MakeCallback(dequeue0));
  Config::ConnectWithoutContext("/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue",MakeCallback(enqueue1));
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue",MakeCallback(enqueue0));
  
  Simulator::Stop(Seconds(200));
  Simulator::Run();
  
  cout << "device 0-1 : " << d0d1.Get(0)->m_bytesTransmitted << endl;
  cout << "device 1-0 : " << d0d1.Get(1)->m_bytesTransmitted << endl;
  cout << "device 1-2 : " << d1d2.Get(0)->m_bytesTransmitted << endl;
  cout << "device 2-1 : " << d1d2.Get(1)->m_bytesTransmitted << endl;
  cout << "device 2-3 : " << d2d3.Get(0)->m_bytesTransmitted << endl;
  cout << "device 3-2 : " << d2d3.Get(1)->m_bytesTransmitted << endl;
  cout << "device 3-4 : " << d3d4.Get(0)->m_bytesTransmitted << endl;
  cout << "device 4-3 : " << d3d4.Get(1)->m_bytesTransmitted << endl;
  cout << "device 4-5 : " << d4d5.Get(0)->m_bytesTransmitted << endl;
  cout << "device 5-4 : " << d4d5.Get(1)->m_bytesTransmitted << endl;

  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  ns3::experiment(argc, argv);
  return 0;
}
