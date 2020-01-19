#include <stdlib.h>
#include <QString>

#include "Parser.h"

namespace ServerManagerDecl
{
/// constructor
   CParser::CParser()
   {
      m_vMsgStrEnumType.push_back(std::make_pair("SC_CONNID", eConfirmConnect));
      m_vMsgStrEnumType.push_back(std::make_pair("SC_ERR", eError));
      m_vMsgStrEnumType.push_back(std::make_pair("SC_FINISH", eFinishGame));
      m_vMsgStrEnumType.push_back(std::make_pair("SC_STATE", eStateMap));
      m_vMsgStrEnumType.push_back(std::make_pair("SC_START", eStartGame));
      m_vMsgStrEnumType.push_back(std::make_pair("SC_TIMETOSTART", eTimeToStart));
   }

   CParser::~CParser()
   {
   }

/// function for parse
   CParser::ETypeMessage CParser::CheckTypeMessage(const std::string& sMes) const
   {
      std::vector< std::pair<std::string, ETypeMessage> >::const_iterator it =
            m_vMsgStrEnumType.begin();

      for(; it != m_vMsgStrEnumType.end(); ++it)
      {
         if(sMes.find(it->first) == 0)
         {
            return (*it).second;
         }
      }
      return eUnknown;
   }

   const Message::CMessageConfirmationConnectToServerPtr
      CParser::ParseMConfirmConnect(const std::string& sMes)
   {      
      /// SC_CONNID#3##
      Message::CMessageConfirmationConnectToServerPtr ptr(
         new Message::CMessageConfirmationConnectToServer);

      ptr->m_playerID = parseMesToUInt(sMes);

      m_validMessage = true;

      return ptr;
   }

   const Message::CMessageErrorPtr
      CParser::ParseMError(const std::string& sMes)
   {
      /// SC_ERR#Player1 disconnected!##
      size_t posBegPart = sMes.find_first_of('#');
      size_t posEndPart = sMes.find_first_of('#', posBegPart + 1);

      Message::CMessageErrorPtr ptr( new Message::CMessageError);

      ptr->m_strError = sMes.substr(posBegPart + 1, posEndPart - posBegPart - 1);

      m_validMessage = true;

      return ptr;
   }

   const Message::CMessageFinishGamePtr
      CParser::ParseMFinishGame(const std::string& sMes)
   {
      /// SC_FINISH#2##
      Message::CMessageFinishGamePtr ptr(
         new Message::CMessageFinishGame);

      ptr->m_playerID = parseMesToUInt(sMes);

      m_validMessage = true;

      return ptr;
   }

   const Message::CMessageStateMapPtr
      CParser::ParseMStateMap(const std::string& sMes)
   {
      /// SC_STATE#(2,0,15)(3,0,8)(4,4,8)#(1,2,4,2,12,0)(2,2,3,2,24,0)##
      Message::CMessageStateMapPtr ptr(
         new Message::CMessageStateMap);

      size_t posBegPart = 0;
      size_t posEndPart = 0;
      size_t posFrstBkt = 0;
      size_t posScndBkt = 0;

      /// parse planet state
      Message::CStatePlanet tempPlanet;

      posBegPart = sMes.find('#', posEndPart + 1);
      posEndPart = sMes.find('#', posBegPart + 1);
      posScndBkt = posBegPart;

      do
      {
         posFrstBkt = sMes.find('(', posScndBkt);
         posScndBkt = sMes.find(')', posFrstBkt + 1);

         parseSubMesToVectorUInt(sMes, ",", posFrstBkt, posScndBkt);

         if(m_vParseSubMesUInt.size() < 3)
         {
            setParseMessage("Error, bad message");
            m_validMessage = false;
            return ptr;
         }

         tempPlanet.m_planetID = m_vParseSubMesUInt[0];
         tempPlanet.m_playerID = m_vParseSubMesUInt[1];
         tempPlanet.m_countFleet = m_vParseSubMesUInt[2];

         ptr->m_planetState.push_back(tempPlanet);
      }
      while(posScndBkt != (posEndPart - 1));

      /// parse fleet state
      Message::CStateFleet tempFleet;

      posBegPart = posEndPart;
      posEndPart = sMes.find('#', posBegPart + 1);
      posScndBkt = posBegPart;

      do
      {
         posFrstBkt = sMes.find('(', posScndBkt);
         posScndBkt = sMes.find(')', posFrstBkt + 1);

         parseSubMesToVectorUInt(sMes, ",", posFrstBkt, posScndBkt);

         if(m_vParseSubMesUInt.size() < 6)
         {
            setParseMessage("Error, bad message");
            m_validMessage = false;
            return ptr;
         }

         tempFleet.m_fleetID = m_vParseSubMesUInt[0];
         tempFleet.m_playerID = m_vParseSubMesUInt[1];
         tempFleet.m_planetStartID = m_vParseSubMesUInt[2];
         tempFleet.m_planetFinishID = m_vParseSubMesUInt[3];
         tempFleet.m_countFleet = m_vParseSubMesUInt[4];
         tempFleet.m_percentRoute = m_vParseSubMesUInt[5];

         ptr->m_fleetState.push_back(tempFleet);
      }
      while(posScndBkt != (posEndPart - 1));

      m_validMessage = true;

      return ptr;
   }

