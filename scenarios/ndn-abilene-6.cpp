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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "model/ndn-net-device-face.hpp"

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

auto printFib = [](Ptr<Node> node) {
    auto ndn = node->GetObject<ndn::L3Protocol>();
    for (const auto& entry : ndn->getForwarder()->getFib()) {
      cout << entry.getPrefix() << " (";

      bool isFirst = true;
      for (auto& nextHop : entry.getNextHops()) {
        cout << *nextHop.getFace();
        auto face = dynamic_pointer_cast<ndn::NetDeviceFace>(nextHop.getFace());
        if (face == nullptr)
          continue;

        cout << " towards ";

        if (!isFirst)
          cout << ", ";
        if ( face->GetNetDevice()->GetChannel()->GetDevice(1)->GetNode() == node )
          cout << Names::FindName(face->GetNetDevice()->GetChannel()->GetDevice(0)->GetNode());
        else 
          cout << Names::FindName(face->GetNetDevice()->GetChannel()->GetDevice(1)->GetNode());
        isFirst = false;
      }
      cout << ")" << endl;
    }
  };


int queueLength1 = 0, queueLength0 = 0;
ofstream queueLength1File, queueLength0File;

void
enqueue1 (Ptr<const Packet> p)
{
  queueLength1++;
  queueLength1File << float(Simulator::Now().GetMicroSeconds())/1000 << "," << queueLength1 << endl;
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
  Config::SetDefault("ns3::RateErrorModel::ErrorRate", DoubleValue (prob));
  Config::SetDefault("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));

  // Creating nodes
  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/abilene-topology.txt");
  topologyReader.SetDefaultLossRate(prob);
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  //ndnHelper.setCsSize(1);
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix1", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix2", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix3", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix4", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix5", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix6", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::InstallAll("/prefix7", "/localhost/nfd/strategy/best-route");
    
  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute("MaxSeq", IntegerValue(10000));
  consumerHelper.SetAttribute("LifeTime",TimeValue(MilliSeconds(400)));
  //consumerHelper.SetAttribute("Randomize",StringValue("exponential"));
  
  consumerHelper.SetPrefix("/prefix1");
  consumerHelper.SetAttribute("Frequency", StringValue("650"));
  ApplicationContainer consumerApps1 = consumerHelper.Install(Names::Find<Node>("STTLng"));
  consumerApps1.Get(0)->SetStopTime(MilliSeconds(99999));

  consumerHelper.SetPrefix("/prefix2");
  consumerHelper.SetAttribute("Frequency", StringValue("650"));
  ApplicationContainer consumerApps2 = consumerHelper.Install(Names::Find<Node>("SNVAng"));
  consumerApps2.Get(0)->SetStopTime(MilliSeconds(99999));
 
  consumerHelper.SetPrefix("/prefix3");
  consumerHelper.SetAttribute("Frequency", StringValue("880"));
  ApplicationContainer consumerApps3 = consumerHelper.Install(Names::Find<Node>("IPLSng"));
  consumerApps3.Get(0)->SetStopTime(MilliSeconds(99999));

  consumerHelper.SetPrefix("/prefix4");
  consumerHelper.SetAttribute("Frequency", StringValue("220"));
  ApplicationContainer consumerApps4 = consumerHelper.Install(Names::Find<Node>("KSCYng"));
  consumerApps4.Get(0)->SetStopTime(MilliSeconds(99999));

  consumerHelper.SetPrefix("/prefix5");
  consumerHelper.SetAttribute("Frequency", StringValue("440"));
  ApplicationContainer consumerApps5 = consumerHelper.Install(Names::Find<Node>("LOSAng"));
  consumerApps5.Get(0)->SetStopTime(MilliSeconds(99999));

  consumerHelper.SetPrefix("/prefix6");
  consumerHelper.SetAttribute("Frequency", StringValue("440"));
  ApplicationContainer consumerApps6 = consumerHelper.Install(Names::Find<Node>("HSTNng"));
  consumerApps6.Get(0)->SetStopTime(MilliSeconds(99999));

  consumerHelper.SetPrefix("/prefix7");
  consumerHelper.SetAttribute("Frequency", StringValue("880"));
  ApplicationContainer consumerApps7 = consumerHelper.Install(Names::Find<Node>("NYCMng"));
  consumerApps7.Get(0)->SetStopTime(MilliSeconds(99999));


  

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  
  producerHelper.SetPrefix("/prefix1");
  producerHelper.Install(Names::Find<Node>("NYCMng"));
  
  producerHelper.SetPrefix("/prefix2");
  producerHelper.Install(Names::Find<Node>("WASHng"));
  
  producerHelper.SetPrefix("/prefix3");
  producerHelper.Install(Names::Find<Node>("SNVAng"));
  
  producerHelper.SetPrefix("/prefix4");
  producerHelper.Install(Names::Find<Node>("WASHng"));
  
  producerHelper.SetPrefix("/prefix5");
  producerHelper.Install(Names::Find<Node>("DNVRng"));
  
  producerHelper.SetPrefix("/prefix6");
  producerHelper.Install(Names::Find<Node>("STTLng"));
  
  producerHelper.SetPrefix("/prefix7");
  producerHelper.Install(Names::Find<Node>("HSTNng"));
  

  GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
  ndnGlobalRoutingHelper.AddOrigins("/prefix1", Names::Find<Node>("NYCMng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix2", Names::Find<Node>("WASHng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix3", Names::Find<Node>("SNVAng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix4", Names::Find<Node>("WASHng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix5", Names::Find<Node>("DNVRng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix6", Names::Find<Node>("STTLng"));
  ndnGlobalRoutingHelper.AddOrigins("/prefix7", Names::Find<Node>("HSTNng"));
  GlobalRoutingHelper::CalculateRoutes();

  Config::ConnectWithoutContext("/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue",MakeCallback(dequeue1));
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue",MakeCallback(dequeue0));
  Config::ConnectWithoutContext("/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue",MakeCallback(enqueue1));
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue",MakeCallback(enqueue0));

 // cout << "ATLAng : " << endl; printFib(Names::Find<Node>("ATLAng")); cout << endl;
 // cout << "CHINng : " << endl; printFib(Names::Find<Node>("CHINng")); cout << endl;
 // cout << "DNVRng : " << endl; printFib(Names::Find<Node>("DNVRng")); cout << endl;
 // cout << "HSTNng : " << endl; printFib(Names::Find<Node>("HSTNng")); cout << endl;
 // cout << "IPLSng : " << endl; printFib(Names::Find<Node>("IPLSng")); cout << endl;
 // cout << "KSCYng : " << endl; printFib(Names::Find<Node>("KSCYng")); cout << endl;
 // cout << "LOSAng : " << endl; printFib(Names::Find<Node>("LOSAng")); cout << endl;
 // cout << "NYCMng : " << endl; printFib(Names::Find<Node>("NYCMng")); cout << endl;
 // cout << "SNVAng : " << endl; printFib(Names::Find<Node>("SNVAng")); cout << endl;     
 // cout << "STTLng : " << endl; printFib(Names::Find<Node>("STTLng")); cout << endl;
 // cout << "WASHng : " << endl; printFib(Names::Find<Node>("WASHng")); cout << endl;

  Simulator::Stop(Seconds(100));
  Simulator::Run();
  
  long int totalBytes = 0;
  std::list<TopologyReader::Link> links= topologyReader.GetLinks();
  for (std::list<TopologyReader::Link>::iterator it = links.begin(); it != links.end(); it++)
  {
    cout << "Data transmitted: " << it->GetFromNetDevice()->m_bytesTransmitted << " - " << it->GetToNetDevice()->m_bytesTransmitted << " - " << it->GetFromNetDevice()->GetDataRate() << endl;
    totalBytes += it->GetFromNetDevice()->m_bytesTransmitted;
    totalBytes += it->GetToNetDevice()->m_bytesTransmitted;
  }
  fprintf(stderr,"Total Bytes  ");
  cout << totalBytes << endl;


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
