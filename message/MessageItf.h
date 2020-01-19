#pragma once

#include <string>
#include <QSharedPointer>

namespace Message
{
   struct IMessage;

   typedef QSharedPointer<IMessage> IMessagePtr;

   struct IMessage
   {
      virtual std::string toString() const = 0;

   }; // struct IMessage

} // namespace Message