   const Message::CMessageStartMapGamePtr
      CParser::ParseMStartMapGame(const std::string& sMes)
   {
      /// SC_START#100#80#2#25#(3,0,50,70,45,8)(4,2,80,60,12,20)#(1,Red Fox)(2,Star_123456)##
      Message::CMessageStartMapGamePtr ptr(
         new Message::CMessageStartMapGame);

      size_t posBegPart = 0;
      size_t posEndPart = 0;
      size_t posFrstBkt = 0;
      size_t posScndBkt = 0;

      posBegPart = sMes.find('#');
      posEndPart = sMes.find('(', posBegPart + 1);
      --posEndPart;

      parseSubMesToVectorUInt(sMes, "#", posBegPart, posEndPart);

      if(m_vParseSubMesUInt.size() < 4)
      {
         setParseMessage("Error, bad message");
         m_validMessage = false;
         return ptr;
      }

      ptr->m_mapX = m_vParseSubMesUInt[0];
      ptr->m_mapY = m_vParseSubMesUInt[1];
      ptr->m_growV = m_vParseSubMesUInt[2];
      ptr->m_flyV = m_vParseSubMesUInt[3];

      /// parse planet start data
      Message::CPlanetStartData tempPlanet;

      posBegPart = posEndPart;
      posEndPart = sMes.find('#', posBegPart + 1);
      posScndBkt = posBegPart;

      do
      {
         posFrstBkt = sMes.find('(', posScndBkt);
         posScndBkt = sMes.find(')', posFrstBkt + 1);

         parseSubMesToVectorUInt(sMes, ",", posFrstBkt, posScndBkt);

         if(m_vParseSubMesUInt.size() < 6)
         {
            setParseMessage("Error, bad message");
            m_validMessage = false;
            return ptr;
         }

         tempPlanet.m_planetID = m_vParseSubMesUInt[0];
         tempPlanet.m_playerID = m_vParseSubMesUInt[1];
         tempPlanet.m_planetX = m_vParseSubMesUInt[2];
         tempPlanet.m_planetY = m_vParseSubMesUInt[3];
         tempPlanet.m_planetR = m_vParseSubMesUInt[4];
         tempPlanet.m_countFleet = m_vParseSubMesUInt[5];

         ptr->m_planetData.push_back(tempPlanet);
      }
      while(posScndBkt != (posEndPart - 1));

      /// parse player start data
      Message::CPlayerStartData tempPlayer;

      posBegPart = posEndPart;
      posEndPart = sMes.find('#', posBegPart + 1);
      posScndBkt = posBegPart;
      size_t posSep = 0;

      do
      {
         posFrstBkt = sMes.find('(', posScndBkt);
         posScndBkt = sMes.find(')', posFrstBkt + 1);
         posSep = sMes.find(',', posFrstBkt);

         tempPlayer.m_playerID =
            convertStdStrToUInt(sMes.substr(posFrstBkt + 1, posSep - posFrstBkt - 1));
         tempPlayer.m_playerName = sMes.substr(posSep + 1, posScndBkt - posSep - 1);

         ptr->m_playerData.push_back(tempPlayer);
      }
      while(posScndBkt != (posEndPart - 1));

      m_validMessage = true;

      return ptr;
   }

   const Message::CMessageTimeToStartGamePtr
      CParser::ParseMTimeToStartGame(const std::string& sMes)
   {
      /// SC_TIMETOSTART#9##
      Message::CMessageTimeToStartGamePtr ptr(
         new Message::CMessageTimeToStartGame);

      ptr->m_second = parseMesToUInt(sMes);

      m_validMessage = true;

      return ptr;
   }

   unsigned int CParser::convertStdStrToUInt(const std::string& str)
   {
      return QString(str.c_str()).toUInt();
   }

   void CParser::parseSubMesToVectorUInt(
      const std::string& sMes,
      const std::string& separator,
      const size_t posF,
      const size_t posE)
   {
      m_vParseSubMesUInt.clear();
      size_t posFB = posF;
      size_t posSB = sMes.find(separator, posFB + 1);
      for(; (posSB != std::string::npos) && (posSB < posE);
          posFB = posSB , posSB = sMes.find(separator, posFB + 1))
      {
         m_vParseSubMesUInt.push_back(
            convertStdStrToUInt(sMes.substr(posFB + 1, posSB - posFB - 1)));
      }

      m_vParseSubMesUInt.push_back(
         convertStdStrToUInt(sMes.substr(posFB + 1, posE - posFB - 1)));
   }

   unsigned int CParser::parseMesToUInt(const std::string& sMes)
   {
      size_t posBegPart = sMes.find_first_of('#');
      size_t posEndPart = sMes.find_first_of('#', posBegPart + 1);

      return convertStdStrToUInt(sMes.substr(posBegPart + 1, posEndPart - posBegPart - 1));
   }

   void CParser::setParseMessage(const std::string& str)
   {
      m_parseMessage = str;
   }

   const Message::CMessageInformationPtr CParser::GetParserMessage() const
   {
      Message::CMessageInformationPtr ptr(
               new Message::CMessageInformation);

      ptr->m_typeInformation = Message::CMessageInformation::eMessageFromServer;
      ptr->m_strInformation = m_parseMessage;

      return ptr;
   }

   bool CParser::ValidMessage() const
   {
      return m_validMessage;
   }

} //namespace ServerManagerDecl


