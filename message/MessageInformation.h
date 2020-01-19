#pragma once

#include <QSharedPointer>
#include <string>

namespace Message
{
   struct CMessageInformation;

   typedef QSharedPointer<CMessageInformation> CMessageInformationPtr;

   struct CMessageInformation
   {
      enum ETypeInformation
      {
         eConnectionToServer,
         eMessageFromServer,
         eGameError
      };

      ETypeInformation m_typeInformation;
      std::string m_strInformation;
   }; // struct CMessageInformation

} // namespace Message

