#pragma once

#include <string>
#include <QSharedPointer>

namespace Message
{
   struct CMessageError;

   typedef QSharedPointer<CMessageError> CMessageErrorPtr;

   struct CMessageError
   {
      std::string m_strError;   

   }; // struct CMessageError
} // namespace Message
